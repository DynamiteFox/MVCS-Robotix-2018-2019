#pragma config(Sensor, in2,    potClaw,        sensorPotentiometer)
#pragma config(Sensor, in3,    potAuton,       sensorPotentiometer)
#pragma config(Sensor, dgtl1,  encLift,        sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  LED,            sensorLEDtoVCC)
#pragma config(Sensor, dgtl5,  encFR,          sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  encBL,          sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  encBR,          sensorQuadEncoder)
#pragma config(Sensor, dgtl11, encFL,          sensorQuadEncoder)
#pragma config(Motor,  port1,           mFL,           tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           mBL,           tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           mBR,           tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           mIntake,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           mLift,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           mFR,           tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           mClaw,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           mFlyWheelR,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           mFlyWheelL,    tmotorVex393HighSpeed_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


void moveClawUp (int toPos) {
    clearTimer(T1); // set timer in order to prevent burning out motors
    while (SensorValue[potClaw] < toPos && time1[T1] < 3000) {
        motor[mClaw] = 127;
    }
    motor[mClaw] = 0;
}
void moveClawDown (int toPos) {
    clearTimer(T1); // set timer in order to prevent burning out motors
    while (SensorValue[potClaw] > toPos && time1[T1] < 3000) {
        motor[mClaw] = -77;
    }
    motor[mClaw] = 0;
}


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
    encoderValues[0] = abs(SensorValue[encFL] * encListDrive[direction][0]);
    encoderValues[1] = abs(SensorValue[encFR] * encListDrive[direction][1]);
    encoderValues[2] = abs(SensorValue[encBL] * encListDrive[direction][2]);
    encoderValues[3] = abs(SensorValue[encBR] * encListDrive[direction][3]);
    encLeft = (encoderValues[mListDrive[direction][0]] + encoderValues[mListDrive[direction][2]]) / 2;
    encRight = (encoderValues[mListDrive[direction][1]] + encoderValues[mListDrive[direction][3]]) / 2;
}

const int encLeftRatio = 60, encRightRatio = 75;

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
        if (direction == 12 ? abs(encLeft * encLeftRatio - encRight * encRightRatio) : abs(encLeft - encRight) > 30) {
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

void autonLeft () {
	goForward (6, 100);
    turn(127, 20);
    motor[mClaw] = -127;
    wait1Msec(400);
    motor[mClaw] = 0;
    wait1Msec(400);
    goForward (12, 875);
    motor[mFlyWheelL] = motor[mFlyWheelR] = 127;
    goForward (6, 430);
    //500
    goForward (9, 90);
    motor[mIntake] = -127;
    wait1Msec(2500);
    motor[mFlyWheelL] = motor[mFlyWheelR] = motor[mIntake] = 0;
    goForward (12, 65);
    turn(127, 1150);
    //1300
    motor[mIntake] = 127;
    goForward (12, 350);
    motor[mIntake] = 0;
    goForward (12, 150);
    goForward (3, 350);
    goForward (12, 300);
    goForward (3, 50);
    goForward (12, 50);
}

void autonRight () {

    goForward (6, 100);
    //turn(-127, 60);
    motor[mClaw] = -127;
    wait1Msec(400);
    motor[mClaw] = 0;
    wait1Msec(400);
    goForward (12, 860);
    motor[mFlyWheelL] = motor[mFlyWheelR] = 127;
    goForward (6, 430);
    goForward (3, 110);
    motor[mIntake] = -127;
    wait1Msec(2500);
    motor[mFlyWheelL] = motor[mFlyWheelR] = motor[mIntake] = 0;
    goForward (12, 65);
    turn(-127, 1000);
    motor[mIntake] = 127;
    goForward (12, 350);
    motor[mIntake] = 0;
    goForward (12, 150);
    goForward (9, 420);
    goForward (12, 150);
    //goForward (9, 80);
    //goForward (12, 50);
}

void auton() {
    //turn(127, 1200);
    //return;
    if (SensorValue[potAuton] < 1500) {
        autonLeft();
    } else if (SensorValue[potAuton] > 2700) {
        autonRight();
    }
}
/*
task main() {
    auton();

}
*//
