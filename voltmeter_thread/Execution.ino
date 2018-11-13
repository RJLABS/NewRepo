void Execution(){
  Buzzer_temp=false;
  if(Suhu>Suhu_limit){
    Buzzer_temp=true;
    Respon_Client("ERR:SUHU");
  }
  
  if(aruscontrol>Arus_control_limit){
    Buzzer_temp=true;
    Respon_Client("ERR:ICON");
  }
  
  if(arusmotor>Arus_motor_limit){
    Buzzer_temp=true;
    Respon_Client("ERR:IMOT");
  }

  if(tegangancontrol>1){
    if(tegangancontrol<control_limit_min || tegangancontrol>control_limit_max){
      Buzzer_temp=true;
      if(tegangancontrol<control_limit_min){
        Respon_Client("ERR:VCONMIN");
      }
      else if(tegangancontrol>control_limit_max){
        Respon_Client("ERR:VCONMAX");
      }
    }
  }
  
  if(teganganmotor>1){
    if(teganganmotor<motor_limit_min || teganganmotor>motor_limit_max){
      Buzzer_temp=true;
      if(teganganmotor<motor_limit_min){
        Respon_Client("ERR:VMOTMIN");
      }
      else if(teganganmotor>motor_limit_max){
        Respon_Client("ERR:VMOTMAX");
      }
    }
  }
  
  if(Fire_flag==true){
    Buzzer_temp=true;
    Respon_Client("ERR:FIRE");
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

