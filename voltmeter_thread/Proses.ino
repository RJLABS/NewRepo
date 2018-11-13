void processBuffer(){
  String temp_DIP,temp_GIP,temp_NIP,temp_HOST,temp_SSID,temp_PASS,temp_PORT,temp_SUHU,temp_KELEM,temp_PRES,task2;
  String temp_time,dat;
  int m00,m0,m11,m12,m21,m22,m31,m32,m41,m42;
  char Temp[20];
  memset(Temp,'\n',sizeof(Temp));

  m00 = buff.indexOf(':'); //tampung IP
  task2 = buff.substring(0,m00);

  Serial.println(task);
  
  m0 = buff.indexOf(':',m00+1);
  task = buff.substring(m00+1,m0);
  if (task == "CS"){
    m12 = buff.indexOf(':',m0+1);
    String temp = buff.substring(m0+1,m12);
    if(temp == "") {
      String Resp = "CS=";
      Resp += Suhu_limit;
      Respon_Client(Resp);
    }
    else{
      Suhu_limit = temp.toFloat();
      EEPROM.put( addr_Suhu_limit,Suhu_limit );
      Respon_Client("CS_OK");
    }
  }
  else if (task == "CIC"){
    m12 = buff.indexOf(':',m0+1);
    String temp = buff.substring(m0+1,m12);
    if(temp == "") {
      String Resp = "CIC=";
      Resp += Arus_control_limit;
      Respon_Client(Resp);
    }
    else {
      Arus_control_limit = temp.toFloat();
      EEPROM.put( addr_Arus_control_limit,Arus_control_limit );
      Respon_Client("CIC_OK");
    }
  }   
  else if (task == "CIM"){
    m12 = buff.indexOf(':',m0+1);
    String temp = buff.substring(m0+1,m12);
    if(temp == "") {
      String Resp = "CIM=";
      Resp += Arus_motor_limit;
      Respon_Client(Resp);
    }
    else {
      Arus_motor_limit = temp.toFloat();
      EEPROM.put( addr_Arus_motor_limit,Arus_motor_limit );
      Respon_Client("CIM_OK");
    }
  }
  else if (task == "CVMINM"){
    m12 = buff.indexOf(':',m0+1);
    String temp = buff.substring(m0+1,m12);
    if(temp == "") {
      String Resp = "CVMINM=";
      Resp += motor_limit_min;
      Respon_Client(Resp);
    }
    else {
      motor_limit_min = temp.toFloat();
      EEPROM.put( addr_motor_limit_min,motor_limit_min );
      Respon_Client("CVMINM_OK");
    }
  }              
  else if (task == "CVMAXM"){
    m12 = buff.indexOf(':',m0+1);
    String temp = buff.substring(m0+1,m12);
    if(temp == "") {
      String Resp = "CVMAXM=";
      Resp += motor_limit_max;
      Respon_Client(Resp);
    }
    else {
      motor_limit_max = temp.toFloat();
      EEPROM.put( addr_motor_limit_max,motor_limit_max );
      Respon_Client("CVMAXM_OK");
    }
  } 
  else if (task == "CVMINC"){
    m12 = buff.indexOf(':',m0+1);
    String temp = buff.substring(m0+1,m12);
    if(temp == "") {
      String Resp = "CVMINC=";
      Resp += control_limit_min;
      Respon_Client(Resp);
    }
    else {
      control_limit_min = temp.toFloat();
      EEPROM.put( addr_control_limit_min,control_limit_min );
      Respon_Client("CVMINC_OK");
    }
  }  
  else if (task == "CVMAXC"){
    m12 = buff.indexOf(':',m0+1);
    String temp = buff.substring(m0+1,m12);
    if(temp == "") {
      String Resp = "CVMAXC=";
      Resp += control_limit_max;
      Respon_Client(Resp);
    }
    else {
      control_limit_max = temp.toFloat();
      EEPROM.put( addr_control_limit_max,control_limit_max );
      Respon_Client("CVMAXC_OK");
    }
  }
  else if(task == "RESET")
  {
    if(Relay_state) {
      Reset = true;
      Respon_Client("RESET_OK");
      Serial.println("Restart");
      wdt_enable(WDTO_2S);
      while(1);
    }
  }
  else if(task == "STREAM")
  {
    m12 = buff.indexOf(':',m0+1);
    String temp = buff.substring(m0+1,m12);
    if(temp == "ON"){
      DataStream = true;
      Respon_Client("SON_OK");
    }
    else if(temp == "OFF"){
      DataStream = false;
      Respon_Client("SOFF_OK");
    }
  }
  else if(task == "CP")
  {
    m12 = buff.indexOf(':',m0+1);
    String temp = buff.substring(m0+1,m12);
    period = temp.toInt();
    Serial.println(period);
    Thread1.setInterval(period);
    Thread2.setInterval(2000);
    Thread3.setInterval(500);
    controll.run();
    Respon_Client("CP_OK");
  }
  else if(task == "EXEC")
  {
    m12 = buff.indexOf(':',m0+1);
    String temp = buff.substring(m0+1,m12);
    if(temp == "ON"){
      Exec_flag = true;
    }
    else if(temp == "OFF"){
      Exec_flag = false;
    }
    Respon_Client("EXEC_OK");
  }
  else if(task == "BUZZER")
  {
    m12 = buff.indexOf(':',m0+1);
    String temp = buff.substring(m0+1,m12);
    if(temp == "ON"){
      Buzzer_flag = true;
    }
    else if(temp == "OFF"){
      Buzzer_flag = false;
    }
    Respon_Client("BUZZER_OK");
  }
  else if(task == "SUHU")
  {
    String SSuhu = "SH=";
    SSuhu += String(Suhu);
    SSuhu += "&";
    Respon_Client(SSuhu);
  }
  else if(task == "VMOT")
  {
    String SSuhu = "VMOT=";
    SSuhu += String(teganganmotor);
    SSuhu += "&";
    Respon_Client(SSuhu);
  }
  else if(task == "VCON")
  {
    String SSuhu = "VCON=";
    SSuhu += String(tegangancontrol);
    SSuhu += "&";
    Respon_Client(SSuhu);
  }
  else if(task == "ICON")
  {
    String SSuhu = "ICON=";
    int arus = aruscontrol * 1000;
    SSuhu += String(arus);
    SSuhu += "&";
    Respon_Client(SSuhu);
  }
  else if(task == "SAS")
  {
    String SSuhu = "SAS=";
    int hasil;
    if (digitalRead(Pin_Smoke_Detector)==HIGH){
      hasil = 1;
    }
    else{
      hasil = 0;
    }
    SSuhu += String(hasil);
    SSuhu += "&";
    Respon_Client(SSuhu);
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
