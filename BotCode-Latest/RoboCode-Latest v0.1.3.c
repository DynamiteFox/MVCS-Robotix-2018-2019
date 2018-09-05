#pragma config(Motor,  port1,           mLFT2,         tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           mBL,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           mBR,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           mGRB,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           mFRE,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           mLFT1,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           mCLW,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           mFR,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           mFL,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          mBST,          tmotorVex393_HBridge, openLoop)
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



// ----------------------------------------------IMPORTANT---------------------------------------------
// USE SEPARATE TASKS FOR EVERYTHING. IT ACCOMPLISHES THE SAME THING AS PUTTING EVERYTHING IN MAIN,
// BUT IT'S MORE ORGANIZED AND HELPS DEBUG
//-----------------------------------------------IMPORTANT---------------------------------------------



task drivetrain(){ //Drivetrain Task. Joshua's code.
	while(true){
		motor[mFR]=(-vexRT[Ch4]+vexRT[Ch3])-vexRT[Ch1];
	  motor[mFL]=(-vexRT[Ch4]-vexRT[Ch3])-vexRT[Ch1];
	  motor[mBL]=(vexRT[Ch4]-vexRT[Ch3])-vexRT[Ch1];
	  motor[mBR]=(vexRT[Ch4]+vexRT[Ch3])-vexRT[Ch1];
	}
}


task ballGrabber(){ //Ball intake system code
	while(true){
		if (vexRT[Btn6U]){
			motor[mGRB] = 127;
	  }else if (vexRT[Btn6D]){
	  	motor[mGRB] = -127;
		} else {
			motor[mGRB] = 0;
		}
	}
}


void Start()
{
startTask(drivetrain);
startTask(ballGrabber);

}
