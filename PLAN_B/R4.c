#pragma config(Sensor, in1,    potSelect,      sensorPotentiometer)
#pragma config(Sensor, in2,    potLift,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  encBL,          sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  jumper,         sensorDigitalIn)
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
#pragma config(Motor,  port9,           mFlyWheelL,    tmotorVex393HighSpeed_MC29, openLoop)
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


#define __LIFT_POT_BOTTOM 122
#define __LIFT_POT_LOW_SCORE 304


void pre_auton() {
//	bStopTasksBetweenModes = true;
}
bool flippingOnBot = false, clawInAction = false;

/*
goForward function doc
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
    encLeft = (abs(encoderValues[mListDrive[direction][0]]) + abs(encoderValues[mListDrive[direction][2]])) / 2;
    encRight = (abs(encoderValues[mListDrive[direction][1]]) + abs(encoderValues[mListDrive[direction][3]])) / 2;
}

const int encLeftRatio = 60, encRightRatio = 30;

bool encLeftGoesFurther (int direction) {
    if (direction == 12) {
        return encLeft * encLeftRatio > encRight * encRightRatio;
    } else {
        return encLeft > encRight;
    }
}

void goForward (int direction, int distance) {
    if (mListDirection[0][direction] == 0) {
        return;
    }
    resetEncoderValues();
    while ((encLeft + encRight) / 2 < distance) {
        getEncoderValues(direction);
        if (direction == 6 ? abs(encLeft * encLeftRatio - encRight * encRightRatio) : abs(encLeft - encRight) > 30) {
            motorPower[mListDrive[direction][0]] = motorPower[mListDrive[direction][2]] = encLeftGoesFurther(direction) ? 95 : 127;
            motorPower[mListDrive[direction][1]] = motorPower[mListDrive[direction][3]] = encLeftGoesFurther(direction) ? 127 : 95;
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
    resetEncoderValues();
    while (abs (SensorValue[encFL]) + abs (SensorValue[encFR]) + abs (SensorValue[encBL]) + abs (SensorValue[encBR]) < distance) {
        wait1Msec(1);
    }
    motor[mFL] = motor[mBL] = motor[mFR] = motor[mBR] = power > 0 ? 35 : -35;
    wait1Msec(50);
    motor[mFL] = motor[mBL] = motor[mFR] = motor[mBR] = 0;
}
#define driveF(h) goForward(6,h)
#define driveB(h) goForward(12,h)
#define driveL(h) goForward(3,h)
#define driveR(h) goForward(9,h)

void autonBack(){
    goForward(6,200);
    motor[mLift] = 127;
    wait1Msec(10);
    motor[mLift] = 0;
    motor[mIntake] = 127;
    goForward(6,700);
    motor[mIntake] = 0;
    goForward(12,200);
}

void autonLeft () {
	motor[mLift] = 127;
	wait1Msec(500);
	motor[mLift] = 0;
	goForward(3, 75);
	motor[mFlyWheelL] = motor[mFlyWheelL] = 127;
	wait1Msec(5000);
	motor[mIntake] = 127;
	wait1Msec(2000);
	motor[mIntake] = 0;
	goForward(9, 50);
	goForward(6, 500);
	goForward(9, 25);
	motor[mFlyWheelL] = motor[mFlyWheelR] = 0;
	goForward(9, 30);
	goForward(6, 200);



	/*
    motor[mFlyWheelL] = motor[mFlyWheelR] = 127;
    motor[mLift] = 127;
    wait1Msec(10);
    motor[mLift] = 0;
    goForward(3,100);
    wait1Msec(2000);
    motor[mIntake] = 127;
    wait1Msec(800);
    motor[mFlyWheelL] = motor[mFlyWheelR] = 0;
    goForward(3,100);
    turn(-127,1020);
    goForward(3, 50);
    goForward(6,500);
    goForward(12,475);
    motor[mIntake] = 0;
    turn(-127, 1025);
    goForward(9, 45);
    motor[mFlyWheelL] = motor[mFlyWheelR] = 127;
    wait1Msec(2000);
    goForward(6, 200);
    wait1Msec(100);
    turn(127, 20);
    motor[mIntake] = 127;
    wait1Msec(2000);
    motor[mIntake] = motor[mFlyWheelL] = motor[mFlyWheelR] = 0;
    goForward(9, 50);
    goForward(6, 500);
    goForward(12, 450);
    /*
     goForward (12, 200);
     motor[mIntake] = 127;
     goForward (12, 200);
     wait1Msec(700);
     motor[mIntake] = 0;
     goForward (6, 100);
     turn(127,-100);
     goForward(300);
     motor[mIntake] = 127;
     motor[mFlyWheelL] = motor[mFlyWheelR] = 127;
     wait1Msec(3500);
     motor[mIntake]= 0;
     motor[mFlyWheelL] = motor[mFlyWheelR] = motor[mIntake] = 0;
     goForward (12,300);
     */
}
void autonRight() {
    motor[mFlyWheelL] = motor[mFlyWheelR] = 127;
    motor[mLift] = 127;
    wait1Msec(10);
    motor[mLift] = 0;
    goForward(9,100);
    wait1Msec(2000);
    motor[mIntake] = 127;
    wait1Msec(800);
    motor[mFlyWheelL] = motor[mFlyWheelR] = 0;
    goForward(9,100);
    turn(127,1020);
    goForward(3, 50);
    goForward(6,500);
    goForward(12,475);
    motor[mIntake] = 0;
    turn(-127, 1025);
    goForward(3, 45);
    motor[mFlyWheelL] = motor[mFlyWheelR] = 127;
    wait1Msec(2000);
    goForward(6, 200);
    wait1Msec(100);
    turn(-127, 20);
    motor[mIntake] = 127;
    wait1Msec(2000);
    motor[mIntake] = motor[mFlyWheelL] = motor[mFlyWheelR] = 0;
    goForward(3, 50);
    goForward(6, 500);
    goForward(12, 450);
    /*
     goForward (12, 200);
     motor[mIntake] = 127;
     goForward (12, 200);
     wait1Msec(700);
     motor[mIntake] = 0;
     goForward (6, 100);
     turn(127,-100);
     goForward(300);
     motor[mIntake] = 127;
     motor[mFlyWheelL] = motor[mFlyWheelR] = 127;
     wait1Msec(3500);
     motor[mIntake]= 0;
     motor[mFlyWheelL] = motor[mFlyWheelR] = motor[mIntake] = 0;
     goForward (12,300);
     */
}
void auton() {
    //goForward (3, 4000);
    //turn(127, 1200);
    //return;
    if (SensorValue[jumper] == 0) {
        if(SensorValue[potSelect] < 1150) {
            autonLeft();
        } else if (SensorValue[potSelect] > 2650){
            autonRight();
        } else {
            autonBack();
        }


    }
    // else if (SensorValue[potAuton] > 2700) {
    //    autonRight();
    //}
}


task autonomous() {
	auton();
}

// USER CONTROL


int flyWheelSpeed = 0;
int flyWheelSpeed_Memory = __FLYWHEEL_SECONDARY_SPEED;

float driveFactor = 1.0;
float FL, FR, BL, BR, headflip = 1.0;

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
			}
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


int curClawPos;



bool clawAssistOn = false, clawInManualControl = false;
task clawControl(){  //basic motor control by 2 buttons
	while(true){
		if(vexRT[Btn7R]){
			motor[mClaw] = 50;
			} else if (vexRT[Btn7L]) {
			motor[mClaw] = -50;
			} else {
			motor[mClaw] = 0;
		}
		EndTimeSlice();
	}
}

task SmartLift(){
	int bool stay=true;
	int pos=SensorValue[potLift];
	while(true){
		int pos1=Sensorvalue[potLift];
		wait1Msec(10);
		int pos2=Sensorvalue[potLift];
		float v=(pos1-pos2)
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

			motor[mLift] =0.2*(pos-SensorValue[potLift])+v;
			if (vexRT[Btn7U]){
				pos=2675
			}
			if (vexRT[Btn7D]){
				pos=1236
				motor[mClaw]=-60;
				wait1Msec(100);
				motor[mClaw]=0;
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
		if (vexRT[Btn8R]) { // flip head
			while (vexRT[Btn8R]) { wait1Msec(10);}
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
	while (true) {
		EndTimeSlice();
	}
}
/*
task rebootDetect(){
	while(true){
		if(vexRT[7U] && vexRT[7D] && vexRT[


		*/
