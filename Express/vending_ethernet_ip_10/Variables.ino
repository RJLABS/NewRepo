void RunMotor(int i){
  digitalWrite(motor[i].X,HIGH);
  digitalWrite(motor[i].Y,HIGH);
  if(reverse)
  {
    digitalWrite(RVS_PIN,HIGH);
  }
  else if(!reverse)
  {
    digitalWrite(RVS_PIN,LOW);
  }
  
  timeout = s_Timeout;
  Delay = s_Delay_Sensor;
}

void DeclareVariable()
{
  motor[1].X = A0;
  motor[1].Y = 3;
  motor[1].Magnetpin = 0;
  motor[1].MCPnum = 1;
  
  motor[2].X = A2;
  motor[2].Y = 3;
  motor[2].Magnetpin = 0;
  motor[2].MCPnum = 2;

  motor[3].X = A0;
  motor[3].Y = 4;
  motor[3].Magnetpin = 1;
  motor[3].MCPnum = 1;

  motor[4].X = A2;
  motor[4].Y = 4;
  motor[4].Magnetpin = 1;
  motor[4].MCPnum = 2;

  motor[5].X = A0;
  motor[5].Y = 2;
  motor[5].Magnetpin = 2;
  motor[5].MCPnum = 1;

  motor[6].X = A2;
  motor[6].Y = 2;
  motor[6].Magnetpin = 2;
  motor[6].MCPnum = 2;

  motor[7].X = A0;
  motor[7].Y = 5;
  motor[7].Magnetpin = 3;
  motor[7].MCPnum = 1;

  motor[8].X = A2;
  motor[8].Y = 5;
  motor[8].Magnetpin = 3;
  motor[8].MCPnum = 2;

  motor[9].X = A0;
  motor[9].Y = 8;
  motor[9].Magnetpin = 4;
  motor[9].MCPnum = 1;

  motor[10].X = A2;
  motor[10].Y = 8;
  motor[10].Magnetpin = 4;
  motor[10].MCPnum = 2;

  motor[11].X = A0;
  motor[11].Y = 6;
  motor[11].Magnetpin = 5;
  motor[11].MCPnum = 1;

  motor[12].X = A2;
  motor[12].Y = 6;
  motor[12].Magnetpin = 5;
  motor[12].MCPnum = 2;

  motor[13].X = A0;
  motor[13].Y = 7;
  motor[13].Magnetpin = 6;
  motor[13].MCPnum = 1;

  motor[14].X = A2;
  motor[14].Y = 7;
  motor[14].Magnetpin = 6;
  motor[14].MCPnum = 2;

  motor[15].X = A1;
  motor[15].Y = 3;
  motor[15].Magnetpin = 7;
  motor[15].MCPnum = 1;

  motor[16].X = A3;
  motor[16].Y = 3;
  motor[16].Magnetpin = 7;
  motor[16].MCPnum = 2;
  
  motor[17].X = A1;
  motor[17].Y = 4;
  motor[17].Magnetpin = 8;
  motor[17].MCPnum = 1;

  motor[18].X = A3;
  motor[18].Y = 4;
  motor[18].Magnetpin = 8;
  motor[18].MCPnum = 2;

  motor[19].X = A1;
  motor[19].Y = 2;
  motor[19].Magnetpin = 9;
  motor[19].MCPnum = 1;

  motor[20].X = A3;
  motor[20].Y = 2;
  motor[20].Magnetpin = 9;
  motor[20].MCPnum = 2;
 
  motor[21].X = A1;
  motor[21].Y = 5;
  motor[21].Magnetpin = 10;
  motor[21].MCPnum = 1;
  
  motor[22].X = A3;
  motor[22].Y = 5;
  motor[22].Magnetpin = 10;
  motor[22].MCPnum = 2;
  
  motor[23].X = A1;
  motor[23].Y = 8;
  motor[23].Magnetpin = 11;
  motor[23].MCPnum = 1;

  motor[24].X = A3;
  motor[24].Y = 8;
  motor[24].Magnetpin = 11;
  motor[24].MCPnum = 2;
  /*
  motor[25].X = A1;
  motor[25].Y = 6;
  motor[25].Magnetpin = 12;
  motor[25].MCPnum = 1;

  motor[26].X = A3;
  motor[26].Y = 6;
  motor[26].Magnetpin = 12;
  motor[26].MCPnum = 2;
  
  motor[27].X = A1;
  motor[27].Y = 7;
  motor[27].Magnetpin = 13;
  motor[27].MCPnum = 1;
  
  motor[28].X = A3;
  motor[28].Y = 7;
  motor[28].Magnetpin = 13;
  motor[28].MCPnum = 2;
  */
}
