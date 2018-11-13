#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <avr/wdt.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Thread.h>
#include <ThreadController.h>

ThreadController controll = ThreadController();

#define Pin_Reset_Smoke 8
#define Pin_Smoke_Detector 7
#define Pin_Buzzer 3
#define Pin_Relay_SSR 2
#define Pin_Sensor_Suhu 5
#define Pin_Arus_Control A1
#define Pin_Tegangan_Control A4
#define Pin_Arus_Motor A0
#define Pin_Tegangan_Motor A3

byte mac[] = {0x28, 0x88, 0x7D, 0xFF, 0xFF, 0xFD };

//--------------------------------------------------
int Timeout_Connect = 6;      //3 detik
int Timeout_Reset = 1000;      //5 menit
//--------------------------------------------------

IPAddress ip,subnet,gateway,server;
IPAddress ip_dns(0, 0, 0, 0);

int port;
int addrDIP = 1;
int addrGIP = 18;
int addrNIP = 35;
int addrSIP = 52;
int addrPORT = 69;

int addr_Suhu_limit = 105;
int addr_Arus_control_limit = 110;
int addr_Arus_motor_limit = 115;  
int addr_control_limit_min = 120;
int addr_motor_limit_min = 125;
int addr_control_limit_max = 130;
int addr_motor_limit_max = 135;

float Suhu_limit;
float Arus_control_limit;
float Arus_motor_limit;
float motor_limit_min;
float control_limit_min;
float motor_limit_max;
float control_limit_max;

bool Connect_Stat=false;
bool Reset = false;
byte Count_Reset=0;
byte Reconnect_count=0;
String buff,task;

//My Thread
Thread Thread1 = Thread();
//His Thread
Thread Thread2 = Thread();
Thread Thread3 = Thread();

EthernetClient client;
OneWire oneWire(Pin_Sensor_Suhu);
DallasTemperature sensors(&oneWire);
DeviceAddress SensorSuhu;

int streamevent;
int value;
float arusmotor,aruscontrol,teganganmotor,tegangancontrol;
float Suhu;
int period = 500;

bool Buzzer_temp=false;
bool DataStream = false;
bool Fire_flag=false;
bool Exec_flag=true;
bool Buzzer_flag=true;
bool Relay_state=false;
bool Reset_fire_flag=false;

float min_Adc_Arus_5A = 333; //327.7;
float max_Adc_Arus_5A = 696.3;
float min_Adc_Arus_20A = 125;
float max_Adc_Arus_20A = 900.24;
float Power_Factor = 0.01545;         //0.015054;
uint8_t Run_Count_Read_adc=0;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  sensors.begin();
  if (!sensors.getAddress(SensorSuhu, 0))  {
    Serial.println("Tidak Terkoneksi dengan Sensor Suhu");
  }
  
  Run_Count_Read_adc=8;   //skip 4 detik di awal supaya tegangan stabil
  
  io();
  
  delay(1000);
  EEPROM_Setup();
  //Restore_default();
  Read_EEPROM();
  delay(2000);
  Ethernet.begin(mac, ip, ip_dns, gateway, subnet);
  delay(300);
  
  /*
  // device identification
  Serial.println("Device information");
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
  */  
  
  ip = Ethernet.localIP();
  //Serial.print("IP: ");
  //Serial.println(IpAddress2String(ip));

  gateway = Ethernet.gatewayIP();
  //Serial.print("Gateway: ");
  //Serial.println(IpAddress2String(gateway));
  
  subnet = Ethernet.subnetMask();
  //Serial.print("Subnet: ");
  //Serial.println(IpAddress2String(subnet));
    
  //Serial.print("Connecting to: ");
  //Serial.println(server);
  //Serial.print("Port: ");
  //Serial.println(port);
  /*
  Serial.print("Limit Suhu = ");
  Serial.println(Suhu_limit);
  
  Serial.print("Arus control limit = ");
  Serial.println(Arus_control_limit);

  Serial.print("Arus motor limit = ");
  Serial.println(Arus_motor_limit);

  Serial.print("Tegangan control min = ");
  Serial.println(control_limit_min);

  Serial.print("Tegangan control max = ");
  Serial.println(control_limit_max);

  Serial.print("Tegangan motor min = ");
  Serial.println(motor_limit_min);

  Serial.print("Tegangan motor max = ");
  Serial.println(motor_limit_max);
  */
  con2server();
  
  // Allow the hardware to sort itself out
  digitalWrite(Pin_Buzzer,HIGH);
  delay(500);
  digitalWrite(Pin_Buzzer,LOW);
  delay(1000);
  Thread1.onRun(StreamData);
  Thread1.setInterval(period);
  Thread2.onRun(Get_temp);
  Thread2.setInterval(2000);
  Thread3.onRun(Timer_isr);
  Thread3.setInterval(500);
  
  // Adds both threads to the controller
  controll.add(&Thread1); // & to pass the pointer to it
  controll.add(&Thread2);
  controll.add(&Thread3);
  
  sensors.setResolution(SensorSuhu, 12);
  
  Count_Reset=Timeout_Reset;
  wdt_enable(WDTO_8S);
  wdt_reset();
}

void loop() {
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
  controll.run();
}

void StreamData(){
  if(DataStream){
    if(Buzzer_temp == false){
      String dat = "IM=";
      int arus = arusmotor * 1000;
      dat += arus;
      dat += "&";
      Respon_Client(dat);
    }
  }
}
void Timer_isr(){
  if (!client.connected()) {
    if(Reconnect_count==0){
      con2server();
      Reconnect_count=Timeout_Connect;
    }
    else{
      Reconnect_count--;
    }
    if(Count_Reset==0){
      Serial.println("Restart");
      wdt_enable(WDTO_2S);
      while(1);
    }
    else{
      Count_Reset--;
    }
  }
  else{
    Reconnect_count=0;
    Count_Reset=Timeout_Reset;
    if(Connect_Stat==false){
      digitalWrite(Pin_Buzzer,HIGH);
      delay(300);
      digitalWrite(Pin_Buzzer,LOW);
      delay(20);
      digitalWrite(Pin_Buzzer,HIGH);
      delay(300);
      digitalWrite(Pin_Buzzer,LOW);
      Serial.println("Connected");
      Connect_Stat=true;  
      String temp = IpAddress2String(ip);
      temp += "#Voltmeter Vending#$";
      client.print(temp);  
    }
  }
  if(Run_Count_Read_adc==0){
    Read_ADC();
    Read_Fire();
    Execution();
  }else if(Run_Count_Read_adc==1){
    for(uint8_t a=0;a<10;a++){
      Read_ADC();
      delay(50);
    }
    Run_Count_Read_adc--;
  }else{
    Run_Count_Read_adc--;
  }
  Cek_timeout_relay();
}

void Read_Fire(){
  if (digitalRead(Pin_Smoke_Detector)==HIGH){
    Fire_flag=true;
  }else{
    Fire_flag=false;
  }
}

//=================================================================================
void Get_temp(){
  byte a =0;
  bool state=false;

  sensors.requestTemperatures();
  Suhu = sensors.getTempC(SensorSuhu);
  
  if (Suhu == 85.0 || Suhu == -127.0 ){
    Suhu=0;
  }
}

//****************************************************************************************************
void Cek_timeout_relay(){
  if(Reset){
    Relay_state=false;
    digitalWrite(Pin_Relay_SSR,LOW);
    if(Fire_flag==true){
      digitalWrite(Pin_Reset_Smoke,LOW);
      delay(1000);
      digitalWrite(Pin_Reset_Smoke,HIGH);
      Reset_fire_flag=false;
    } 
    if(client.connected()) {
      Respon_Client("STANDBY");
    }        
   Reset = false;
  }
}
