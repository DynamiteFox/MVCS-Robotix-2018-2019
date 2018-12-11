#pragma config(Sensor, in2,    potClaw,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  encLift,        sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  LED,            sensorLEDtoVCC)
#pragma config(Sensor, dgtl5,  encFR,          sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  encBL,          sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  encBR,          sensorQuadEncoder)
#pragma config(Sensor, dgtl11, encFL,          sensorQuadEncoder)
#pragma config(Motor,  port1,           mFL,           tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           mBL,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           mBR,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           mIntake,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           mLift,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           mFR,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           mClaw,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           mFlyWheelR,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           mFlyWheelL,    tmotorVex393HighSpeed_MC29, openLoop)
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


/*
goFoward function doc
	direction can be one of the four: 12, 3, 6, 9
		let flyWheel be 12 o'clock
		lift facing 6 o'clock, etc
*/
int mListDirection[4][13] = // 0: FL, 1: FR, 2: BL, 3: BR
{{0, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, -1},
{0, 0, 0, 1, 0, 0, -1, 0, 0, -1, 0, 0, 1},
{0, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, -1},
{0, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 1}};

int mListDrive[13][4] = {{0, 0, 0, 0},
	{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 3, 0, 2},  //  3 o'clock
	{0, 0, 0, 0}, {0, 0, 0, 0}, {3, 2, 1, 0},  //  6 o'clock
	{0, 0, 0, 0}, {0, 0, 0, 0}, {2, 0, 3, 1},  //  9 o'clock
	{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 2, 3}}; // 12 o'clock

int encListDrive[13][4] = {{0, 0, 0, 0},
	{0, 0, 0, 0}, {0, 0, 0, 0}, {-1, 1, 1, -1},    //  3 o'clock
	{0, 0, 0, 0}, {0, 0, 0, 0}, {-1, -1, -1, -1},  //  6 o'clock
	{0, 0, 0, 0}, {0, 0, 0, 0}, {1, -1, -1, 1},    //  9 o'clock
	{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}};     // 12 o'clock

int motorPower[4] = {0, 0, 0, 0};
int encoderValues[4];

int encLeft, encRight;
void resetEncoderValues () {
	encLeft = encRight = SensorValue[encFL] = SensorValue[encFR] = SensorValue[encBL] = SensorValue[encBR] = 0;
}
void getEncoderValues (int direction) {
	encoderValues[0] = -SensorValue[encFL] * encListDrive[direction][0];
	encoderValues[1] = -SensorValue[encFR] * encListDrive[direction][1];
	encoderValues[2] =  SensorValue[encBL] * encListDrive[direction][2];
	encoderValues[3] = -SensorValue[encBR] * encListDrive[direction][3];
	encLeft = (encoderValues[mListDrive[direction][0]] + encoderValues[mListDrive[direction][2]]) / 2;
	encRight = (encoderValues[mListDrive[direction][1]] + encoderValues[mListDrive[direction][3]]) / 2;
}

void goFoward (int direction, int distance) {
	if (mListDirection[0][direction] == 0) {
		return;
	}
	resetEncoderValues();
	while ((encLeft + encRight) / 2 < distance) {
		getEncoderValues(direction);
		if (abs(encLeft - encRight) > 5) {
			motorPower[mListDrive[direction][0]] = motorPower[mListDrive[direction][2]] = encLeft > encRight ? 100 : 127;
			motorPower[mListDrive[direction][1]] = motorPower[mListDrive[direction][3]] = encLeft > encRight ? 127 : 100;
		} else {
			motorPower[0] = motorPower[1] = motorPower[2] = motorPower[3] = 127;
		}

		motor[mFL] = mListDirection[0][direction] * motorPower[0];
		motor[mFR] = mListDirection[1][direction] * motorPower[1];
		motor[mBL] = mListDirection[2][direction] * motorPower[2];
		motor[mBR] = mListDirection[3][direction] * motorPower[3];
	}
	motor[mFL] = motor[mFR] = motor[mBL] = motor[mBR] = 0;
}

void turn (int power, int distance) {
	motor[mFL] = motor[mBL] = motor[mFR] = motor[mBR] = -power;
	while (abs (SensorValue[encBL]) < distance) {
		wait1Msec(1);
	}
	motor[mFL] = motor[mBL] = motor[mFR] = motor[mBR] = power > 0 ? 35 : -35;
	wait1Msec(50);
	motor[mFL] = motor[mBL] = motor[mFR] = motor[mBR] = 0;
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

void auton () {
	goFoward (12, 950);
	motor[mFlyWheelL] = motor[mFlyWheelR] = 127;
	goFoward (6, 500);
	goFoward (9, 90);
	motor[mIntake] = 127;
	wait1Msec(2500);
	motor[mFlyWheelL] = motor[mFlyWheelR] = motor[mIntake] = 0;
	goFoward (12, 65);
	turn(127, 413);
	motor[mIntake] = -127;
	goFoward (12, 500);
	motor[mIntake] = 0;
	goFoward (3, 440);
	goFoward (12, 350);
	goFoward (3, 50);
	goFoward (12, 80);
}

task autonomous() {
	auton();
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
		initClawMovement (1950);
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

task claw () {
	while (true) {
		if (vexRT[Btn5U] && vexRT[Btn5D]) {
			if (vexRT[Btn7U]) {
				clawInManualControl = true;
				motor[mClaw] = 127;
			} else if (vexRT[Btn7D]) {
				clawInManualControl = true;
				motor[mClaw] = -127;
			} else {
				clawInManualControl = false;
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
				moveClawUp (2150);
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
			FR = (headflip * (-vexRT[Ch4] + vexRT[Ch3])) - vch1; //Determines motor speeds. Joshua's Code.
      FL = (headflip * (-vexRT[Ch4] - vexRT[Ch3])) - vch1;
      BL = (headflip * (vexRT[Ch4] - vexRT[Ch3])) - vch1;
      BR = (headflip * (vexRT[Ch4] + vexRT[Ch3])) - vch1;

			if(sqrt((vexRT[Ch4] * vexRT[Ch4]) + (vexRT[Ch3] * vexRT[Ch3])) < 100 &&
				 sqrt((vexRT[Ch1] * vexRT[Ch1]) + (vexRT[Ch2] * vexRT[Ch2])) < 100) {
				driveFactor = 0.5;
			} else {
				driveFactor = 1.0;
			}

		}
		if (vexRT[Btn7L]) { // flip head
    	while (vexRT[Btn7L]) { wait1Msec(10);}
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
	startTask(lifter);
	startTask(claw);
	startTask(clawAssist);
	startTask(LED_Update);
  while (true) {
    EndTimeSlice();
  }
}
