void Execution(){
  Buzzer_temp=false;
  if(Suhu>Suhu_limit){
    Buzzer_temp=true;
    Respon_Client("TEMP_WARNING");
  }
  
  if(aruscontrol>Arus_control_limit){
    Buzzer_temp=true;
    Respon_Client("ICONTROL_WARNING");
  }
  
  if(arusmotor>Arus_motor_limit){
    Buzzer_temp=true;
    Respon_Client("IMOTOR_WARNING");
  }

  if(tegangancontrol>1){
    if(tegangancontrol<control_limit_min || tegangancontrol>control_limit_max){
      Buzzer_temp=true;
      Respon_Client("VCONTROL_WARNING");
    }
  }
  
  if(teganganmotor>1){
    if(teganganmotor<motor_limit_min || teganganmotor>motor_limit_max){
      Buzzer_temp=true;
      Respon_Client("VMOTOR_WARNING");
    }
  }
  
  if(Fire_flag==true){
    Buzzer_temp=true;
    Respon_Client("FIRE_WARNING");
  }

//-------------------Exec Buzzer & Relay----------------------
  if(Buzzer_flag==true){
    if(Buzzer_temp==true){
      digitalWrite(Pin_Buzzer,HIGH);
    }else{
      digitalWrite(Pin_Buzzer,LOW);
    }
  }else{
    digitalWrite(Pin_Buzzer,LOW);
  }

  if(Exec_flag==true){
    if(Buzzer_temp==true){
      if(Relay_state==false){
        Relay_state=true;
        digitalWrite(Pin_Relay_SSR,HIGH);     
      }
    } 
  }
}

