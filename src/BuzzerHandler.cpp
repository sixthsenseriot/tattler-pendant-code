//--------------------------------------------------------------------------------------------------
// BuzzerHandler.cpp
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

#include "BuzzerHandler.h"

//#define DEBUG_VERBOSE_DIGITAL_OUTPUT_HANDLER	// displays extra debugging messages on serial port

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// class BuzzerHandler
//
/**
 *
 * This class controls a buzzer. The buzzer can be activated to emit a specified tone, or to emit
 * a tone pulse for a set time period. Also, the buzzer can be activated with a sequence of tones.
 *
 * This class is asynchronous and does NOT use the delay() function for timing. It uses a
 * DelayHandler object for timing instead.
 *
 * The BuzzerHandler::process function must be called frequently while a pulse or pulse sequence is
 * active so that it can be turned off when the timer or the sequence transitions.
 *
 */

//--------------------------------------------------------------------------------------------------
// BuzzerHandler::BuzzerHandler (constructor)
//
/**
 *
 * Class constructor.
 *
 * The ONLY thing that should be done here is initializing objects and variables and storing
 * parameters to class variables.
 *
 * @param pIoPinNum         the I/O pin to which the device is attached
 * @param pObjectName       the name for this object
 * @param pObjectNumber     an identifying number for this object
 *
 */

BuzzerHandler::BuzzerHandler(const int pIOPinNum, const String pObjectName, const int pObjectNumber) :
    objectName(pObjectName),
    objectNumber(pObjectNumber),
    ioPinNum(pIOPinNum),
    durationMS1(0),
    durationMS2(0),
    durationMS3(0),
    stateMachine(STATE_IDLE),
    mode(MODE_IDLE),
    pulseDelayHandler(PULSE_DELAY_LIMIT_MS, "Pulse Delay Handler",PULSE_DELAY_HANDLER_OBJECT_NUMBER)
{

    className = "  BuzzerHandler:" + objectName;

    Serial.print(className); Serial.println(": constructor called...");

}// end of BuzzerHandler::BuzzerHandler (constructor)
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// BuzzerHandler::init
//
/**
 *
 * Initializes the object...call immediately after instantiation.
 *
 * The output pin mode is NOT set up here. For PWM, set up is done when the output is activated
 * so the frequency can be set for each call.
 *
 */

void BuzzerHandler::init()
{

    Serial.print(className); Serial.println(": init called...");

}// end of BuzzerHandler::init
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// BuzzerHandler::emitTone
//
/**
 *
 * Activates the buzzer to emit at tone at the specified frequency. The tone will be emitted
 * until a call to endTone (or certain other functions) is called.
 *
 * Note:
 *
 *  The 'ledc' phrase used in the function calls is due to ESP32 referring to all PWM outputs as
 *  LED controllers...even though they have many other uses.
 *
 * @param pFrequencyHz      the frequency in Hertz of the tone to be emitted
 *
 */

 void BuzzerHandler::emitTone(const int pFrequencyHz)
 {

    frequencyHz = pFrequencyHz;
    ledcSetup(PWM_CHANNEL, frequencyHz, PWM_RESOLUTION);
    ledcAttachPin(ioPinNum, PWM_CHANNEL);

    ledcWrite(PWM_CHANNEL, 128);

 }// end of BuzzerHandler::emitTone
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// BuzzerHandler::endTone
//
/**
 *
 * Stops the buzzer from emitting sound.
 *
 */

 void BuzzerHandler::endTone()
 {

    ledcWrite(PWM_CHANNEL, 0);

 }// end of BuzzerHandler::endTone
 //--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// BuzzerHandler::startPulse
//
/**
 *
 * Activates the buzzer to emit a tone at the specified frequency for the specified time duration.
 *
* Calling certain other functions in this class before (mode == MODE_IDLE) may result in unexpected
 * behavior.
 *
 * @param pFrequencyHz      the frequency of the tone in Hertz to be emitted
 * @param pDurationMS       the time duration in milliseconds for which the tone is to be emitted
 *
 */

 void BuzzerHandler::startPulse(const int pFrequencyHz, const int pDurationMS)
 {

    Serial.println("Tone pulse activated.");
    frequencyHz = pFrequencyHz;
    ledcSetup(PWM_CHANNEL, frequencyHz, PWM_RESOLUTION);
    emitTone(frequencyHz);

    pulseDelayHandler.start(pDurationMS);

    mode = MODE_HANDLE_PULSE;

 }// end of BuzzerHandler::startPulse
 //--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// BuzzerHandler::startPulseSequence
//
/**
 *
 * Invokes emmission of a series of tones of specified frequencies for specified time durations.
 *
 * Calling certain other functions in this class before (mode == MODE_IDLE) may result in unexpected
 * behavior.
 *
 * wip mks:
 *
 * This function needs to be changed to accept a variable number of parameters to allow for
 * a variable length of the sequence.
 *
 * @param pFrequencyHz1     frequency of first tone in the series
 * @param pDurationMS1      time duration of first tone in the series
 * @param pFrequencyHz2     frequency of first tone in the series
 * @param pDurationMS2      time duration of first tone in the series
 * @param pFrequencyHz3     frequency of first tone in the series
 * @param pDurationMS3      time duration of first tone in the series
 *
 */

 void BuzzerHandler::startPulseSequence(
    const int pFrequencyHz1, const int pDurationMS1,
    const int pFrequencyHz2, const int pDurationMS2,
    const int pFrequencyHz3, const int pDurationMS3)
 {

    frequencyHz1 = pFrequencyHz1;
    frequencyHz2 = pFrequencyHz2;
    frequencyHz3 = pFrequencyHz3;

    durationMS1 = pDurationMS1;
    durationMS2 = pDurationMS2;
    durationMS3 = pDurationMS3;

    Serial.println("Starting pulse sequence...");

    stateMachine = STATE_START_SEQUENCE;

    mode = MODE_HANDLE_SEQUENCE;

 }// end of BuzzerHandler::startPulseSequence
 //--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// BuzzerHandler::process
//
/**
 *
 * This function should be called by the Arduino 'loop' function which is in the .ino file for
 * Arduino IDE projects and in the main.cpp file for VSCode projects.
 *
 * This function handles various ongoing processes such as a pulse or pulse sequence by turning
 * off or changing tone emissions at predetermined times.
 *
 *
 */

void BuzzerHandler::process()
{

    switch(mode){

        case MODE_IDLE:

            return;

            break;

        case MODE_HANDLE_PULSE:

            handlePulse();

            break;

        case MODE_HANDLE_SEQUENCE:

            handlePulseSequence();

            break;

    }

}// end of BuzzerHandler::process
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// BuzzerHandler::handlePulse
//
/**
 *
 * Ends a pulse when the timer ends.
 *
 * This function should be called frequently when a pulse is in progress. It is usally called by
 * the process fuction, so the process function should be called frequently by the client code.
 *
 */

 void BuzzerHandler::handlePulse()
 {

    if(pulseDelayHandler.isRunning()){
        return;
    } else {
        endTone();
        mode = MODE_IDLE;
    }

}// end of BuzzerHandler::handlePulse
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// BuzzerHandler::handlePulseSequence
//
/**
 *
 * Handles a pulse sequence started by a function such as BuzzerHandler::startPulseSequence.
 * Switches between a series of specified tones for specified time durations.
 *
 * This function should be called frequently when a sequence is in progress. It is usally called by
 * the process fuction, so the process function should be called frequently by the client code.
 *
 * Client code can determine when a sequence is in progress by checking if (mode != MODE_IDLE).
 *
 */

 void BuzzerHandler::handlePulseSequence()
 {

    switch(stateMachine){


        case STATE_IDLE:

            return;

            break;

        case STATE_START_SEQUENCE:

            emitTone(frequencyHz1);
            pulseDelayHandler.start(durationMS1);
            stateMachine = 1;

            break;

        case 1:

            if(pulseDelayHandler.isRunning()){
                return;
            } else {
                emitTone(frequencyHz2);
                pulseDelayHandler.start(durationMS2);
                stateMachine = 2;
            }

            break;

        case 2:

            if(pulseDelayHandler.isRunning()){
                return;
            } else {
                emitTone(frequencyHz3);
                pulseDelayHandler.start(durationMS3);
                stateMachine = 3;
            }

            break;

        case 3:

            if(pulseDelayHandler.isRunning()){
                return;
            } else {
                endTone();
                stateMachine = STATE_IDLE;
                mode = MODE_IDLE;
            }

            break;

    }

 }// end of BuzzerHandler::handlePulseSequence
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// BuzzerHandler::~BuzzerHandler (destructor)
//
/**
 *
 * This gets called when the object is destroyed. All objects and memory allocated by this object
 * should be released here.
 *
 */

BuzzerHandler::~BuzzerHandler()
{

    Serial.print(className); Serial.println(": destructor called...");

}// end of BuzzerHandler::~BuzzerHandler (destructor)
//--------------------------------------------------------------------------------------------------

//end of class BuzzerHandler
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
