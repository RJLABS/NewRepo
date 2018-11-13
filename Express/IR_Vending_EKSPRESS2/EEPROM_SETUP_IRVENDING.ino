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
 /*
 
  //Just for write
  clear_eeprom(0,100);
  write_string_eeprom("192.168.1.11",addrDIP); //10.1.37.139 punya reza
  write_string_eeprom("192.168.1.1",addrGIP);
  write_string_eeprom("255.255.255.0",addrNIP);
  write_string_eeprom("192.168.1.5",addrSIP);
  write_string_eeprom("192.168.1.6",addrSIP2);
  write_string_eeprom("6017",addrPORT);
 */
 /*
  //Just for write
  clear_eeprom(0,100);
  write_string_eeprom("10.1.37.139",addrDIP); //10.1.37.139 punya reza
  write_string_eeprom("10.1.36.254",addrGIP);
  write_string_eeprom("255.255.252.0",addrNIP);
  write_string_eeprom("10.1.37.14",addrSIP);
  write_string_eeprom("10.1.37.14",addrSIP2);
  write_string_eeprom("6017",addrPORT);
*/
  
  ip = String2IpAddress(read_string_eeprom(addrDIP,addrDIP+15));
  gateway = String2IpAddress(read_string_eeprom(addrGIP,addrGIP+15));
  subnet = String2IpAddress(read_string_eeprom(addrNIP,addrNIP+15));
  server = String2IpAddress(read_string_eeprom(addrSIP,addrSIP+15));
  server2 = String2IpAddress(read_string_eeprom(addrSIP2,addrSIP2+15));
  port = read_string_eeprom(addrPORT,addrPORT+5).toInt();
  
  Serial.print("Device IP : ");
  Serial.println(ip);
  Serial.print("Gateway   : ");
  Serial.println(gateway);
  Serial.print("Subnet    : ");
  Serial.println(subnet);
  
  Serial.print("Server IP 1  : ");
  Serial.println(server);
  Serial.print("Server IP 2  : ");
  Serial.println(server2);
  Serial.print("Server port : ");
  Serial.println(port);
  
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
