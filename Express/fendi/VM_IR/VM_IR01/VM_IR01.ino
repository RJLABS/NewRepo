#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "Timer.h"

#define Pin_Relay 8
#define Pin_Buzzer 9
#define Pin_Sensor1 2 //logic AND, logic LOW jika kena obstacle (Falling)
#define Pin_Sensor2 3 //logic OR, logic HIGH jika kena obstacle (Rising)

//===========================================================
#define Limit_Sensor 1
//===========================================================

// Update these with values suitable for your network.
byte mac[]    = {  0xA1, 0xA2, 0xAA, 0xAA, 0xBB, 0xAC };
/*
IPAddress ip(192, 168, 1, 10);
IPAddress ip_dns(0, 0, 0, 0);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 1, 1);
IPAddress server(192, 168, 1, 5);
*/
IPAddress ip(10, 1, 26, 251);
IPAddress ip_dns(0, 0, 0, 0);
IPAddress subnet(255, 255, 248, 0);
IPAddress gateway(10, 1, 27, 254);
IPAddress server(10, 1, 26, 19);

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

EthernetClient ethClient;
PubSubClient client(ethClient);

//----------------------------------------------------------------------
void reconnect() {
  // Loop until we're reconnected
  if (!client.connected()) {
    Serial.print("MQTT => ");
    // Attempt to connect
    if (client.connect("VM_IR_0")) {
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

  pinMode(Pin_Sensor1,INPUT_PULLUP);
  pinMode(Pin_Sensor2,INPUT_PULLUP);
  pinMode(Pin_Relay,OUTPUT);
  pinMode(Pin_Buzzer,OUTPUT);
  digitalWrite(Pin_Relay,LOW);
  
  digitalWrite(Pin_Buzzer,HIGH);

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip, ip_dns, gateway, subnet);      //mac, ip, dns, gateway, subnet
//  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(500);
  digitalWrite(Pin_Buzzer,LOW);
  delay(1000);
  timer1.every(1000, Timer_isr);
  reconnect();
  attachInterrupt(digitalPinToInterrupt(Pin_Sensor1),INT0_Isr,FALLING);
//  attachInterrupt(digitalPinToInterrupt(Pin_Sensor2),INT1_Isr,RISING);
}

//================================================================
void loop()
{
  client.loop();
  timer1.update();   
  Send_Detect();   
}
//================================================================

void Timer_isr(){
  if (!client.connected()) {
    if(Reconnect_count==0){
      reconnect();
      Reconnect_count=5;
    }else{
      Reconnect_count--;
    }
  }else{
    Reconnect_count=0;
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

