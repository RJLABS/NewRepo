void processBuffer(){
  String temp_DIP,temp_GIP,temp_NIP,temp_HOST,temp_SSID,temp_PASS,temp_PORT,temp_TO;
  String buf,Task;
  int m00,m0,m11,m12,m21,m22,m31,m32,m41,m42;
  uint8_t a,b;
  char Temp[20];
  memset(Temp,'\n',sizeof(Temp));
  
//------------------------------------------
  m00 = buff.indexOf(':'); //tampung IP //a
  //task2 = buff.substring(0,m00);   

  m0 = buff.indexOf(':',m00+1);   //b
  Task = buff.substring(m00+1,m0);
  //pub("Topic/Reply/"+id,"TASK="+Task);
//------------------------------------------
 if(Task == "ME"){
    String waitS;
    a = buff.indexOf(':',m0+1);
    buf = buff.substring(m0+1,a);
    b=buff.indexOf('$',a+1);
    waitS=buff.substring(a+1,b);
    wait = waitS.toInt();
    num = buf.toInt();
    //Serial.println(num);
    //Serial.println(wait);
    if(num>NUM_MOTORS)
    {
      return;
    }
    //Serial.println(num);
    //pub("Topic/Reply/"+id,"MTR = "+buf+" Delay = "+waitS+" ms");
    Run = true;
    Runme = true;
    reverse = false;
    RunMotor(num);
 }

  else if(Task == "MD"){
    String waitS;
    a = buff.indexOf(':',m0+1);
    buf = buff.substring(m0+1,a);
    b=buff.indexOf('$',a+1);
    waitS=buff.substring(a+1,b);
    uint16_t wait =waitS.toInt();
    num = buf.toInt();
    delayMotorPrint = wait;
 }  

  else if(Task == "BS"){
    String waitS;
    a = buff.indexOf(':',m0+1);
    buf = buff.substring(m0+1,a);
    b=buff.indexOf('$',a+1);
    waitS=buff.substring(a+1,b);
    uint16_t wait =waitS.toInt();
    num = buf.toInt();
    s_Delay_Sensor = wait;
 } 

  else if(Task == "RE"){
    String waitS;
    a = buff.indexOf(':',m0+1);
    buf = buff.substring(m0+1,a);
    b=buff.indexOf('$',a+1);
    waitS=buff.substring(a+1,b);
    wait =waitS.toInt();
    num = buf.toInt();
    //Serial.println(num);
    //Serial.println(wait);
    if(num>NUM_MOTORS)
    {
      return;
    }
    //Serial.println(num);
    //pub("Topic/Reply/"+id,"MTR = "+buf+" Delay = "+waitS+" ms");
    Run = true;
    Runme = true;
    reverse = true;
    RunMotor(num);
 }
 /*
 else if(Task == "PING" && !pubOK){
    pubOK=true;
 }*/
 else if(Task == "FM")
  { 
    a = buff.indexOf('$');
    buf = buff.substring(m0+1,a);
    num = buf.toInt();
    if(num>NUM_MOTORS)
    {
      return;
    }
    //Serial.println(num);
    Run = true;
    reverse = false;
    RunMotor(num);
  }

  else if(Task == "CS")
  { 
    a = buff.indexOf('$');
    buf = buff.substring(m0+1,a);
    num = buf.toInt();
    if(num>NUM_MOTORS)
    {
      return;
    }
     //   printfeedback('~' +String(mcp.digitalRead(motor[num].Magnetpin)));
    if(motor[num].MCPnum == 1){
        printfeedback('~' +String(mcp.digitalRead(motor[num].Magnetpin)));
      }
    else if(motor[num].MCPnum == 2){
        printfeedback('~' +String(mcp1.digitalRead(motor[num].Magnetpin)));
      }

/*
    if (mcp.digitalRead(motor[num].Magnetpin) == 0)
    {
      //True
      printfeedback("#0#");
    }
    else
    {
      //False
      printfeedback("#1#");
    }
  */
    
  }
  
  else if(Task == "RM" )
  {
    a = buff.indexOf('$');
    buf = buff.substring(m0+1,a);
    num = buf.toInt();
    if(num>NUM_MOTORS)
    {
      return;
    }
    //Serial.println(num);
    Run = true;
    reverse = true;
    RunMotor(num);
  }
  else if(Task == "FON")
  {
    a = buff.indexOf('$');
    buf = buff.substring(m0+1,a);
    num = buf.toInt();
    if(num>NUM_MOTORS)
    {
      return;
    }
    digitalWrite(motor[num].X,HIGH);
    digitalWrite(motor[num].Y,HIGH);
    digitalWrite(RVS_PIN,LOW);
    //pubON=true;
  }
  else if(Task == "FOFF")
  {
    a = buff.indexOf('$');
    buf = buff.substring(m0+1,a);
    num = buf.toInt();
    if(num>NUM_MOTORS)
    {
      return;
    }
    digitalWrite(motor[num].X,LOW);
    digitalWrite(motor[num].Y,LOW);
    digitalWrite(RVS_PIN,LOW);
    //pubOFF=true;
  }
  else if(Task == "RON")
  {
    a = buff.indexOf('$');
    buf = buff.substring(m0+1,a);
    num = buf.toInt();
    if(num>NUM_MOTORS)
    {
      return;
    }
    digitalWrite(motor[num].X,HIGH);
    digitalWrite(motor[num].Y,HIGH);
    digitalWrite(RVS_PIN,HIGH);
    //pubON=true;
  }
  else if(Task == "ROFF")
  {
    a = buff.indexOf('$');
    buf = buff.substring(m0+1,a);
    num = buf.toInt();
    if(num>NUM_MOTORS)
    {
      return;
    }
    digitalWrite(motor[num].X,LOW);
    digitalWrite(motor[num].Y,LOW);
    digitalWrite(RVS_PIN,HIGH);
    //pubOFF=true;
  }
/*
//------------TO-------------
//10.1.27.97:C5:60~$
else if(Task == "C1"){
  conf=true;
  m12 = buff.indexOf('~',m0+1); 
  temp_TO = buff.substring(m0+1,m12);
  
  //change TO
  clear_eeprom(addrTO,addrTO+4);
  memset(Temp,'\n',sizeof(Temp));
  temp_TO.toCharArray(Temp,4);
  write_string_eeprom(Temp,addrTO);
  delay(100); 
  conf2=true;
}
//----------------SERVER IP-----------------------------
//10.1.27.97:C3:10.1.39.110~$
else if(Task == "C2"){
  conf=true;
  m12 = buff.indexOf('~',m0+1); 
  temp_HOST = buff.substring(m0+1,m12);
  
  //change server ip
  clear_eeprom(addrSIP,addrSIP+16);
  memset(Temp,'\n',sizeof(Temp));
  temp_HOST.toCharArray(Temp,16);
  write_string_eeprom(Temp, addrSIP);
  delay(100); 
  conf2=true;
}
//---------------port + mac + gateway------------------
//10.1.27.97:C4:6011~MAC=AB:AB:AB:AB:AB:AC~G=192.168.1.1~$
else if(Task == "C3"){
  conf=true;
  m12 = buff.indexOf('~',m0+1);
  m21 = buff.indexOf('=',m12+1);
  m22 = buff.indexOf('~',m21+1);
  m31 = buff.indexOf('=',m22+1);
  m32 = buff.indexOf('~',m31+1);
  
  temp_PORT = buff.substring(m0+1,m12);
  temp_PASS = buff.substring(m21+1,m22);
  temp_GIP = buff.substring(m31+1,m32);
  
  //change port
  clear_eeprom(addrPORT,addrPORT+5);
  memset(Temp,'\n',sizeof(Temp));
  temp_PORT.toCharArray(Temp,6);
  write_string_eeprom(Temp, addrPORT);
  
  //mac addrr
  clear_eeprom(addrMAC,addrMAC+19);
  memset(Temp,'\n',sizeof(Temp));
  temp_PASS.toCharArray(Temp,19);
  write_string_eeprom(Temp, addrMAC);

  //change gateway
  clear_eeprom(addrGIP,addrGIP+16);
  memset(Temp,'\n',sizeof(Temp));
  temp_GIP.toCharArray(Temp,16);
  write_string_eeprom(Temp, addrGIP);
  delay(100); 
  conf2=true;  
}

//---------------------ip+netmask+server----------------------
//192.168.1.5:C2:192.168.1.10~N=255.255.255.0~S=192.168.1.1~$  
else if (Task == "C4"){
    conf=true;
    m12 = buff.indexOf('~',m0+1);
    m21 = buff.indexOf('=',m12+1);
    m22 = buff.indexOf('~',m21+1);
    m31 = buff.indexOf('=',m22+1);
    m32 = buff.indexOf('~',m31+1);
    
    temp_DIP = buff.substring(m0+1,m12);
    temp_NIP = buff.substring(m21+1,m22);
    temp_HOST = buff.substring(m31+1,m32);

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
    
    //change server ip
    clear_eeprom(addrSIP,addrSIP+16);
    memset(Temp,'\n',sizeof(Temp));
    temp_HOST.toCharArray(Temp,16);
    write_string_eeprom(Temp, addrSIP);
    delay(100); 
    conf2=true;  
  }*/
}
