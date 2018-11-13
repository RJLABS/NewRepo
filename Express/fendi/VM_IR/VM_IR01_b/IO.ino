void io(){
  pinMode(Pin_Sensor1,INPUT_PULLUP);
  pinMode(Pin_Sensor2,INPUT_PULLUP);
  pinMode(Pin_Relay,OUTPUT);
  pinMode(Pin_Buzzer,OUTPUT);
  digitalWrite(Pin_Relay,LOW);
  
  digitalWrite(Pin_Buzzer,HIGH);
}
