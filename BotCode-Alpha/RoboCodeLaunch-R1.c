#pragma config(Sensor, in1,    firedBallSensor, sensorNone)
#pragma config(Sensor, in2,    Claw,           sensorPotentiometer)
#pragma config(Sensor, dgtl1,  LED,            sensorLEDtoVCC)
#pragma config(Sensor, dgtl2,  configLED,      sensorLEDtoVCC)
#pragma config(Sensor, dgtl3,  quadLift,       sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  encFR,          sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  encBR,          sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  encBL,          sensorQuadEncoder)
#pragma config(Sensor, dgtl11, encFL,          sensorQuadEncoder)
#pragma config(Motor,  port1,           mFL,           tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           mBL,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           mBR,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           mGRB,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           mLFT,          tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           mFR,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           mCLW,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           mFRE,          tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           mLFT1,         tmotorVex393HighSpeed_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define speed1 70;
#define flywheelSensorThreshhold 300;

//Potentiometer Positions for Cascade Lift Arm. Multiply Values by 10
const int Lifted =  200;
const int Down  = 60;

//Encoder Values for Lift Hieghts. Multiply Values by 10
const int liftBottom = 0;
const int liftMiddle = 130;
const int liftTop = 200;
const int removeMod = 50;

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

#include "RoboCode-Alpha.h"



/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
  bStopTasksBetweenModes = true;
    wait1Msec(500);
    turnLEDOn(configLED);
    wait1Msec(1500);

    /*------------------------------------------*/
    /*      Insert Pre Config Options Here      */
    /*------------------------------------------*/

    liftClawControl = vexRT[btn7U] == 1


    /*------------------------------------------*/
    turnLEDOff(configLED);

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
  // ..........................................................................
  // Insert user code here.
  // ..........................................................................

  // Remove this function call once you have "real" code.
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
  Start();
}
