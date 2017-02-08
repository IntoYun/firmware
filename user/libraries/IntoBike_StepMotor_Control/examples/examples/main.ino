/*该头文件引用由IntoRobot自动添加.*/
#include <IntoBike_StepMotor_Control/IntoBike_StepMotor_Control.h>
//motor pin definitions
#define FR_L   D4    //Left  motor: Direction
#define STEP_L D0    //Left  motor: PWM pulse, One pulse means one step for the Step Motor
#define FR_R   D7    //Right motor: Direction
#define STEP_R D3    //Right motor: PWM pulse, One pulse means one step for the Step Motor
#define STOP   A4    //Enable motor, Active HIGH 

StepMotorControl          _stepmotor_control[2];

void setup() 
{
// put your setup code here, to run once.
	_stepmotor_control[0].begin(FR_L, STEP_L, STOP);
	_stepmotor_control[1].begin(FR_R, STEP_R, STOP);
	_stepmotor_control[0].enableMotors();
	_stepmotor_control[1].enableMotors();
}

void loop() 
{
// put your main code here, to run repeatedly.
	_stepmotor_control[0].setMotorSpeed(1000);
	_stepmotor_control[1].setMotorSpeed(1000);
	delay(1000);
}