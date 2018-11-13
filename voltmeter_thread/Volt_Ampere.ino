void Read_ADC(){
  int a;
  float f;
  
  a = analogRead(Pin_Arus_Control);
  f = fmap(a, min_Adc_Arus_5A, max_Adc_Arus_5A, -5.0, 5.0);    
  aruscontrol = (aruscontrol + f)/2;

  a = analogRead(Pin_Arus_Motor);
  f = fmap(a, min_Adc_Arus_5A, max_Adc_Arus_5A, -5.0, 5.0);     
  arusmotor = (arusmotor + f)/2;

  a = analogRead(Pin_Tegangan_Control);
  f = a * Power_Factor;
  tegangancontrol = (tegangancontrol + f)/2;

  a = analogRead(Pin_Tegangan_Motor);
  f = a * Power_Factor;
  teganganmotor = (teganganmotor + f)/2;
  /*
  Serial.print("Tegangan Control = ");
  Serial.println(tegangancontrol);
  Serial.print("Tegangan Motor = ");
  Serial.println(teganganmotor);
  Serial.print("Arus Control = ");
  Serial.println(aruscontrol);
  Serial.print("Arus Motor = ");
  Serial.println(arusmotor);
  */
}

//----------------------------------------------------------------------------
float fmap (float sensorValue, float sensorMin, float sensorMax, float outMin, float outMax)
    {
  return (sensorValue - sensorMin) * (outMax - outMin) / (sensorMax - sensorMin) + outMin;
     }

