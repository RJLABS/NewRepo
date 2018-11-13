#include <Ethernet.h>
#include <SPI.h>
#include <avr/wdt.h>

//#include <EEPROM.h>

#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <avr/io.h>
#include <stdint.h>
#include <Timer.h>
#include <TimerOne.h>
Timer timer0;

//=========ethernet======================
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192,168,1,10);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
IPAddress ip_dns(0,0,0,0);
IPAddress server(192,168,1,5);
EthernetClient client;
int port = 6017;
//========================================

bool isr=false;
bool Run=false;
int num;

int timeout; 
int Delay;
bool reverse = true;
//--------------------------------------------------
int Timeout_Connect = 60;      //3 detik
int Timeout_Reset = 6000;      //5 menit

int s_Delay_Sensor = 6;
int s_Timeout = 60;
//--------------------------------------------------
bool Connect_Stat=false;
int Count_Reset=0;
int Count_Connect=0;
int Count_Suhu=0;
String buff,temp,Task;
//-------------------------------------------------
uint16_t  wait = 0;
bool Runme = false;
uint16_t delayMotorPrint = 1;

struct motors{
  int X;
  int Y;
  int Magnetpin;
  int MCPnum;
  String report;
};

Adafruit_MCP23017 mcp;
Adafruit_MCP23017 mcp1;

#define RVS_PIN 9
#define NUM_MOTORS 24

volatile motors motor[NUM_MOTORS+1];

void printfeedback(String feedback){
    temp = IpAddress2String(ip);
    temp+=":";
    temp+=Task;
    temp+=num;
    temp+=feedback;
    temp+="$";
    client.print(temp);
}

void Timer_exec(){
  if(timeout >0){
    timeout--;
  }
  /*
  if(wait > 0 && Runme == true)
  {
    wait = wait - 50;
  }
  if(wait == 0 && Runme == true)
  {
    Runme = false;
    Serial.println("ok");
    if(reverse)
    {
      digitalWrite(RVS_PIN,HIGH);
    }
    else if(!reverse)
    {
      digitalWrite(RVS_PIN,LOW);
    }
    delay(5);
    digitalWrite(motor[num].X,LOW);
    digitalWrite(motor[num].Y,LOW);
    timeout = 0;
  }
  */
  if(Delay > 0){
    Delay--;
  }
  if(!client.connected()){
    if(Count_Connect==0){
      Count_Connect=Timeout_Connect;
      con2server();
    }else{
      Count_Connect--;
    }

    if(Count_Reset==0){
      Serial.println("Restart");
      wdt_enable(WDTO_2S);
      while(1);
    }else{
      Count_Reset--;
    }
  }
  else{
    Count_Connect=0;
    Count_Reset=Timeout_Reset;
    if(Connect_Stat==false){
      Serial.println("Connected");
      Connect_Stat=true;  
      temp = IpAddress2String(ip);
      temp += "#vending_#$";
      client.print(temp);  
    }
  }
}

//----------------------------------------------------
String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ;
}

void cek_timeout(){
  if(timeout == 0 && Run == true){
    Run = false;
    digitalWrite(motor[num].X,LOW);
    digitalWrite(motor[num].Y,LOW);
    //Serial.print("MOTOR ");
    //Serial.print(num);
    //Serial.println(" Timeout");
    //printfeedback("#TO#");
    //printfeedback("#SE#");
    printfeedback("~0");
  }
}

void timerIsr(){
  isr=true;

  if(wait > 0 && Runme == true)
  {
    wait = wait -1;
  }
  if(wait == 0 && Runme == true)
  {
    Run = false;
    Runme = false;
    Serial.println("ok");
    if(reverse)
    {
      digitalWrite(RVS_PIN,HIGH);
    }
    else if(!reverse)
    {
      digitalWrite(RVS_PIN,LOW);
    }
    delay(5);
    digitalWrite(motor[num].X,LOW);
    digitalWrite(motor[num].Y,LOW);
    timeout = 0;
  }


}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  InitializeIO();
  DeclareVariable();
  Ethernet.begin(mac, ip, ip_dns, gateway, subnet);
  delay(1000);
  //=================================================
  // device identification
  Serial.println("Device information");
  Serial.print("Mac Address : ");
  
  Serial.print(mac[0],HEX);
  Serial.print(":");
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.println(mac[5],HEX);
  
  ip = Ethernet.localIP();
  Serial.print("Device IP : ");
  Serial.println(ip);
  gateway = Ethernet.gatewayIP();
  Serial.print("Gateway   : ");
  Serial.println(gateway);
  subnet = Ethernet.subnetMask();
  Serial.print("Subnet    : ");
  Serial.println(subnet);
  Serial.println();
  Serial.print("Connecting to : ");
  Serial.println(server);
  Serial.print("Port : ");
  Serial.println(port);
  //===================================================
  
  con2server();
  
  mux();
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
  timer0.every(50, Timer_exec);
  
  Count_Reset=Timeout_Reset;
  wdt_enable(WDTO_8S);
  wdt_reset();
}

void loop() {
  // put your main code here, to run repeatedly:
  wdt_reset();
  int strLength;
  if (client.available()>0){
    wdt_reset();
    buff = client.readStringUntil('\n');
    strLength = buff.indexOf("$");
    //Serial.println(strLength);
    if (strLength != -1){
      Serial.println(buff);
      buff = buff.substring(0,strLength);
      processBuffer();
      wdt_reset();
      buff ="";
    }
    client.flush();
  }
  //cek_timeout();
  if(isr){
    i2c_master();
    isr=false;
  }
  timer0.update();
}
