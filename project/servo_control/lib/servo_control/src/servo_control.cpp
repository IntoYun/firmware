#include "servo_control.h"


void servo_control::init()
{
    /*
  pinMode(motor1_pwm,OUTPUT);
  pinMode(motor2_pwm,OUTPUT);
  pinMode(motor3_pwm,OUTPUT);
  pinMode(motor4_pwm,OUTPUT);
  pinMode(motor1_rst,AN_INPUT);
  pinMode(motor2_rst,AN_INPUT);
  pinMode(motor3_rst,AN_INPUT);
  pinMode(motor4_rst,AN_INPUT);
  pinMode(grasp_pin1,OUTPUT);
  pinMode(grasp_pin2,OUTPUT);
  pinMode(grasp_pin3,OUTPUT);
  */
  Serial.begin(115200);
  /*
  analogWriteResolution(motor1_pwm,16);
  analogWriteResolution(motor2_pwm,16);
  analogWriteResolution(motor3_pwm,16);
  analogWriteResolution(motor4_pwm,16);
 // analogWrite(motor1_pwm,3194-COMPENSATION_motor1_pwm,50);
  //analogWrite(motor2_pwm,3194-COMPENSATION_motor2_pwm,50);
 // analogWrite(motor3_pwm,3194-COMPENSATION_motor3_pwm,50);
 // analogWrite(motor4_pwm,3194-COMPENSATION_motor3_pwm,50);
    digitalWrite(grasp_pin1,LOW);
    digitalWrite(grasp_pin2,LOW);
    digitalWrite(grasp_pin3,LOW);
    */
}

void servo_control::get_angle()
{
   // Serial.print("enter get_angle");
    for(int i=0;i<4;i++)
    {
        angle[i]=(((float)prot->data[i*2])*256.0+(float)prot->data[i*2+1])/10.0;
    }
}

void servo_control::get_pwm(float *angle_)
{
   // Serial.print("enter get_pwm");
    for(int i=0;i<3;i++)
    {
        pwm_time[i]=uint32_t(((angle_[i]/0.09)+500.0)/20000.0*65535.0);//180度分成2000份，一份是0.09
    }
    pwm_time[3]=uint32_t(((angle_[3]/0.072)+500.0)/20000.0*65535.0);//180度分成2000份，一份是0.09
}

void servo_control::pwm_action()
{
    if(step_cnt)
    {
        step_cnt--;
        for(int i=0;i<4;i++)
        {
            set_angle[i]=last_set_angle[i]+d_angle[i];
            last_set_angle[i]=set_angle[i];
            //if(fabs(set_angle[i]-angle[i])<1.0)set_angle[i]=angle[i];
        }
        get_pwm(set_angle);
        if(set_angle[0]<=180.0&&pwm_time[0]!=last_set_pwm[0]) {analogWrite(motor1_pwm,pwm_time[0],50);last_set_pwm[0]=pwm_time[0];}
        if(set_angle[1]<=180.0&&pwm_time[1]!=last_set_pwm[1]) {analogWrite(motor2_pwm,pwm_time[1],50);last_set_pwm[1]=pwm_time[1];}
        if(set_angle[2]<=180.0&&pwm_time[2]!=last_set_pwm[2]) {analogWrite(motor3_pwm,pwm_time[2],50);last_set_pwm[2]=pwm_time[2];}
        if(set_angle[3]<=180.0&&pwm_time[3]!=last_set_pwm[3]) {analogWrite(motor4_pwm,pwm_time[3],50);last_set_pwm[3]=pwm_time[3];}
    }
    if(grasp_step_cnt)
    {
         grasp_step_cnt--;
         if(grasp_mode==1)
         {
             digitalWrite(grasp_pin1,HIGH);
             digitalWrite(grasp_pin2,LOW);
             digitalWrite(grasp_pin3,HIGH);
         }
         else if(grasp_mode==2)
         {
             digitalWrite(grasp_pin1,LOW);
             digitalWrite(grasp_pin2,HIGH);
             digitalWrite(grasp_pin3,HIGH);
         }
         else if(grasp_mode==3)
         {
             digitalWrite(grasp_pin1,LOW);
             digitalWrite(grasp_pin2,LOW);
             digitalWrite(grasp_pin3,LOW);
         }
    }
    else
    {
         digitalWrite(grasp_pin1,LOW);
             digitalWrite(grasp_pin2,LOW);
             digitalWrite(grasp_pin3,LOW);
    }

}

void servo_control::control_pwm()
{
    static uint32_t i=0;
    if(Serial.available()>5)
      {
       //delay(5);
       //Serial.printf("start");
       get_pwm_time=get_num_of_xyz(prot);
       if(get_pwm_time)
       {
           switch(prot->command)
             {
                 case 0x01:{
                     uint16_t angle_read[4];
                     angle_read[0] = 2010.0-0.5433*analogRead(motor1_rst);//(uint16_t)(((3635.0-(float)analogRead(motor1_rst))*1.0/17.0)*10.0);
                     angle_read[1] = 2010.0-0.5433*analogRead(motor2_rst);//(uint16_t)(((3635.0-(float)analogRead(motor2_rst))*1.0/17.0)*10.0);
                     angle_read[2] = 2010.0-0.5433*analogRead(motor3_rst);//(uint16_t)(((3635.0-(float)analogRead(motor3_rst))*1.0/17.0)*10.0);
                     angle_read[3] = 1683.2061-0.6870229*analogRead(motor4_rst);//(uint16_t)(((3635.0-(float)analogRead(motor4_rst))*1.0/17.0)*10.0);
                     //angle_read[3] = map(analogRead(motor4_rst),2450,1140,0,900);//(uint16_t)(((3635.0-(float)analogRead(motor4_rst))*1.0/17.0)*10.0);
                     struct protocal prot_send;
                     prot_send.head[0]=0x24;prot_send.head[1]=0x4d;prot_send.head[2]=0x3c;
                     prot_send.data_length=0x08;
                     prot_send.command=0x01;
                     static bool converHL;
                     converHL=true;
                     for(i=0;i<prot_send.data_length;i++)
                     {
                        if(converHL){
                            prot_send.data[i]=byte(angle_read[i/2]/256);
                            converHL=false;
                        }
                        else{
                            prot_send.data[i]=byte(angle_read[i/2]%256);
                            converHL=true;
                        }
                     }
                     prot_send.checksum=getChecksum(prot_send.data_length,prot_send.command, prot_send.data);
                     byte send[14];
                     for(i=0;i<14;i++)
                     {
                         switch(i){
                             case(0):case(1):
                             case(2):send[i]=prot_send.head[i];break;
                             case(3):send[i]=prot_send.data_length;break;
                             case(4):send[i]=prot_send.command;break;
                             case(5):case(6):case(7):case(8):case(9):case(10):case(11):
                             case(12):send[i]=prot_send.data[i-5];break;
                             case(13):send[i]=prot_send.checksum;break;
                         }
                     }
                     Serial.write(send,14);
                     break;
                 }
                 case 0x02:{
                    for(int i=0;i<4;i++)
                    {
                        angle[i]=(((float)prot->data[i*2])*256.0+(float)prot->data[i*2+1])/10.0;
                    }
                    action_time=prot->data[8]*256+prot->data[9];
                    if(last_set_angle[0]==999.0)//reset last_set_angle by angle_read
                    {
                        uint16_t angle_read[4];
                        angle_read[0] = 2010.0-0.5433*analogRead(motor1_rst);//(uint16_t)(((3635.0-(float)analogRead(motor1_rst))*1.0/17.0)*10.0);
                        angle_read[1] = 2010.0-0.5433*analogRead(motor2_rst);//(uint16_t)(((3635.0-(float)analogRead(motor2_rst))*1.0/17.0)*10.0);
                        angle_read[2] = 2010.0-0.5433*analogRead(motor3_rst);//(uint16_t)(((3635.0-(float)analogRead(motor3_rst))*1.0/17.0)*10.0);
                        angle_read[3] = 1683.2061-0.6870229*analogRead(motor4_rst);//(uint16_t)(((3635.0-(float)analogRead(motor4_rst))*1.0/17.0)*10.0);
                        for(int i=0;i<4;i++)last_set_angle[i]=angle_read[i]/10.0;
                    }
                    step_cnt=action_time/dt+1;
                     for(int i=0;i<4;i++)d_angle[i]=(angle[i]-last_set_angle[i])/step_cnt;

                     break;
                 }
                 case 0x03:{
                     get_angle();
                     get_pwm(angle);
                     if(angle[0]<=180.0&&pwm_time[0]!=last_set_pwm[0]) {analogWrite(motor1_pwm,pwm_time[0],50);last_set_pwm[0]=pwm_time[0];}
                     if(angle[1]<=180.0&&pwm_time[1]!=last_set_pwm[1]) {analogWrite(motor2_pwm,pwm_time[1],50);last_set_pwm[1]=pwm_time[1];}
                     if(angle[2]<=180.0&&pwm_time[2]!=last_set_pwm[2]) {analogWrite(motor3_pwm,pwm_time[2],50);last_set_pwm[2]=pwm_time[2];}
                     if(angle[3]<=180.0&&pwm_time[3]!=last_set_pwm[3]) {analogWrite(motor4_pwm,pwm_time[3],50);last_set_pwm[3]=pwm_time[3];}
                     //Serial.print("enter 0x03");
                     //Serial.printf("%f\n%f\n%f\n%f\n",angle[0],angle[1],angle[2],angle[3]);
                    // Serial.printf("%d\n%d\n%d\n%d\n",pwm_time[0],pwm_time[1],pwm_time[2],pwm_time[3]);
                     break;
                 }
                  case 0x04:{
                     DEBUG("cmd 4");
                     grasp_mode=prot->data[0];
                     grasp_step_cnt=1500/dt+1;
                     break;
                 }
             }
       }
      }
}