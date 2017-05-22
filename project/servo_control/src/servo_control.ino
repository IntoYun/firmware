
//SYSTEM_MODE(MANUAL);
SerialDebugOutput debugOutput(115200, ALL_LEVEL);

/*该头文件引用由IntoRobot自动添加.*/
#include "servo_control/servo_control.h"

/*该头文件引用由IntoRobot自动添加.*/
#include "serial_resolution/serial_resolution.h"

servo_control ser_c;
bool do_notnet=false;

void pwm_action()
{
    ser_c.pwm_action();
}
Timer timer(100, pwm_action);

void setup()
{
    ser_c.init();
    timer.start();
}

void loop()
{
    ser_c.control_pwm();

    if(do_notnet==false&&!WiFi.status())
    {
        do_notnet=true;
        timer.reset();
    }
}

