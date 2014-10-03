///////////////////////////////////////////////////////////////////////////////
//
//                    Digital Sensor I2C Test Program
//
//
// This is a quick and simple program to test out the initial implementation of
// I2C messaging on the NXT in ROBOTC
//
// The program illustrates a user written device driver for the Lego Sonar sensor.
// The driver is very simple. It loops forever in a state machine generating
// messages to the SONAR sensor . The state machine has the following states
//
// 1. Reset
//        - performs a warm reset
//
// 2. Initializaton.
//         - sends a initialization message to the SONAR sensor
//
// 3. Set interval.
//        - waits for I2C idle signal indicating sensor has been initialized.
//
// 4. Start polling.
//        - send a poll request to the SONAR sensor
//        - waits for the reply from the polling value.
//        - updates the "sensorValue" so that it can be used by other programs
//        - re-enters the polling state
///////////////////////////////////////////////////////////////////////////////


// #define NXT2excelMaxData 3

// #include "NXT2excel.c";




const tSensors kTestPort = S2;


const int kDelayBForHWStartup  = 15;
static int kDelayBetweenPolls  =  12;
const int kSonarReplySize      = 1;    // 8 for single shot mode, 1 for continuous
ubyte replyMsg[kSonarReplySize];
bool resetAfterEveryRead=false;




task SonarDeviceDriver()
{
  //
  // Message constants for Lego Sonar (Ultra Sonic) sensor
  //
  const ubyte kSonarID               = 2;
  const ubyte kContinuousMeasurement = 0x02;  // 0x01 for single shot mode, 0x02 for continuous measurement
  const ubyte kInitializeSonar       = 0x41;
  const ubyte kReadSonarResult       = 0x42;
  const ubyte kReset                 = 0x04;  // for a warm reset
  const ubyte kSetInterval           = 0x40;  // sets the interval at continuous measurement
  const ubyte kInterval              = 0x01;  // The interval

  //
  // I2C Message definition for SONAR sensor
  //
  static const byte kSonarInitialize[4] = {3, kSonarID, kInitializeSonar, kContinuousMeasurement};
  static const byte kSonarReset[4]      = {3, kSonarID, kInitializeSonar, kReset};
  static const byte kSonarInterval[4]   = {3, kSonarID, kSetInterval, kInterval};
  static const byte kSonarRead[3]       = {2, kSonarID, kReadSonarResult};

  //
  //  state machine for SONAR "device driver"
  //
  typedef enum
  {
  	stateInitializing,
  	stateWaitForInitDone,
  	stateSendPollMessage,
  	stateWaitingForReply,
  	stateResetting,
  	stateWaitForReset,
  	stateSettingInterval,
  	stateWaitForSettingInterval
  } TSonarState;

  static TSonarState nSonarState;
  long lastTime;
  long interval;


  //
  // Set up port 'kTestPort' as user defined I2C sensor
  //

  nI2CRetries = 0;
  SensorType[kTestPort] = sensorI2CCustom9V;
	nSonarState = stateResetting;
  while(true)
  {
  	//
  	// Loop forever, polling the SONAR sensor
  	//
  	switch (nSonarState)
	  {
	  case stateResetting:
	  	//
	  	// Send rest message to Sonar sensor
	  	//
	  	sendI2CMsg(kTestPort, kSonarReset[0], 0);
	  	nSonarState = stateWaitForReset;
	  	break;

	  case stateWaitForReset:
	  	//
	  	// Wait for Sonar sensor warm reset to complete
	  	//
	  	switch (nI2CStatus[kTestPort])
	  	{
  		case NO_ERR:
		  	nSonarState = stateInitializing;
		  	wait1Msec(kDelayBForHWStartup);
		  	break;

  		case STAT_COMM_PENDING:
  			// Keep waiting for reply. I2C messaging is not complete
  			wait1Msec(1);
  			break;

  		default:
  		case (TI2CStatus) ERR_COMM_BUS_ERR:
		  	// re-initialize sensor. An I2C messaging error occurred.
     		PlaySound(soundException);
		  	nSonarState = stateResetting;
		  	break;
			}
			break;

	  case stateSettingInterval:
	  	//
	  	// Send initialize message to Sonar sensor
	  	//
	  	sendI2CMsg(kTestPort, kSonarInterval[0], 0);
	  	nSonarState = stateWaitForSettingInterval;
	  	break;

	  case stateWaitForSettingInterval:
	  	//
	  	// Wait for Sonar sensor initialization to complete
	  	//
	  	switch (nI2CStatus[kTestPort])
	  	{
  		case NO_ERR:
		  	nSonarState = stateSendPollMessage;
		  	wait1Msec(kDelayBForHWStartup);
		  	break;

  		case STAT_COMM_PENDING:
  			// Keep waiting for reply. I2C messaging is not complete
  			wait1Msec(1);
  			break;

  		default:
  		case (TI2CStatus) ERR_COMM_BUS_ERR:
		  	// re-initialize sensor. An I2C messaging error occurred.
     		PlaySound(soundException);
    		nSonarState = stateResetting;
		  	break;
			}
			break;


			case stateInitializing:
	  	//
	  	// Send initialize message to Sonar sensor
	  	//
	  	sendI2CMsg(kTestPort, kSonarInitialize[0], 0);
	  	nSonarState = stateWaitForInitDone;
	  	break;

	  case stateWaitForInitDone:
	  	//
	  	// Wait for Sonar sensor initialization to complete
	  	//
	  	switch (nI2CStatus[kTestPort])
	  	{
  		case NO_ERR:
		  	nSonarState = stateSettingInterval;
		  	wait1Msec(kDelayBForHWStartup);
		  	break;

  		case STAT_COMM_PENDING:
  			// Keep waiting for reply. I2C messaging is not complete
  			wait1Msec(1);
  			break;

  		default:
  		case (TI2CStatus) ERR_COMM_BUS_ERR:
		  	// re-initialize sensor. An I2C messaging error occurred.
     		PlaySound(soundException);

		  	nSonarState = stateResetting;
		  	break;
			}
			break;

	  case stateSendPollMessage:
	  	//
	  	// Ready to send next polling message to Sensor
	  	//
	  	nI2CBytesReady[kTestPort] = 0; // Clear any ending bytes
	  	sendI2CMsg(kTestPort, kSonarRead[0], kSonarReplySize);
	  	nSonarState = stateWaitingForReply;
	  	break;


	  case stateWaitingForReply:
	  	//
	  	// Wait for reply from SONAR sensor
	  	//
	  	switch (nI2CStatus[kTestPort])
	  	{
  		case NO_ERR:
		  	// Reply from SONAR sensor is ready to be read.
		  	//    - Update sensor value
		  	//    - Setup to send next polling message.


		  	//
		  	// Read the SONAR value and store it in "SensorValue" array so that it can be used
		  	// by other tasks as a standard sensor.
		  	//
   	  	memset(replyMsg,0x00,kSonarReplySize);
		  	readI2CReply(kTestPort, replyMsg[0], kSonarReplySize);
		  	SensorValue[kTestPort]=replyMsg[0];
		  	nSonarState = stateSendPollMessage;
		  	if (resetAfterEveryRead) nSonarState= stateResetting;
        interval=nSysTime-lastTime;
        lastTime=nSysTime;
		  	//
		  	// Sonar hardware delay. If the next poll request is immediately sent to
		  	// the SONAR sensor, then the message gets corrupted and generates a
		  	// 'ERR_COMM_BUS_ERR' error. So we'll put a little delay in here. I'm not
		  	// certain if this is a problem in the sonar sensor or a problem in the
		  	// bit-banged firmware trying to do too many things in one timeslice or
		  	// a problem in the ROBOTC firmware. The error message is that when
		  	// transmitting the next polling I2C message an ack is not received by the
		  	// the ARM processor after transmitting a byte of the message. Suspicion
		  	// is that this is bug in SONAR sensor.
		  	//
		  	// We really only need to wait a few msec to fix above problem. But might as
		  	// well go ahead and wait longer because sonar pulses are only sent out on a
		  	// 50 to 80 period.
		  	//
		  	if (kDelayBForHWStartup != 0)
					wait1Msec(kDelayBetweenPolls);
		  	break;

  		case STAT_COMM_PENDING:
  			// Insert short wait for reply. I2C messaging xmit/rcv is not complete
  			wait1Msec(1);
  			break;

  		default:
  		case (TI2CStatus) ERR_COMM_BUS_ERR:
		  	// re-initialize sensor. An I2C messaging error occurred.
  		  PlaySound(soundException);
		  	nSonarState = stateResetting;
  			wait1Msec(2);
		  	break;
			}
			break;
		}
	}
	return;
}

///////////////////////////////////////////////////////////////////////////////
//
//                                 task main()
//
// Multi-tasking illustration of user written I2C device driver. The task
// "SonarDeviceDriver" is the device driver.
//
// The "main" task is simply a copy of the "try me" program for SONAR sensor
// with the addition of one line to start the device driver task.
//
///////////////////////////////////////////////////////////////////////////////

task main()
{
	//
	// Start the SONAR device driver
	//
	StartTask(SonarDeviceDriver);
	eraseDisplay();
 // Variables declaration
  long lngCurrentDistance = 0;
  long lngPreviousDistance = 0;
  long lngCurrentTime = 0;
  long lngPreviousTime = 0;
  long lngTimeUpdate = 0;


  // Main loop
  while (true)
  {
    lngCurrentDistance = SensorValue[kTestPort];
    lngCurrentTime = nSysTime;
    if (lngCurrentDistance != lngPreviousDistance)
    {
      lngTimeUpdate = lngCurrentTime - lngPreviousTime;
      lngPreviousTime = lngCurrentTime;
      lngPreviousDistance = lngCurrentDistance;
    }

     // NXT2excelAddValue(0,2);
     // NXT2excelAddValue(SensorValue[kTestPort],3);
     // NXT2excelSendMessage();
  }
}
