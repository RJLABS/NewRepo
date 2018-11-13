void con2server(){
  if(!client.connected()){
    Serial.println("Reconnect Server 1");
    client.stop();
    Connect_Stat=false;
    client.connect(server,port);
  }
}
void conserver(){
  if(!client1.connected()){
    Serial.println("Reconnect Server 2");
    client1.stop();
    Connect_Stat1=false;
    client1.connect(server2,port);   
  }
}

//--------------------------------------------------------
void Respon_Client(String temp){
  String dat;
  if (client.connected()){
    dat = IpAddress2String(ip);
    dat+="#";
    dat+= temp;
    dat+= "#$";
    client.print(dat);
  }
}

void Respon2_Client(String temp){
  String dat;
  if (client1.connected()){
    dat = IpAddress2String(ip);
    dat+="#";
    dat+= temp;
    dat+= "#$";
    client1.print(dat);
  }
}
