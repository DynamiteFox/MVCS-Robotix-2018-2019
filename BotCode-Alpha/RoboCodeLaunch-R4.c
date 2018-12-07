#pragma config(Sensor, in2,    Claw,           sensorPotentiometer)
#pragma config(Sensor, dgtl1,  LED,            sensorLEDtoVCC)
#pragma config(Sensor, dgtl4,  configLED,      sensorLEDtoVCC)
#pragma config(Sensor, dgtl2,  quadLift,       sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  encBL,          sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  encBR,          sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  encFL,          sensorQuadEncoder)
#pragma config(Sensor, dgtl11, encFR,          sensorQuadEncoder)
#pragma config(Motor,  port1,           mNOT,          tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           mBL,           tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           mLFT,          tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           mGRB,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           mFL,           tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           mFR,           tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           mCLW,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           mFRE,          tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           mFRE2,         tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          mBR,           tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define speed1 70
#define flywheelSensorThreshhold 300

//Potentiometer Positions for Cascade Lift Arm. Multiply Values by 10
#define Lifted 235
#define gndFlip 200
#define Down  60

//Encoder Values for Lift Hieghts. Multiply Values by 10
#define liftBottom 0
#define liftMiddle 130
#define liftTop 2200
#define removeMod 50


//Auton Constants

#define driveTolerance 10;



//----------------------Controller Action Defnintions--------------------

//Drivetrain
#define flipHead rmt(L7)

//Flywheels
#define intakeI (rmt(U6) && !rmt(D6))
#define intakeO (rmt(D6) && !rmt(U6))
#define toggleFly (rmt(D8) && !(rmt(D6) && rmt(U6)))
#define onFull (rmt(U8) && !(rmt(U6) && rmt(D6)))
#define secondarySpeed rmt(L8)
#define memSpeed rmt(R8)
#define speedInc (rmt(U8) && (rmt(U6) && rmt(D6)))
#define speedDec (rmt(D8) && (rmt(D6) && rmt(U6)))
#define speedReset (rmt(U8) && rmt(D8) && (rmt(D6) && rmt(U6)))

//Cascade Lift
#define cascadeU (rmt(U5) && !rmt(D5))
#define cascadeD (rmt(D5) && !rmt(U5))
#define clawU (rmt(D5) && rmt(U5) && rmt(U7))
#define clawD (rmt(D5) && rmt(U5) && rmt(D7))
#define clawToggle (rmt(R7))
#define cascadeTop (cascadeU && rmt(U7))
#define groundFlip (rmt(U7) && !(rmt(D5) && rmt(U5)) && !cascadeTop)
#define cascadeDown (rmt(D7) && !(rmt(D5) && rmt(U5)))

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

#include "RoboCode-Core.h"

//Function Declaration



/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/
void initMotor(driveMotor that, tMotor mot, tSensors senso){
	that.motorm = mot;
	that.encoder = senso;
    SensorValue[senso] = 0;
    that.speed = 0;
    that.target = 0;
    that.topspeed = 127;
}
void pre_auton()
{
	initMotor(FL, mFL, encFL);
	initMotor(BL, mBL, encBL);
	initMotor(BR, mBR, encBR);
	initMotor(FR, mFR, encFR);
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
    //functions for programing auton:
    /*
     translate (x,y); Translates the robot relative to the robot's Head.
     rotate (turns); rotates robot around center.
     (liftOff)  boolean variable indicating if lift is not in position
     (armOff) boolean variable indicating if arm is not in position
     (liftTarget) integer value indicating target position of lift
     (armTarget) integer value indicating target position of arm
     (flywheelRunning) Boolean variable indicating wether or not flywheel is on
     (flySpeed) integer variable indicating flywheel motor speed.
     (intake) Boolean variable toggling intake on/off
     */
     
     
     
     
  AutonomousCodePlaceholderForTesting();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol()
{
	wait1Msec(500);
    turnLEDOn(configLED);
    wait1Msec(1500);

    /*------------------------------------------*/
    /*      Insert Pre Config Options Here      */
    /*------------------------------------------*/

    liftClawControl = vexRT[Btn7U] == 1;


    /*------------------------------------------*/
    turnLEDOff(configLED);
  Start();
}
