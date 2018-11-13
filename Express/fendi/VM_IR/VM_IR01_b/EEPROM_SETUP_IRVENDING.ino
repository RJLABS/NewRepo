//----------------------------------------------------
void write_string_eeprom(char *str, int a){
  for(int i = 0; i < strlen(str); i++){
    char x = str[i];
    EEPROM.write(a,x);
    a++;
    yield();
  }
}

//----------------------------------------------------
String read_string_eeprom(int start, int finish){
  String temp = "";
  for(start; start < finish; start++){
    char c;
    c = EEPROM.read(start);
    if(c != NULL){
      temp += c;
    }
  }
  return temp;
}

//----------------------------------------------------
void clear_eeprom(int start, int finish){
  for(start; start < finish; start++){
    EEPROM.write(start,'\0');
    yield();
  }
}

//----------------------------------------------------
void EEPROM_Setup(){
    clear_eeprom(0,100);
 /*
  //Just for write
  clear_eeprom(0,100);
  write_string_eeprom("192.168.1.10",addrDIP);
  write_string_eeprom("192.168.1.0",addrGIP);
  write_string_eeprom("255.255.255.0",addrNIP);
  write_string_eeprom("192.168.1.5",addrSIP);
  write_string_eeprom("6100",addrPORT);
*/
  
  ip = String2IpAddress(read_string_eeprom(addrDIP,addrDIP+15));
  gateway = String2IpAddress(read_string_eeprom(addrGIP,addrGIP+15));
  subnet = String2IpAddress(read_string_eeprom(addrNIP,addrNIP+15));
  server = String2IpAddress(read_string_eeprom(addrSIP,addrSIP+15));
  port = read_string_eeprom(addrPORT,addrPORT+5).toInt();
  /*
  Serial.print("Device IP : ");
  Serial.println(device_ip);
  Serial.print("Gateway   : ");
  Serial.println(gateway_ip);
  Serial.print("Subnet    : ");
  Serial.println(subnet_ip);
  
  Serial.print("Server IP   : ");
  Serial.println(server_ip);
  Serial.print("Server port : ");
  Serial.println(s_port);
  */
}

//----------------------------------------------------
IPAddress String2IpAddress(String Temp_IP){
  String temp;
  byte a,b=0;
  byte IP_temp[4];
  
  for(a=0;a<=Temp_IP.length();a++){
    if(Temp_IP[a]=='.'){
      IP_temp[b]=temp.toInt();
      temp="";
      b++;
    }else{
      temp +=Temp_IP[a];
    }
  }
  IP_temp[b]=temp.toInt();
  return IP_temp;
}

//----------------------------------------------------
String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ;
}
