//INCLUDE THIS CODE BY USING #include "UsefulFunctions.c"


//////Drive examples
// TO DRIVE STRAIGHT @ 50 power
// drive(50, 0);
// TO DRIVE STRAIGHT IN REVERSE @ 50 power
// drive(-50, 0);
// TO DRIVE WITH A MEDIUM VEER @ 50 power
// drive(50, 50); //Goes Left (I think)
// drive(50, -50); //Goes Right (I think)
// TO DRIVE WITH ONE TIRE MOVING
// drive(50, 100); //Goes Left (I think)
// drive(50, -100); //Goes Right (I think)
// TO TURN IN PLACE
// drive(50, 200); //TURNS IN PLACE TO THE LEFT
// drive(50, -200); //TURNS IN PLACE TO THE RIGHT

//TODO: Create a task that accelerates the motors to the desired power
void drive(int power, float steer){//function to drive with a certain speed while steering.
	float steerF = steer/100;
	int left = power;
	int right = power;
	if(steer==0){
		//do nothing
	} else if(steer>0 && power>0){
		//subtract from right
		right -= (int)(power*steerF);
	} else if(steer<0 && power>0){
		//subtract from left
		left -= (int)(power*abs(steerF));
	} else if(steer>0 && power<0){
		//subtract from right
		right += (int)(power*steerF);
	} else if(steer<0 && power<0){
		//subtract from left
		left += (int)(power*abs(steerF));
	}
	motor[motorLeft] = left;
	motor[motorRight] = right;
}