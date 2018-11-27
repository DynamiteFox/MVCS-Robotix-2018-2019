//RoboCode-Alpha v1.1.0


//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


/*
Port1: LiftMotor2 (Can be combined with Prt. 6 in necessity

Port2: Rear Left Drive

Port3: Rear Right Drive

Port4: Intake Motor

Port5: Front Left Drive

Port6: Front Right Drive

Port7: ClawMotor

Port8: Firing Motors (Splitterx2 + Power Expander)

Port9: LiftMotor1

Port10: Platform Boost system (?)

*/
#define mabs(a1) (a1 < 0 ? -a1 : a1)
// ----------------------------------------------IMPORTANT---------------------------------------------
// USE SEPARATE TASKS FOR EVERYTHING. IT ACCOMPLISHES THE SAME THING AS PUTTING EVERYTHING IN MAIN,
// BUT IT'S MORE ORGANIZED AND HELPS DEBUG
//-----------------------------------------------IMPORTANT---------------------------------------------

#include "controllerDefinitions.h"

//Config
#define adjustmentThreshhold 0.04

//Internal Use Variables
int FR;
int FL;
int BR;
int BL;
bool flywheelRunning;
int flySpeed = 500;
signed int flyModifier = 0;
int blinker = 0;
int revFR;
int revFL;
int revBR;
int revBL;
int armTarget;
int liftTarget;
int headFlip = 1;
int sens;
bool liftOff;
bool armOff;
bool secondary = false;
bool liftClawControl;
bool armOff;
const char assignmentOrder[4] = {'D','L','U','R'};

word rmt(char *a){ //takes advantage of some research on vexRT[]
    int z = 0;
    int b = 0;
    int j;
    switch (a[0]) {
        case 'C':
            if (secondary){
                z += 6;
            }
            z += (a[1] - '0')-1;
            break;
        default:
            b = a[0] - '0';
            if(b > 8)
                return 0;
            z += 14;
            if(secondary)
                z += 12;
            if(b <= 6){
                if (a[1] == 'U')
                    ++z;
                z += ((b-5)*2);
            } else {
                for(j = 0; !( assignmentOrder[j] == a[1]|| j == 5); ++j){
                    ++z;
                }
                if (j == 5)
                    return 0;
                z += (b == 8 ? 1 : 2)*4;
            }
            break;
    }
    return vexRT[z];
}

/*

int rmt2(char[5] s1){   //does not work. vexRT is an array(?) that takes a special variable or an integer(?)
    char result[STR_SIZE];
    if (secondary){
        snprintf(result, sizeof(result), "%s %s", a1, s2);
    } else {
        result[] = s1[];
    }
    return vexRT[result];
}


word rmt(char[2] a){    //allow remote switching (massive)
    switch (a [0]) {
        case '7':
            switch (a[1]) {

                    //Pad 7
                case 'U':
                    return secondary ? vexRT[Btn7UXmtr2] : vexRT[Btn7U];
                    break;

                case 'D':
                    return secondary ? vexRT[Btn7DXmtr2] : vexRT[Btn7D];
                    break;

                case 'L':
                    return secondary ? vexRT[Btn7LXmtr2] : vexRT[Btn7L];
                    break;

                case 'R':
                    return secondary ? vexRT[Btn7RXmtr2] : vexRT[Btn7R];
                    break;
            }
            break;

        case '8':
            switch(a[1]){
                    //Pad 8
                case 'U':
                    return secondary ? vexRT[Btn8UXmtr2] : vexRT[Btn8U];
                    break;

                case 'D':
                    return secondary ? vexRT[Btn8DXmtr2] : vexRT[Btn8D];
                    break;

                case 'L':
                    return secondary ? vexRT[Btn8LXmtr2] : vexRT[Btn8L];
                    break;

                case 'R':
                    return secondary ? vexRT[Btn8RXmtr2] : vexRT[Btn8R];
                    break;
            }
            break;

        case '6':
            switch(a[1]){
                    //triggers 6
                case 'U':
                    return secondary ? vexRT[Btn6UXmtr2] : vexRT[Btn6U];
                    break;

                case 'D':
                    return secondary ? vexRT[Btn6DXmtr2] : vexRT[Btn6D];
                    break;
            }
            break;

        case '5':
            switch(a[1]){

                    //triggers 5
                case 'U':
                    return secondary ? vexRT[Btn5UXmtr2] : vexRT[Btn5U];
                    break;

                case 'D':
                    return secondary ? vexRT[Btn5DXmtr2] : vexRT[Btn5D];
                    break;
            }
            break;

        case 'C':
            switch(a[1]){

                    //joystick channels
                case '1':
                    return secondary ? vexRT[Ch1Xmtr2] : vexRT[Ch1];
                    break;

                case '2':
                    return secondary ? vexRT[Ch2Xmtr2] : vexRT[Ch2];
                    break;

                case '3':
                    return secondary ? vexRT[Ch3Xmtr2] : vexRT[Ch3];
                    break;

                case '4':
                    return secondary ? vexRT[Ch4Xmtr2] : vexRT[Ch4];
                    break;
            }
    }
    return 0;
}


int mabs (int a) {
    return a < 0 ? -a : a;
}
*/

void applyMotorSpeed(int FRi, int BRi, int BLi, int FLi){
    float ratios [4][2];
    int *lo;
    int *loSens;

    motor[FR] = FRi;
    motor[BR] = BRi;
    motor[BL] = BLi;
    motor[FL] = FLi;
    if (FRi < BRi && FRi < BLi && FRi < FLi){
        lo = &FRi;
        loSens = &revFR;
    } else if (BRi < BLi && BRi < FLi){
        lo = &BRi;
        loSens = &revBR;
    } else if (BLi < FLi){
        lo = &BLi;
        loSens = &revBL;
    } else {
        lo = &FLi;
        loSens = &revFL;
    }
    ratios[0][0] = FRi / *lo;
    ratios[1][0] = BRi / *lo;
    ratios[2][0] = BLi / *lo;
    ratios[3][0] = FLi / *lo;
    wait1Msec(3);
    for(int i = 0; i < 5; ++i){
        ratios[0][1] = ratios[0][0] - (revFR / *loSens);
        ratios[1][1] = ratios[1][0] - (revBR / *loSens);
        ratios[2][1] = ratios[2][0] - (revBL / *loSens);
        ratios[3][1] = ratios[3][0] - (revFL / *loSens);
        if(abs(ratios[0][1]) > adjustmentThreshhold){
            motor[mFR] += 5*sgn(ratios[0][1]);
        }
        if(abs(ratios[1][1]) > adjustmentThreshhold){
            motor[mBR] += 5*sgn(ratios[1][1]);
        }
        if(abs(ratios[2][1]) > adjustmentThreshhold){
            motor[mBL] += 5*sgn(ratios[2][1]);
        }
        if(abs(ratios[3][1]) > adjustmentThreshhold){
            motor[mFL] += 5*sgn(ratios[3][1]);
        }
        wait1Msec(3);
    }
}

task controllerSwitch(){
    while(true){
        if (vexRT[Btn8RXmtr2]){
            while(vexRT[Btn8RXmtr2]){wait1Msec(1);}
            secondary  = !secondary;
        }
    }
}
task drivetrain(){ //Drivetrain Task. Joshua's code.
    while(true){
        FR = (headFlip * (-vexRT[Ch4] + vexRT[Ch3])) - vexRT[Ch1]; //Determines motor speeds. Joshua's Code.
        FL = (headFlip * (-vexRT[Ch4] - vexRT[Ch3])) - vexRT[Ch1];
        BL = (headFlip * (vexRT[Ch4] - vexRT[Ch3])) - vexRT[Ch1];
        BR = (headFlip * (vexRT[Ch4] + vexRT[Ch3])) - vexRT[Ch1];
        if (mabs(vexRT[Ch4]) < 20 && mabs(vexRT[Ch3]) < 20 && mabs(vexRT[Ch1]) < 30){ //Checks if joystick is in deadzone
            FR = FL = BR = BL = 0;                                                                                                    //if so, sets motor speeds to 0
        }
        if(sqrt((vexRT[Ch4]*vexRT[Ch4]) + (vexRT[Ch3]*vexRT[Ch3])) < 120 && sqrt((vexRT[Ch1]*vexRT[Ch1]) + (vexRT[Ch2]*vexRT[Ch2])) < 120){    //if Joystick not at extremes
            FR = FR/2;      //halfSpeed
            FL = FL/2;
            BR = BR/2;
            BL = BL/2;
            }
        motor[mFR] = FR;  //Applies Motor Speeds
        motor[mFL] = FL;
        motor[mBR] = BR;
        motor[mBL] = BL;
        if(flipHead){                           //tests for button to flip robot head
            while (vexRT[Btn8R]) { wait1Msec(1);}
            headFlip *= -1;                         //flips head
        }
        EndTimeSlice(); //tells task handler is done
    }
}

task ballGrabber(){ //Ball intake system code
    while(true){
        if (intakeI) { //if intake button pressed...
            motor[mGRB] = 127; //run intake motor
            } else if (intakeO) { //if reverse button Pressed
            motor[mGRB] = -127; // reverse
            } else {
            motor[mGRB] = 0; //else stop
        }
        EndTimeSlice(); //tell task handler done
    }
}
task LEDControl(){      //task to turn LED on & off
    while(true){
        if(flySpeed < 127){                 //if flywheel is in secondary state
            while(flySpeed < 127){          //while it's in secondary state
                if(flyModifier > 0){        //if the modifier is positive
                    blinker = 300;          //set blinking delay to 300
                } else if(flyModifier < 0){ //if modifier is negative
                    blinker = 1000;         //set delay to 1000
                } else  {                   //else (it's 0)
                    while(flyModifier == 0 && flySpeed < 127){  //while modifier is 0
                        turnLEDOn(LED);                         //keep LED on
                        EndTimeSlice();                         //tell task handler is done
                    }
                }
                turnLEDOn(LED);     //turn LED on
                wait1Msec(100);     //wait 100 ms
                turnLEDOff(LED);    //turn LED off
                wait1Msec(blinker); //wait for [blinking delay] ms
                EndTimeSlice(); //tell task handler is done
            }
        }else{              //else (flySpeed is in full power)
            turnLEDOff(LED);//turn off LED
            EndTimeSlice(); //tell task handler done
        }
    }
}


task flywheelToggle() { //detects button presses to toggle the flywheel
    while (true) {
        if (toggleFly) { //if toggle button pressed
            flywheelRunning = !flywheelRunning;  //set flywheel running bool to the opposite value
            while (toggleFly) { //wait until button un-pressed
                wait1Msec(1);
            }
        }
        if (flywheelRunning) { // if flywheel set to be on
            motor[mFRE] = flySpeed + (5*flyModifier); //turn on
            motor[port9] = flySpeed + (5*flyModifier);
            } else {
            motor[mFRE] = 0; //else turn off
            motor[port9] = 0;
        }

        EndTimeSlice(); //tell task handler done
    }
}

task flySpeedAdjuster() {       //Adjust the flywheel Speeds
    while(true){
        if (secondarySpeed) {             //if 8L pressed
            while(secondarySpeed){        //wait until not pressed
                wait1Msec(1);
            }
            switch(flySpeed){
            case 500:                   //if flySpeed = 500
                flySpeed = speed1;         //set flySpeed to lv1
                break;
            default:                    //else
                flySpeed = 500;         //set flyspeed to 500
            }
        }
        if (speedReset) {                      //if speed reset buttons
            while(speedReset){wait1Msec(1);}
            flyModifier = 0;                                    //reset modifier
        }else if (speedInc) {                               //if speed increment buttons
            while(speedInc){wait1Msec(1);}
            flyModifier++;                                      //increase modifier
        }else if (speedDec) {                               //if speed decrement buttons
            while(speedDec){wait1Msec(1);}
            flyModifier--;                                      //decrease modifier
        }
    }
}
task RPMTrack(){
	int FRi,FLi,BRi,BLi;
	while(true){
		FRi = SensorValue[encFR];       //find initial sensor values
		FLi = SensorValue[encFL];
		BLi = SensorValue[encBL];
		BRi = SensorValue[encBR];
		wait1Msec(2);                   // wait 2 ms
		revFR = 500*(FR - SensorValue[encFR]);  //determine rotational speed (Deg/s)
		revFL = 500*(FL - SensorValue[encFL]);
		revBL = 500*(BL - SensorValue[encBL]);
		revBR = 500*(BR - SensorValue[encBR]);
	}
}

task cascadeClawManual(){  //control cascade lift and claw manually
	while(true){
		if(cascadeU){
            cascadeTarget = liftTop;
            while (cascadeU) {wait1Msec(1);}
            cascadeTarget = SensorValue[quadLift]/10
		}
        if(cascadeD){
            cascadeTarget = liftBottom;
            while (cascadeD) {wait1Msec(1);}
            cascadeTarget = SensorValue[quadLift]/10
        }
        if(clawU){
            clawTarget = Lifted;
            while (cascadeU) {wait1Msec(1);}
            cascadeTarget = SensorValue[Claw]/10
        }
        if(clawD){
            clawTarget = Stowed;
            while (clawD) {wait1Msec(1);}
            clawTarget = SensorValue[Claw]/10
        }
		EndTimeSlice();
	}
}
task autoClaw(){  //handle Claw Movement

    armTarget = SensorValue[Claw]/10;  //set target to current position to prevent early movement
    while (true) {
        sens = SensorValue[Claw]/10;    //determine current claw position
        armOff = mabs(sens-armTarget) > 1
        if(armOff){   // if the current position is off of the target by at least 10
            if (sens > armTarget) {     //if the position is higher than the target
                motor[mCLW] = -127;     //set motor to go down
            } else {                    //else
                motor[mCLW] = 127;      //set motor to go up
            }
        } else {                        //if the current position is not off
            motor[mCLW] = 0;            //stop the claw
        }

        EndTimeSlice(); //tell task handler done
    }
}
task autoLift(){ //Task to move Lift into postition
    int sensLift;
    SensorValue[quadLift] = 0;                  //initialize the quad encoder to 0;
    liftTarget = SensorValue[quadLift]/10;      //set target to current position to prevent unwanted movement
    while (true) {
        sensLift = SensorValue[quadLift]/10;    // determine current lift position
        liftOff = mabs(sensLift-liftTarget) > 1;
        if(liftOff){                            //if lift is off the target by at least 10
            if (sensLift > liftTarget) {        //if the lift is above the target
                motor[mLFT] = -127;             //move lift down
            } else {                            //else (the lift is below the target)
                motor[mLFT] = 127;              // move lift up
            }
        } else {                                //else (lift is not off the target by at least 50)
            motor[mLFT] = 0;                    //stop the lift
        }

        EndTimeSlice();            //tell task handler done
    }
}

task liftClawInterfaceSimple(){
    while (true) {
        if (clawToggle) {                         //if button for claw pressed
            while (clawToggle) {wait1Msec(1);}     //wait until un-pressed
            switch (armTarget) {                    //switch case:
                case Down:                          //if target is set to down position
                    armTarget = Lifted;             //set target to lifted position
                    break;
                    
                default:                            //else
                    armTarget = Down;               //set target to down position
                    wait1Msec(500);
                    liftTarget = liftBottom;
                    break;
            }
        }
        if(cascadeDown){
            liftTarget = liftBottom
        }
        if(groundFlip){
            while(groundFlip){wait1MSec(1);}
            armTarget = lifted/2;
            while(armOff){wait1MSec(1);}
            armTarget = Down;
        }
        EndTimeSlice();
    }
}
task liftClawControllerInterface(){
    startTask(autoLift);  //ensure position monitors already running
    startTask(autoClaw);
    while (true) {
        if (vexRT[Btn7D]) {                         //if button for lift pressed
            while (vexRT[Btn7D]) {wait1Msec(1);}     //wait until button un-pressed
            switch (liftTarget) {                   //switch case:
                case liftBottom:                    //if target is set to bottom
                    liftTarget = liftMiddle;        //set target to middle
                    break;

                case liftMiddle:                    //if target set to middle
                    liftTarget = liftTop;           //set target to top
                    break;

                default:                            //else
                    liftTarget = liftBottom;        //set target to bottom
                    break;
            }
        }
        if (vexRT[Btn7L]) {                         //if button for claw pressed
            while (vexRT[Btn7L]) {wait1Msec(1);}     //wait until un-pressed
            switch (armTarget) {                    //switch case:
                case Down:                          //if target is set to down position
                    armTarget = Lifted;             //set target to lifted position
                    break;

                default:                            //else
                    armTarget = Down;               //set target to down position
                    wait1Msec(500);
                    liftTarget = liftBottom;
                    break;
            }
        }
        if(vexRT[Btn7R] && liftTarget < 190 && liftTarget > 50 && clawTarget == Down){    //if cap flip activated and lift is in position
            while(vexRT[Btn7R]){wait1Msec(1);}
            liftTarget += removeMod;                //move lift up my predetermined value
            do{wait1Msec(2);} while (liftOff);      //wait until in position
            armTarget = Lifted;                     //set move arm to up position
            liftTarget += 10;                       //move lift up a little more
            do{wait1Msec(2);}while(liftOff);        //wait until in position
            liftTarget += -(removeMod+10);          //move lift back down
            do{wait1Msec(2);}while(liftOff);        //wait unti in position
            armTarget = Down;                       //move arm down
            wait1Msec(500);                         //wait 500 ms
            liftTarget = liftBottom;                //move lift down
        }
        EndTimeSlice();
    }
}


void Start()
{
    startTask(drivetrain);
    startTask(ballGrabber);
    flywheelRunning = false;
    startTask(flywheelToggle);
    startTask(flySpeedAdjuster);
    if(liftClawControl){
        startTask(liftClawControllerInterface);
    } else {
        startTask(liftClawInterfaceSimple);
    startTask(cascadeClawManual);
	startTask(autoLift);
	startTask(autoClaw);
	startTask(LEDControl);
}
