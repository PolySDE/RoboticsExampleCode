#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     sonar1,         sensorSONAR)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    servo1,               tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
#include "JoystickDriver.c"
void initializeRobot(){
  return;
}
task main()
{
	initializeRobot();
	float mod = .4; //Global Speed Modifier
	motor[motorD] = 0;
	motor[motorE] = 0;
	float tempA = 0;
	float tempB = 0;
	waitForStart(); //wait for start
	while(true){
		getJoystickSettings(joystick);
		if(joy1Btn(2) || joy1Btn(3)){
			if(joy1Btn(2)){
				motor[motorA] = 100;
				motor[motorB] = 100;
			}
			if(joy1Btn(3)){
				motor[motorA] = -100;
				motor[motorB] = -100;
			}
		} else {
			motor[motorA] = 0;
			motor[motorB] = 0;
		}
		if(joy1Btn(1) || joy1Btn(4)){
			if(joy1Btn(1)){
				//servo down a step
				servoChangeRate[servo1] = 1;
				servo[servo1] = 150;
			}
			if(joy1Btn(4)){
				//servo up a step
				servoChangeRate[servo1] = 3;
				servo[servo1] = 0;
			}
		}

		if(joystick.joy1_y1 > 5 || joystick.joy1_y1 < -5){
			if(joystick.joy1_y1 > 5){
				tempA = abs(joystick.joy1_y1)*mod;
			}
			if(joystick.joy1_y1 < -5){
				tempA = -abs(joystick.joy1_y1)*mod;
			}
		} else {
			tempA = 0;
		}
		getJoystickSettings(joystick);
		if(joystick.joy1_y2 > 5 || joystick.joy1_y2 < -5){
			if(joystick.joy1_y2 > 5){
				tempB = -(abs(joystick.joy1_y2))*mod;
			}
			if(joystick.joy1_y2 < -5){
				tempB = (abs(joystick.joy1_y2))*mod;
			}
		} else {
			tempB = 0;
		}
		motor[motorD] = tempA;
		motor[motorE] = tempB;
	}

}
