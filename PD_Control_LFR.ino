

#include<LiquidCrystal.h>
LiquidCrystal lcd(30, 31, 32, 33, 34, 35);


int IN1= 7;
int IN2 = 6;
int IN3 = 5;
int IN4 = 4;

int EN1 = 2;
int EN2 =3;

int S1=53;
int S2=51;
int S3=49;
int S4=47;
int S5=45;


#define Forward 1
#define Revers 0


float Total_sensor_v=0;
float Measured_value=0;
float Control_variable_P=0;
float Control_variable_D=0;
float PRE_ERROR_P=0;
float CUR_ERROR_P=0;


float D_ERROR=0;
float P_ERROR =0;


float Control_variable_I=0; 


int ss1, ss2, ss3, ss4, ss5;

int Left_motor_speed=0;
int Right_motor_speed=0;

#define Left_motor_basespeed 150
#define Right_motor_basespeed 150

int Max_PID = 100;

int Set_value= 3;


double PID=0;

float P_CONSTANT=30;
float D_CONSTANT=2;
float I_CONSTANT=0;



void setup()
{
 pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);
 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT);
 
 pinMode(IN1, OUTPUT);
 pinMode(IN1, OUTPUT);
 
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
  
  lcd.begin(16 , 2);
  lcd.print("PID CONTROL");
}

void loop()
{
  Read_sensor_L();
  PID_Calculation();
  Speed_cal();
  Take_action();
   
}


void PID_Calculation()
{
  P_ERROR = Set_value - Measured_value;
  Control_variable_P = P_ERROR * P_CONSTANT;
  CUR_ERROR_P = P_ERROR;
  D_ERROR = PRE_ERROR_P - CUR_ERROR_P;
  PRE_ERROR_P = CUR_ERROR_P;
  Control_variable_D = D_ERROR * D_CONSTANT;
  Control_variable_I = Control_variable_I + P_ERROR*I_CONSTANT;
  
  PID = Control_variable_P + Control_variable_D + Control_variable_I;
 /* lcd.clear();
  lcd.print("P_ERROR");
  lcd.setCursor(0,1);
  lcd.print (P_ERROR);
  delay(1000);
  lcd.clear();
  lcd.print("PID");
  lcd.setCursor(0,1);
  lcd.print (PID);
   delay(1000);*/
  
}


void Take_action()
{
 /* lcd.clear();
  lcd.print("Left_motor_speed");
  lcd.setCursor(0,1);
  lcd.print(Left_motor_speed);
  delay(1000);
  lcd.clear();
  lcd.print("Right_motor_speed");
  lcd.setCursor(0,1);
  lcd.print(Right_motor_speed);
  delay(1000);  */
  
  if(Right_motor_speed >0)
  {
    Motor_right(Forward, Right_motor_speed );
  }
  else
  {
    Motor_right(Revers,Right_motor_speed );
  }
  
  if(Left_motor_speed >0)
  {
    Motor_left(Forward, Left_motor_speed );
  }
  else 
  {
    Motor_left(Revers, Left_motor_speed );
  }
  
  
  
  
 }
void Speed_cal()
{
 if(PID > Max_PID)
  {
    PID = Max_PID;
   // Left_motor_speed = Left_motor_basespeed - PID;
   // Right_motor_speed = Right_motor_basespeed + PID;
  }
  else if(PID <- Max_PID)
  {
    PID = -Max_PID;
   // Left_motor_speed = Left_motor_basespeed - PID;
   // Right_motor_speed = Right_motor_basespeed + PID;
  }
  
 
  
  
  
  
  
  if(PID < 0 )  // if error is negative 
  {
    Left_motor_speed = Left_motor_basespeed;
    Right_motor_speed = Right_motor_basespeed + PID;
  }
  else  // if error is positive 
  {
    Left_motor_speed = Left_motor_basespeed - PID;
    Right_motor_speed = Right_motor_basespeed;
  }
  
  
  
}



void Read_sensor_L()
{
  if(digitalRead(S1)==HIGH)
    ss1=1;
    else
      ss1=0;
    
      if(digitalRead(S2)==HIGH)
        ss2=1;
        else
          ss2=0;
      
          if(digitalRead(S3)==HIGH)
            ss3=1;
            else
              ss3=0;
        
              if(digitalRead(S4)==HIGH)
                ss4=1;
                else
                  ss4=0;
          
                  if(digitalRead(S5)==HIGH)
                  ss5=1;
                    else
                      ss5=0;
                    
                    
Total_sensor_v=ss1*1+ss2*2+ss3*3+ss4*4+ss5*5;
Total_sensor_v = Total_sensor_v /(ss1*1 + ss2*1 + ss3*1 + ss4*1 + ss5*1);
Measured_value = Total_sensor_v;
/*lcd.clear();
lcd.print("Measured_value");
lcd.setCursor(0,1);
lcd.print(Measured_value);
delay(1000); */

}

void Motor_left(int dir, int spreed_l)
{
  if(dir==1)
    {
      digitalWrite(IN1,HIGH);
      digitalWrite(IN2,LOW);
    }
    else
    {
      digitalWrite(IN1,LOW);
      digitalWrite(IN2,HIGH);
    }
    
  analogWrite(EN1,spreed_l);    

}


void Motor_right(int dir, int spreed_r)
{
  if(dir==1)
    {
      digitalWrite(IN3,HIGH);
      digitalWrite(IN4,LOW);
    }
    else
    {
      digitalWrite(IN3,LOW);
      digitalWrite(IN4,HIGH);
    }
    
  analogWrite(EN2,spreed_r);    

}




