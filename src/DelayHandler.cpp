//--------------------------------------------------------------------------------------------------
// DelayHandler.cpp
//
//
// Main code for the project.
//
// Authors:
//            Khanh Nguyen
//            Mike Schoonover
//
// Date of project start: 2025/02/12
//
//--------------------------------------------------------------------------------------------------

#include "DelayHandler.h"

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// class DelayHandler
//
/**
 *
 * This class handles a delay timer.
 *
 */

//--------------------------------------------------------------------------------------------------
// DelayHandler::DelayHandler (constructor)
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

DelayHandler::DelayHandler(const unsigned long pDelayTimeMS, const String pObjectName,
                                                                         const int pObjectNumber) :
    objectName(pObjectName),
    objectNumber(pObjectNumber),
    startTimeMS(0),
    delayTimeMS(pDelayTimeMS)
{

    // NOTE:
    // In many CPP programs, variables are initialized here instead of in the initializer list
    // above. For example:
    //
    //      INSTEAD OF THE ABOVE:
    //          objectNumber(pObjectNumber)
    //      THIS WOULD BE PLACED HERE IN THE FUNCTION:
    //          objectNumber = objectNumber;

    className = "  DelayHandler:" + objectName;

    Serial.print(className); Serial.println(": constructor called...");

}// end of DelayHandler::DelayHandler (constructor)
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// DelayHandler::init
//
/**
 *
 * Initializes the object...call immediately after instantiation.
 *
 */

void DelayHandler::init()
{

    Serial.print(className); Serial.println(": init called...");

}// end of DelayHandler::init
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// DelayHandler::start
//
/**
 *
 * Starts or resets a delay timer by recording the current time and setting the desired 
 * delay duration.
 *
 */

 void DelayHandler::start(const unsigned long pDelayTimeMS)
 {
 
    startTimeMS = millis();

    delayTimeMS = pDelayTimeMS;

 }// end of DelayHandler::start
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// DelayHandler::start
//
/**
 *
 * Starts or resets the delay timer without specifying a new duration.
 *
 */

 void DelayHandler::start()
 {
 
    startTimeMS = millis();
   
 }// end of DelayHandler::start
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// DelayHandler::isRunning
//
/**
 *
 * Checks whether the delay timer is still active (i.e. if the specified time has not yet elapsed 
 * since it was started).
 *
 */

 bool DelayHandler::isRunning()
 {

    if((startTimeMS != 0) && ((millis() - startTimeMS) < delayTimeMS)){
        return(true);
    } else {
        startTimeMS = 0;
        return(false);
    }

 }// end of DelayHandler::isRunning
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// DelayHandler::process
//
/**
 *
 * This function should be called by the Arduino 'loop' function which is in the .ino file for
 * Arduino IDE projects and in the main.cpp file for VSCode projects.
 *
 */

void DelayHandler::process()
{

}// end of DelayHandler::process
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// DelayHandler::~DelayHandler (destructor)
//
/**
 *
 * This gets called when the object is destroyed. All objects and memory allocated by this object
 * should be released here.
 *
 */

DelayHandler::~DelayHandler()
{

    Serial.print(className); Serial.println(": destructor called...");

}// end of DelayHandler::~DelayHandler (destructor)
//--------------------------------------------------------------------------------------------------

//end of class DelayHandler
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
