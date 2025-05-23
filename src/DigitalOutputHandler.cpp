//--------------------------------------------------------------------------------------------------
// DigitalOutputHandler.cpp
//
//
// Main code for the project.
//
// Authors:
//            Khanh Nguyen
//            Mike Schoonover
//
// Date of project start: 2025/01/17
//
//--------------------------------------------------------------------------------------------------

#include "DigitalOutputHandler.h"

//#define DEBUG_VERBOSE_DIGITAL_OUTPUT_HANDLER	// displays extra debugging messages on serial port

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// class DigitalOutputHandler
//
/**
 *
 * This class controls a digitial output pin. The output can be turned ON or OFF, or pulsed
 * for a set time duration. Also, the output can be pulsed with a specific sequence of on/off times.
 *
 * This class is asynchronous and does NOT use the delay() function for timing. It uses a
 * DelayHandler object for timing instead.
 *
 * The DigitalOutputHandler::process function must be called repeatedly and often while a pulse
 * or pulse sequence is active so that it can be turned off when the timer or sequence transitions.
 *
 */

//--------------------------------------------------------------------------------------------------
// DigitalOutputHandler::DigitalOutputHandler (constructor)
//
/**
 *
 * Class constructor.
 *
 * The ONLY thing that should be done here is initializing objects and variables and storing
 * parameters to class variables.
 *
 * @param pObjectName       the name for this object
 * @param pObjectNumber     an identifying number for this object
 *
 */

DigitalOutputHandler::DigitalOutputHandler(const int pIOPinNum, const String pObjectName,
                                                                         const int pObjectNumber) :
    objectName(pObjectName),
    objectNumber(pObjectNumber),
    ioPinNum(pIOPinNum),
    durationMS1(0),
    durationMS2(0),
    durationMS3(0),
    stateMachine(MODE_IDLE),
    pulseDelayHandler(PULSE_DELAY_LIMIT_MS, "Pulse Delay Handler",PULSE_DELAY_HANDLER_OBJECT_NUMBER)
{

    className = "  DigitalOutputHandler:" + objectName;

    Serial.print(className); Serial.println(": constructor called...");

}// end of DigitalOutputHandler::DigitalOutputHandler (constructor)
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// DigitalOutputHandler::init
//
/**
 *
 * Initializes the object...call immediately after instantiation.
 *
 */

void DigitalOutputHandler::init()
{

    Serial.print(className); Serial.println(": init called...");

    pinMode(ioPinNum, OUTPUT);

}// end of DigitalOutputHandler::init
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// DigitalOutputHandler::setState
//
/**
 *
 * Turns the digital output on or off.
 *
 * @param pState    the state to which the output is to be set: 0 (off) or 1 (on)
 *
 */

 void DigitalOutputHandler::setState(const int pState)
 {

    state = pState;

    digitalWrite(ioPinNum, state);

 }// end of DigitalOutputHandler::setState
 //--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// DigitalOutputHandler::startOnPulse
//
/**
 *
 * Turns the digital output ON for the specified time duration and then turns the output OFF.
 *
 * Calling certain other functions in this class before (mode == MODE_IDLE) may result in unexpected
 * behavior.
 *
 * @param pDurationMS       the time duration in milliseconds for which output is to be ON
 *
 */

 void DigitalOutputHandler::startOnPulse(const int pDurationMS)
 {

    Serial.println("LED on pulse started...");
    setState(1);
    pulseDelayHandler.start(pDurationMS);

    mode = MODE_HANDLE_ON_PULSE;

 }// end of DigitalOutputHandler::startOnPulse
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// DigitalOutputHandler::startPulseSequence
//
/**
 *
 * Invokes a series of On/Off pulses for specified time durations.
 *
 * Calling certain other functions in this class before (mode == MODE_IDLE) may result in unexpected
 * behavior.
 *
 * NOTE:
 *
 * The final OFF duration might seem superflous, but client code can check if (mode == MODE_IDLE)
 * during that delay before changing the state of the output...this ensures that the last specified
 * delay is completed.
 *
 *
 * wip mks:
 *
 * This function needs to be changed to accept a variable number of parameters to allow for
 * a variable length of the sequence.
 *
 * @param pOnDurationMS1        time duration of first ON period
 * @param pOffDurationMS1       time duration of first OFF period
 * @param pOnDurationMS2        time duration of second ON period
 * @param pOffDurationMS2       time duration of second OFF period
 * @param pOnDurationMS3        time duration of third ON period
 * @param pOffDurationMS3       time duration of third OFF period
 *
 */

void DigitalOutputHandler::startPulseSequence(
    const int pOnDurationMS1, const int pOffDurationMS1,
    const int pOnDurationMS2, const int pOffDurationMS2,
    const int pOnDurationMS3, const int pOffDurationMS3)
{

    onDurationMS1 = pOnDurationMS1; offDurationMS1 = pOffDurationMS1;
    onDurationMS2 = pOnDurationMS2; offDurationMS2 = pOffDurationMS2;
    onDurationMS3 = pOnDurationMS3; offDurationMS3 = pOffDurationMS3;

    Serial.println("Starting pulse sequence...");

    stateMachine = STATE_START_SEQUENCE;

    mode = MODE_HANDLE_SEQUENCE;

}// end of DigitalOutputHandler::startPulseSequence
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// DigitalOutputHandler::process
//
/**
 *
 * This function should be called by the Arduino 'loop' function which is in the .ino file for
 * Arduino IDE projects and in the main.cpp file for VSCode projects.
 *
 * This function handles various ongoing processes such as a pulse or pulse sequence by turning
 * off or changing the output state at predetermined times.
 *
 *
 */

 void DigitalOutputHandler::process()
 {

     switch(mode){

         case MODE_IDLE:

             return;

             break;

         case MODE_HANDLE_ON_PULSE:

             handleOnPulse();

             break;

         case MODE_HANDLE_SEQUENCE:

             handlePulseSequence();

             break;

     }

 }// end of DigitalOutputHandler::process
 //--------------------------------------------------------------------------------------------------

 //--------------------------------------------------------------------------------------------------
 // DigitalOutputHandler::handleOnPulse
 //
 /**
  *
  * Ends an ON pulse when the timer ends.
  *
  * This function should be called frequently when a pulse is in progress. It is usally called by
  * the process fuction, so the process function should be called frequently by the client code.
  *
  */

  void DigitalOutputHandler::handleOnPulse()
  {

     if(pulseDelayHandler.isRunning()){
         return;
     } else {
         setState(0);
         mode = MODE_IDLE;
     }

 }// end of DigitalOutputHandler::handleOnPulse
 //--------------------------------------------------------------------------------------------------

 //--------------------------------------------------------------------------------------------------
 // DigitalOutputHandler::handlePulseSequence
 //
 /**
  *
  * Handles a pulse sequence started by a function such as DigitalOutputHandler::startPulseSequence.
  * Turns the output ON and OFF for the previously specified durations.
  *
  * This function should be called frequently when a sequence is in progress. It is usally called by
  * the process fuction, so the process function should be called frequently by the client code.
  *
  * Client code can determine when a sequence is in progress by checking if (mode != MODE_IDLE).
  *
  */

void DigitalOutputHandler::handlePulseSequence()
{

    switch(stateMachine){

        case STATE_IDLE:

            return;

            break;

        case STATE_START_SEQUENCE:

            setState(1);
            pulseDelayHandler.start(onDurationMS1);
            stateMachine = 1;

            break;

        case 1:

            if(pulseDelayHandler.isRunning()){
                return;
            } else {

                setState(0);
                pulseDelayHandler.start(offDurationMS1);
                stateMachine = 2;
            }

            break;

        case 2:

            if(pulseDelayHandler.isRunning()){
                return;
            } else {
                setState(1);
                pulseDelayHandler.start(onDurationMS2);
                stateMachine = 3;
            }

            break;

        case 3:

            if(pulseDelayHandler.isRunning()){
                return;
            } else {
                setState(0);
                pulseDelayHandler.start(offDurationMS2);
                stateMachine = 4;
            }

            break;

        case 4:

            if(pulseDelayHandler.isRunning()){
                return;
            } else {
                setState(1);
                pulseDelayHandler.start(onDurationMS3);
                stateMachine = 5;
            }

            break;

        case 5:

            if(pulseDelayHandler.isRunning()){
                return;
            } else {
                setState(0);
                pulseDelayHandler.start(offDurationMS3);
                stateMachine = 6;
            }

            break;

        case 6:

            if(pulseDelayHandler.isRunning()){
                return;
            } else {
                stateMachine = STATE_IDLE;
                mode = MODE_IDLE;
            }

            break;

    }

}// end of DigitalOutputHandler::handlePulseSequence
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// DigitalOutputHandler::~DigitalOutputHandler (destructor)
//
/**
 *
 * This gets called when the object is destroyed. All objects and memory allocated by this object
 * should be released here.
 *
 */

DigitalOutputHandler::~DigitalOutputHandler()
{

    Serial.print(className); Serial.println(": destructor called...");

}// end of DigitalOutputHandler::~DigitalOutputHandler (destructor)
//--------------------------------------------------------------------------------------------------

//end of class DigitalOutputHandler
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
