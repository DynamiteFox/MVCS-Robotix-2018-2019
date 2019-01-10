#pragma config(Sensor, in1,    potClaw,        sensorPotentiometer)
#pragma config(Sensor, in2,    potLift,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  encBL,          sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  LED,            sensorLEDtoVCC)
#pragma config(Sensor, dgtl5,  encFR,          sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  encLift,        sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  encBR,          sensorQuadEncoder)
#pragma config(Sensor, dgtl11, encFL,          sensorQuadEncoder)
#pragma config(Motor,  port2,           mBL,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           mLift,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           mIntake,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           mFL,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           mFR,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           mClaw,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           mFlyWheelR,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           mFlyWheelL,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          mBR,           tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*

--- README ---

8U: Set flyWheel full speed
8L: Set flyWheel half speed
8D: Set flyWheel zero speed (turn off)
*there is no flyWheel toggle button

7R: Toggle assist

*/

// potClaw {up: 4096, down: 600}

#pragma platform(VEX2)

#pragma competitionControl(Competition)

#include "Vex_Competition_Includes.c"

#define __DRIVE_SPEED_FACTOR_SLOW 0.5
#define __FLYWHEEL_SECONDARY_SPEED 67


void pre_auton() {
	bStopTasksBetweenModes = true;
}


task autonomous() {
}

// USER CONTROL


int flyWheelSpeed = 0;
int flyWheelSpeed_Memory = __FLYWHEEL_SECONDARY_SPEED;

float driveFactor = 1.0;
float FL, FR, BL, BR, headflip = 1.0;

task LED_Update () {
	int LEDFlashStatus = 0;
	while (true) {
		if (flyWheelSpeed == 127 || flyWheelSpeed == 127) {
			SensorValue[LED] = 0;
			} else if (flyWheelSpeed > __FLYWHEEL_SECONDARY_SPEED) {
			SensorValue[LED] = LEDFlashStatus;
			LEDFlashStatus = 1 - LEDFlashStatus;
			wait1Msec(100);
			} else if (flyWheelSpeed == __FLYWHEEL_SECONDARY_SPEED) {
			SensorValue[LED] = 1;
			} else if (flyWheelSpeed < __FLYWHEEL_SECONDARY_SPEED) {
			SensorValue[LED] = LEDFlashStatus;
			LEDFlashStatus = 1 - LEDFlashStatus;
			wait1Msec(500);
		}
		EndTimeSlice();
	}
}

task ballIntake () {
	while (true) {
		if (vexRT[Btn6U] && !vexRT[Btn6D]) {
			motor[mIntake] = 127;
			} else if (!vexRT[Btn6U] && vexRT[Btn6D]) {
			motor[mIntake] = -127;
			} else {
			motor[mIntake] = 0;
		}
		EndTimeSlice();
	}
}

task flyWheel () {
	while (true) {
		motor[mFlyWheelL] = motor[mFlyWheelR] = flyWheelSpeed;
		if (vexRT[Btn8U]) { // set full speed button pushed
			flyWheelSpeed = 127;
			} else if (vexRT[Btn8L]) { // set secondary speed button pushed
			flyWheelSpeed = __FLYWHEEL_SECONDARY_SPEED;
			} else if (vexRT[Btn8D]) {
			flyWheelSpeed = 0;
			} else if (vexRT[Btn8R]) {
			flyWheelSpeed = flyWheelSpeed_Memory;
		}
		if (vexRT[Btn6U] && vexRT[Btn6D]) {
			if (vexRT[Btn8U]) {
				while (vexRT[Btn8U]) {
					wait1Msec(10);
				}
				flyWheelSpeed_Memory += 10;
				flyWheelSpeed_Memory = flyWheelSpeed_Memory > 127 ? 127 : flyWheelSpeed_Memory;
				flyWheelSpeed = flyWheelSpeed_Memory;
				} else if (vexRT[Btn8D]) {
				while (vexRT[Btn8D]) {
					wait1Msec(10);
				}
				flyWheelSpeed_Memory -= 10;
				flyWheelSpeed_Memory = flyWheelSpeed_Memory < 0 ? 0 : flyWheelSpeed_Memory;
				flyWheelSpeed = flyWheelSpeed_Memory;
			}
		}
		EndTimeSlice();
	}
}
task lifter () { // task controlling the cascade-lift an the claw
	while (true) {
		if (vexRT[Btn5U] && vexRT[Btn5D]) {
			motor[mLift] = 0;
			}	else if (vexRT[Btn5U]) {
			motor[mLift] = 127;
			} else if (vexRT[Btn5D]) {
			motor[mLift] = -127;
			} else {
			motor[mLift] = 0;
		}
		EndTimeSlice();
	}
}

bool clawInAction = false;

void moveClawUp (int toPos) {
	clawInAction = true;
	clearTimer(T1); // set timer in order to prevent burning out motors
	while (SensorValue[potClaw] < toPos && time1[T1] < 3000) {
		motor[mClaw] = 127;
	}
	motor[mClaw] = 0;
	clawInAction = false;
}
void moveClawDown (int toPos) {
	clawInAction = true;
	clearTimer(T1); // set timer in order to prevent burning out motors
	while (SensorValue[potClaw] > toPos && time1[T1] < 3000) {
		motor[mClaw] = -77;
	}
	motor[mClaw] = 0;
	clawInAction = false;
}

int curClawPos;

void initClawMovement (int toPos) {
	curClawPos = SensorValue[potClaw];
	if (curClawPos < toPos) {
		motor[mClaw] = 127;
		} else if (curClawPos < toPos + 100) {
		motor[mClaw] = 60;//(toPos - curClawPos) / 2;
		} else if (curClawPos < toPos + 227) {
		motor[mClaw] = 10;//(curClawPos - toPos) * 1;
		} else if (curClawPos < toPos + 500) {
		motor[mClaw] = 0;
		} else {
		motor[mClaw] = -10;
	}
}


void flipOnBot () {
	clawInAction = true;
	while (true) {
		if ((vexRT[Btn5U] && vexRT[Btn5D]) && (vexRT[Btn7U] || vexRT[Btn7D]) ||
			vexRT[Btn7U] || vexRT[Btn7R]) {
			break;
		}
		initClawMovement (1800);
	}
	clawInAction = false;
}

bool clawAssistOn = false, clawInManualControl = false;
task clawAssist () {
	while (true) {
		if (vexRT[Btn7R]) {
			while (vexRT[Btn7R]) { }
			clawAssistOn = !clawAssistOn;
			moveClawDown (600);
		}
		if (clawAssistOn && !clawInAction && !clawInManualControl) {
			initClawMovement (900);
		}
		EndTimeSlice ();
	}
}
task clawControl(){  //basic motor control by 2 buttons
	while(true){
		if(vexRT[Btn7R]){
			motor[mClaw] = 40;
			} else if (vexRT[Btn7L]) {
			motor[mClaw] = -40;
			} else {
			motor[mClaw] = 0;
		}
		EndTimeSlice();
	}
}
task Lift(){//basic motor control by 2 buttons
	while(true){
		if(vexRT[Btn5U]){
			motor[mLift] = 127;
			} else if (vexRT[Btn5D]) {
			motor[mLift] = -127;
			} else {
			motor[mLift] = 0;
		}
		EndTimeSlice();
	}
}
task SmartLift(){
	int bool stay=true;
	int pos=SensorValue[potLift];
	while(true){
		if(vexRT[Btn5U]){
			stay=false;
			motor[mLift] = 127;
			} else if (vexRT[Btn5D]) {
			stay=false;
			motor[mLift] = -127;
			} else if (!stay){
			pos=SensorValue[potLift];
			stay=true;
			motor[mLift] = 0;
			}	else {
			motor[mLift] = 0.1*(pos-SensorValue[potLift]);
		}
		EndTimeSlice();
	}
}
task claw () {
	while (true) {
		if (vexRT[Btn5U] && vexRT[Btn5D]) {
			if (vexRT[Btn7U]) {
				//clawInManualControl = true;
				motor[mClaw] = 127;
				} else if (vexRT[Btn7D]) {
				//clawInManualControl = true;
				motor[mClaw] = -127;
				} else {
				//clawInManualControl = false;
				motor[mClaw] = 0;
			}
			} else {
			if (vexRT[Btn7R]) {
				moveClawDown (600);
			}
			if (vexRT[Btn7U]) { // flip on ground
				moveClawUp (1500);
				moveClawDown (600);
			}
			if (vexRT[Btn7D]) { // flip on bot
				//moveClawUp (2000); // change this number to change the target pot position
				moveClawUp (2050);
				motor[mClaw] = 0;
				flipOnBot();
			}
		}
		EndTimeSlice();
	}
}

task drive() {
	int vch1;
	while (true) {
		driveFactor = __DRIVE_SPEED_FACTOR_SLOW;
		// Death zones
		if (abs(vexRT[Ch4]) < 20 && abs(vexRT[Ch3]) < 20 && abs(vexRT[Ch1]) < 20) {
			driveFactor = 0;
			} else {
			vch1 = vexRT[Ch1];
			if (abs (vch1) < 100 && abs(vch1) > 20) {
				vch1 = vch1 > 0 ? 60 : -60;
			}
			FR = (headflip * (-vexRT[Ch4] + vexRT[Ch3])) + vch1; //Determines motor speeds. Joshua's Code.
			FL = (headflip * (-vexRT[Ch4] - vexRT[Ch3])) + vch1;
			BL = (headflip * (vexRT[Ch4] - vexRT[Ch3])) + vch1;
			BR = (headflip * (vexRT[Ch4] + vexRT[Ch3])) + vch1;

			if(sqrt((vexRT[Ch4] * vexRT[Ch4]) + (vexRT[Ch3] * vexRT[Ch3])) < 100 &&
				sqrt((vexRT[Ch1] * vexRT[Ch1]) + (vexRT[Ch2] * vexRT[Ch2])) < 100) {
				driveFactor = 0.5;
				} else {
				driveFactor = 1.0;
			}

		}
		if (vexRT[Btn7D]) { // flip head
			while (vexRT[Btn7D]) { wait1Msec(10);}
			headflip = -headflip;
		}
		// Make sure at least one of the motors is running at full spead
		/*
		maxMotor = ms[0];
		for (i = 1; i < 4; ++i) {
		maxMotor = fmax (ms[i], maxMotor);
		}
		targetRatio = 127.0 / maxMotor;
		for (i = 0; i < 4; ++i) {
		ms[i] *= targetRatio;
		}
		motor[mFL] = ms[0] * driveFactor;
		motor[mFR] = ms[1] * driveFactor;
		motor[mBL] = ms[2] * driveFactor;
		motor[mBR] = ms[3] * driveFactor;
		*/

		// Set each motor to a target speed
		motor[mFL] = FL * driveFactor;
		motor[mFR] = FR * driveFactor;
		motor[mBL] = BL * driveFactor;
		motor[mBR] = BR * driveFactor;
		EndTimeSlice();
	}
}

task usercontrol() {
	startTask(drive);
	startTask(ballIntake);
	startTask(flyWheel);
	startTask(SmartLift);
	startTask(clawControl);
	startTask(LED_Update);
	while (true) {
		EndTimeSlice();
	}
}
