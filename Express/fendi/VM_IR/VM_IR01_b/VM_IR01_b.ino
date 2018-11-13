#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <avr/wdt.h>
#include "Timer.h"

#define Pin_Relay 8
#define Pin_Buzzer 9
#define Pin_Sensor1 2 //logic AND, logic LOW jika kena obstacle (Falling)
#define Pin_Sensor2 3 //logic OR, logic HIGH jika kena obstacle (Rising)

//===========================================================
#define Limit_Sensor 1
//===========================================================

// Update these with values suitable for your network.

byte mac[] = {0x28, 0x88, 0x7D, 0xFF, 0xFF, 0xFF};

IPAddress ip,subnet,gateway,server;
IPAddress ip_dns(0, 0, 0, 0);

int port;
int addrDIP = 1;
int addrGIP = 18;
int addrNIP = 35;
int addrSIP = 52;
int addrPORT = 69;

EthernetClient client;

Timer timer1;

byte Reconnect_count=0;
byte Sensor_0=0;  //0:ready ; 1:detect
byte Sensor_1=0;
byte Sensor_Count;

//----------------------------------------------------------------------
void callback(char* topic, byte* payload, unsigned int length) {
  String param = topic;
  String value;

  Serial.print("Rx =[");
  Serial.print(param);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    value+=(char)payload[i];
  }
  Serial.println();

  if (param=="IR"){
    if (value=="IRON$"){
      digitalWrite(Pin_Relay,HIGH);  
      Sensor_Count=Limit_Sensor;
      Sensor_0=2;      
      Sensor_1=2;      
    }else if (value=="IROFF$"){
      digitalWrite(Pin_Relay,LOW);        
      Sensor_Count=Limit_Sensor;
      Sensor_0=2;      
      Sensor_1=2;      
    }
  }
}
//----------------------------------------------------------------------

EthernetClient Client;

//----------------------------------------------------------------------
void reconnect() {
  // Loop until we're reconnected
  if (!client.connected()) {
    Serial.print("MQTT => ");
    // Attempt to connect
    if (client.connect("VM_IR_1")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic","hello world");
      // ... and resubscribe
      client.subscribe("IR/#");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
//      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
//      delay(5000);
    }
  }
}

//================================================================
void setup()
{
  Serial.begin(57600);
  Serial.println("Starting");

  io();
  delay(1000);
  EEPROM_Setup();
  delay(2000);
  Ethernet.begin(mac, ip, ip_dns, gateway, subnet);
  delay(300);

  // device identification
  Serial.begin(115200);
  Serial.println("Device information");
  //*
  Serial.print("MAC: ");
  Serial.print(mac[0],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.println(mac[5],HEX);
  //*/  
  
  ip = Ethernet.localIP();
  Serial.print("IP: ");
  Serial.println(IpAddress2String(ip));

  gateway = Ethernet.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(IpAddress2String(gateway));
  
  subnet = Ethernet.subnetMask();
  Serial.print("Subnet: ");
  Serial.println(IpAddress2String(subnet));
    
  Serial.print("Connecting to: ");
  Serial.println(server);
  Serial.print("Port: ");
  Serial.println(port);
 
  con2server();
  
  // Allow the hardware to sort itself out
  delay(500);
  digitalWrite(Pin_Buzzer,LOW);
  delay(1000);
  timer1.every(100, Timer_isr);
  
  //attachInterrupt(digitalPinToInterrupt(Pin_Sensor1),INT0_Isr,FALLING);
  attachInterrupt(digitalPinToInterrupt(Pin_Sensor2),INT1_Isr,RISING);
  
  wdt_enable(WDTO_8S);
  wdt_reset();
}

//================================================================
void loop()
{
  int strLength;
  wdt_reset();
  if (client.available()>0){
    buff = client.readStringUntil('\n');
    delay(1);
    wdt_reset();
    //Serial.println(Serial_data);
    strLength = buff.indexOf("$");
    if(strLength != -1){
      buff = buff.substring(0,strLength);
      processBuffer();
      wdt_reset();
      buff ="";
    }
    client.flush();
  }
  timer1.update();   
  Send_Detect();   
}
//================================================================

void Timer_isr(){
  if (!client.connected()) {
    if(Reconnect_count==0){
      con2server();
      Reconnect_count=5;
    }else{
      Reconnect_count--;
    }
    if(Count_Reset==0){
      Serial.println("Restart");
      wdt_enable(WDTO_2S);
      while(1);
    }else{
      Count_Reset--;
    }
  }else{
    Reconnect_count=0;
    Count_Reset=Timeout_Reset;
    if(Connect_Stat==false){
      Serial.println("Conected");
      Connect_Stat=true;  
      String temp = IpAddress2String(ip);
      temp += "#IR SENSOR ETHERNET#$";
      client.print(temp);  
    }
  }
  
  if(Sensor_Count>0){
    Sensor_Count--;
  }else{
    Sensor_0=0;
    Sensor_1=0;
  }
  
}

//---------------------------------------------------------------
void INT0_Isr(){
  if(Sensor_0==0){
    Sensor_0=1;
    Serial.println("IR_detect 0");
  //  if (!client.connected()) {
  //    client.publish("Server","IR_detect 0");
  //  }
  }
}

//---------------------------------------------------------------
void INT1_Isr(){
  if(Sensor_1==0){
    Sensor_1=1;
    Serial.println("IR_detect 1");
  //  if (!client.connected()) {
  //    client.publish("Server","IR_detect 1");
  //  }
  }
}

//---------------------------------------------------------------
void Send_Detect(){
  if(Sensor_0==1){
    if (client.connected()) {
      client.publish("Server","IR_detect 0");
    }
    Sensor_Count=Limit_Sensor;
    Sensor_0=2;
    digitalWrite(Pin_Buzzer,HIGH);
    delay(300);
    digitalWrite(Pin_Buzzer,LOW);
  }
  if(Sensor_1==1){
    if (client.connected()) {
      client.publish("Server","IR_detect 1");
    }
    Sensor_Count=Limit_Sensor;
    Sensor_1=2;
    digitalWrite(Pin_Buzzer,HIGH);
    delay(300);
    digitalWrite(Pin_Buzzer,LOW);
  }
}
