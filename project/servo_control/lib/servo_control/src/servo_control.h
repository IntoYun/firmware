/*该头文件引用由IntoRobot自动添加.*/
#include "serial_resolution.h"
#include "math.h"
#ifndef __servo_control_H__
#define __servo_control_H__
#include"application.h"
#define dt 5
#define motor1_pwm D7
#define motor2_pwm A0
#define motor3_pwm D3
#define motor4_pwm D2
#define motor1_rst A4
#define motor2_rst A5
#define motor3_rst A6
#define motor4_rst A7

#define grasp_pin3 D6
#define grasp_pin2 D1
#define grasp_pin1 D0
class servo_control
{
    public:
    servo_control() {prot=new protocal;}
    void init();
    void control_pwm();
    void pwm_action();
    float angle[4]={0};
    int32_t pwm_time[4]={3194,3194,3194,3194};

    //uint32_t* return_COMPEN()
    //{
    //   uint32_t *r;
    //    *(r+0)=COMPENSATION_D0;
    //    *(r+1)=COMPENSATION_D2;
    //    *(r+2)=COMPENSATION_D3;
    //    return r;
    //}
    private:
    struct protocal *prot;
    void get_angle();
    void get_pwm(float *angle_);
    //const uint32_t COMPENSATION_D0=0;
    //const uint32_t COMPENSATION_D2=0;
    //const uint32_t COMPENSATION_D3=0;
    bool get_pwm_time=false;
    public:
    int32_t last_set_pwm[4]={3194,3194,3194,3194};
    float last_set_angle[4]={999.0f,999.0f,999.0f,999.0f};
    float set_angle[4];
    float d_angle[4];
    int32_t step_cnt;
    int32_t action_time;
    int32_t grasp_step_cnt;
    int32_t grasp_mode;
};

#endif