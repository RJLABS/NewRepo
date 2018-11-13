
void con2server(){
  if(!client.connected()){
    Serial.println("Reconnect");
    client.stop();
    Connect_Stat=false;
    client.connect(server,port);
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
