void io(){
  pinMode(Pin_Reset_Smoke, OUTPUT);
  digitalWrite(Pin_Reset_Smoke, HIGH);
  pinMode(Pin_Buzzer,OUTPUT);
  pinMode(Pin_Relay_SSR, OUTPUT);
  digitalWrite(Pin_Relay_SSR, LOW);
}
