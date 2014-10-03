#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorNormal, PIDControl, reversed, encoder)

//                  TETRIX nMotorEncoderTarget Example
//
// The program below demonstrates how to use the nMotorEndocerTarget function
// There are four steps to using the function
//  Step 1 - Zero out the encoder using "nMotorEncoder(motorX) = 0"   Lines 27 and 28.
//  Step 2 - Set the target distance using the nMotorEncoderTarget function.
//           Lines 30 and 31.
//  Step 3 - Turn the motors on. Lines 33 and 34.
//  Step 4 - Wait for the motor's "runstate" to reach idle. Line 34 through 38.
//
//  nMotorTargetEncoder is coupled with the nMotorRunState function.
//    The nMotorRunState function watches the "state" of the motor in three conditions:
//       1. "runStateRunning" - While the motor is running
//       2. "runStateHoldPosition" - While the motor is begining to slow down.
//          nMotorEncoderTarget sets the condition when the motor begins to slow down.
//       3. "runStateIdle" - Reports back to the controller that the motor is in an
//           idle position; this is when the motor is stopped.


task main()
{

  nMotorEncoder[motorE] = 0;  //clear the TETRIX motor encoders
  nMotorEncoder[motorD] = 0;

  nMotorEncoderTarget[motorE] = 720; //set the target stoping position
  nMotorEncoderTarget[motorD] = 720;

  motor[motorE] = 2; //turn both motors on at 30 percent power
  motor[motorD] = 2;

  while (nMotorRunState[motorE] != runStateIdle || nMotorRunState[motorD] != runStateIdle) //while the encoder wheel turns one revolution
  {
    // This condition waits for motors D & E to come to an idle position. Both motors stop
    // and then jumps out of the loop
  }

  motor[motorE] = 0; //turn both motors off
  motor[motorD] = 0;

  wait1Msec(3000);  // wait 3 seconds to see feedback on the debugger screens
                    // open the "NXT Devices" window to see the distance the encoder spins.
                    // the robot will come to a stop at the nMotorEncoderTarget position.
}
