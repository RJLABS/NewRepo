void processBuffer(){
  
  String temp_DIP,temp_GIP,temp_NIP,temp_HOST,temp_SSID,temp_PASS,temp_PORT,temp_SUHU,temp_KELEM,temp_PRES,task2;
  String temp_time,dat;
  //int m0,m12,m21,m22,m31,m32,m41,m42;
  int m00,m0,m11,m12,m21,m22,m31,m32,m41,m42;
  char Temp[20];
  memset(Temp,'\n',sizeof(Temp));

  m00 = buff.indexOf(':'); //tampung IP
  task2 = buff.substring(0,m00);
  
  m0 = buff.indexOf(':',m00+1);
  task = buff.substring(m00+1,m0);
  
  //m0 = buff.indexOf('=');
  //task = buff.substring(0,m0);
  //m00 = buff.indexOf(':'); //tampung IP
  //task2 = buff.substring(0,m00);
  
   //m0 = buff.indexOf(': ',m00+1);
  //task = buff.substring(m00+1,m0);

   //m0 = buff.indexOf('#');
   //task = buff.substring(0,m0);

  //Serial.print("task = ");
  
  Serial.println(task);
  if (task == "TC"){
    m12 = buff.indexOf(':',m0+1);
    String temp = buff.substring(m0+1,m12);
    valuedelay = temp.toInt();
    //Serial.println(temp);
    //Serial.println(valuedelay);
    Respon_Client("TC_OK");
  }
  if (task == "PC"){
    m12 = buff.indexOf(':',m0+1);
    String temp = buff.substring(m0+1,m12);
    timerdelay = temp.toInt();
    myThread.setInterval(timerdelay);
    //Serial.println(temp);
    //Serial.println(valuedelay);
    Respon_Client("PC_OK");
  }
  if (task == "IRON"){
    relay = true;
    Sensor_0=0;      
    Sensor_1=0;      
    Respon_Client("IRON_OK");
    digitalWrite(Pin_Relay,HIGH);  
   }
  else if(task == "IROFF"){
    relay = false;        
    Sensor_0=0;      
    Sensor_1=0;
    Respon_Client("IROFF_OK");
    digitalWrite(Pin_Relay,LOW);
  }
  
//====================================================  
  else if(task == "C1"){
    m12 = buff.indexOf('~',m0+1);
    m21 = buff.indexOf('=',m12+1);
    m22 = buff.indexOf('~',m21+1);
    m31 = buff.indexOf('=',m22+1);
    m32 = buff.indexOf('~',m31+1);
    m41 = buff.indexOf('=',m32+1);
    m42 = buff.indexOf('~',m41+1);

    temp_HOST = buff.substring(m0+1,m12);
    temp_SSID = buff.substring(m21+1,m22);
    temp_PASS = buff.substring(m31+1,m32);
    temp_PORT = buff.substring(m41+1,m42);

    //change server ip
    clear_eeprom(addrSIP,addrSIP+16);
    memset(Temp,'\n',sizeof(Temp));
    temp_HOST.toCharArray(Temp,16);
    write_string_eeprom(Temp, addrSIP);
      
    //change port
    clear_eeprom(addrPORT,addrPORT+5);
    memset(Temp,'\n',sizeof(Temp));
    temp_PORT.toCharArray(Temp,6);
    write_string_eeprom(Temp, addrPORT);
    
    Respon_Client("|IP changed");    
  }
  
  else if (task == "C2"){
    m12 = buff.indexOf('~',m0+1);
    m21 = buff.indexOf('=',m12+1);
    m22 = buff.indexOf('~',m21+1);
    m31 = buff.indexOf('=',m22+1);
    m32 = buff.indexOf('~',m31+1);
    
    temp_DIP = buff.substring(m0+1,m12);
    temp_NIP = buff.substring(m21+1,m22);
    temp_GIP = buff.substring(m31+1,m32);
    
    //change device IP
    clear_eeprom(addrDIP,addrDIP+16);
    memset(Temp,'\n',sizeof(Temp));
    temp_DIP.toCharArray(Temp,16);
    write_string_eeprom(Temp, addrDIP);
    
    //change subnet
    clear_eeprom(addrNIP,addrNIP+16);
    memset(Temp,'\n',sizeof(Temp));
    temp_NIP.toCharArray(Temp,16);
    write_string_eeprom(Temp, addrNIP);
    
    //change gateway
    clear_eeprom(addrGIP,addrGIP+16);
    memset(Temp,'\n',sizeof(Temp));
    temp_GIP.toCharArray(Temp,16);
    write_string_eeprom(Temp, addrGIP);
    //EEPROM.commit();
    
    Respon_Client("|IP changed");    
  }
}
