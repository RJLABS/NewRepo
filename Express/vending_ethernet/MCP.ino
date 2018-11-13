void mux(){
  mcp.begin(0);      // use default address 0
  mcp1.begin(1);
  
  for(int i = 0;i<=14;i++){
   mcp.pinMode(i, INPUT);
   mcp1.pinMode(i, INPUT); 
  }
  for(int i = 0;i<=14;i++){
   mcp.pullUp(i, HIGH);
   mcp1.pullUp(i, HIGH);  
  }
}


void i2c_master(){
  if(motor[num].MCPnum == 1){
      if(Run == true){
        printfeedback('~' +String(mcp.digitalRead(motor[num].Magnetpin)));
        delay(delayMotorPrint);
      }
          
/*
    //Serial.println("L1");
    if (mcp.digitalRead(motor[num].Magnetpin) == 0){
      if(Run == true){
        delay(delayMotorPrint);
        printfeedback("~0");
//        Serial.println("~0");
      }
    }else{
      if(Run == true){
        delay(delayMotorPrint);
        printfeedback("~1");
//        Serial.println("~1");
      }
    }

  */
  
/*    
    if (mcp.digitalRead(motor[num].Magnetpin) == 0){
    Serial.println("L1");
      if(Run == true){
        delay(delayMotorPrint);
        printfeedback("~0");
        Serial.println("~0");
      }
      if(Run == true && Delay == 0){
        delay(1);
        //Serial.println("L3");
        if (mcp.digitalRead(motor[num].Magnetpin) == 0){
          if(reverse)
          {
            //digitalWrite(RVS_PIN,LOW);
          }
          else
          {
            //digitalWrite(RVS_PIN,HIGH);
          }
          delay(30);
          Run = false;
          //digitalWrite(motor[num].X,LOW);
          //digitalWrite(motor[num].Y,LOW);
          //Serial.print("DONE MOTOR ");
          //Serial.println(num);
          printfeedback("~1");
          Serial.println("~1");
          wait = 0;
        }
      }
    }
*/

  }
  else if(motor[num].MCPnum == 2){
      if(Run == true){
        printfeedback('~' +String(mcp1.digitalRead(motor[num].Magnetpin)));
        delay(delayMotorPrint);
      }

/*
    if (mcp1.digitalRead(motor[num].Magnetpin) == 0){
      if(Run == true){
        delay(delayMotorPrint);
        printfeedback("~0");
//        Serial.println("~0");
      }
    }else{
      if(Run == true){
        delay(delayMotorPrint);
        printfeedback("~1");
//        Serial.println("~1");
      }
    }
  */     
    
    /*if (mcp1.digitalRead(motor[num].Magnetpin) == 0){
      //Serial.println("L2");
      if(Run == true){
        delay(delayMotorPrint);
        printfeedback("~0");
        Serial.println("~0");
      }
      if(Run == true && Delay == 0){
        delay(1);
        //Serial.println("L4");
        if (mcp1.digitalRead(motor[num].Magnetpin) == 0){
          if(reverse)
          {
            //digitalWrite(RVS_PIN,LOW);
          }
          else
          {
            //digitalWrite(RVS_PIN,HIGH);
          }
          delay(30);
          Run = false;
          //digitalWrite(motor[num].X,LOW);
          //digitalWrite(motor[num].Y,LOW);
          //Serial.print("DONE MOTOR ");
          //Serial.println(num);
          printfeedback("~1");
          Serial.println("~1");
          wait = 0;
        }
      }
    }
    */
    
  }
}
 
