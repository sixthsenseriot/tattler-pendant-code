//--------------------------------------------------------------------------------------------------
// MoistureSensorHandler.cpp
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

#include "MoistureSensorHandler.h"

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// class MoistureSensorHandler
//
/**
 *
 * This class interfaces with the moisture sensor.
 *
 */

//--------------------------------------------------------------------------------------------------
// MoistureSensorHandler::MoistureSensorHandler (constructor)
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

MoistureSensorHandler::MoistureSensorHandler(SMSMessageHandler * const pSMSMessageHandlerPtr, 
                WebServer * const pServerPtr, const String pObjectName, const int pObjectNumber) :
    smsMessageHandlerPtr(pSMSMessageHandlerPtr),
    serverPtr(pServerPtr),
    objectName(pObjectName),
    objectNumber(pObjectNumber),
    prevState(false)
{

    // NOTE:
    // In many CPP programs, variables are initialized here instead of in the initializer list
    // above. For example:
    //
    //      INSTEAD OF THE ABOVE:
    //          objectNumber(pObjectNumber)
    //      THIS WOULD BE PLACED HERE IN THE FUNCTION:
    //          objectNumber = objectNumber;

    className = "  MoistureSensorHandler:" + objectName;

    Serial.print(className); Serial.println(": constructor called...");

}// end of MoistureSensorHandler::MoistureSensorHandler (constructor)
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MoistureSensorHandler::init
//
/**
 *
 * Initializes the object...call immediately after instantiation.
 *
 * 
 *
 */

void MoistureSensorHandler::init()
{

    Serial.print(className); Serial.println(": init called...");

    configureIOPins();

}// end of MoistureSensorHandler::init
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MoistureSensorHandler::process
//
/**
 *
 * This function should be called by the Arduino 'loop' function which is in the .ino file for
 * Arduino IDE projects and in the main.cpp file for VSCode projects.
 *
 */

void MoistureSensorHandler::process()
{

    checkMoistureSensorAndSendSMSMessageOnTrigger();

    // DEBUG KTN
    // Serial.print(className); Serial.println(": process called..."); 

}// end of MoistureSensorHandler::process
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MoistureSensorHandler::configureIOPins
//
/**
 *
 * 
 * 
 */

void MoistureSensorHandler::configureIOPins()
{

    analogReadResolution(12); 
    pinMode(MOISTURE_SENSOR_ANALOG_INPUT_PIN, INPUT);

    // For example, ADC_ATTEN_DB_11 covers up to ~3.3V
    adcAttachPin(MOISTURE_SENSOR_ANALOG_INPUT_PIN);
    analogSetPinAttenuation(MOISTURE_SENSOR_ANALOG_INPUT_PIN, ADC_11db);

    pinMode(LED1,OUTPUT);
    pinMode(LED2,OUTPUT);
    pinMode(LED3,OUTPUT);

    Serial.print(className); Serial.println(": configureIOPins called...");

}// end of MoistureSensorHandler::configureIOPins
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MoistureSensorHandler::checkMoistureSensorAndSendSMSMessageOnTrigger
//
/**
 *
 * 
 * 
 */

void MoistureSensorHandler::checkMoistureSensorAndSendSMSMessageOnTrigger()
{

    sensor = analogRead(MOISTURE_SENSOR_ANALOG_INPUT_PIN);

    Serial.println(sensor);

    if(sensor > dryThreshold) {
        Serial.println("dry");
        prevState = false;

    } else if(sensor < wetThreshold) {
        Serial.println("wet");

        if(prevState != true){

            printf("Calling SMSMessageHandler::handleSendSMS using pointer: %%p:  x    = %p\n", smsMessageHandlerPtr);

            smsMessageHandlerPtr->handleSendSMS("Khanh", false);
            // smsMessageHandlerPtr->handleSendSMS("Mike", false);
            // smsMessageHandlerPtr->handleSendSMS("Debra", false);

            Serial.println("Previous state set to true.");
            prevState = true;

        }

    } else {
        prevState = false;
        Serial.println("who knows?");
    }

    delay(delayTime);

}// end of MoistureSensorHandler::checkMoistureSensorAndSendSMSMessageOnTrigger
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MoistureSensorHandler::fetchHTMLFromWebsite
//
/**
 *
 * 
 * 
 */

void MoistureSensorHandler::fetchHTMLFromWebsite(const String& url)
{

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println("HTTP Response Code: " + String(httpCode));
      String payload = http.getString();
      Serial.println("Raw HTML Content:\n" + payload);
    } else {
      Serial.println("Failed to fetch content. HTTP Code: " + String(httpCode));
    }
    http.end();
  } else {
    Serial.println("Wi-Fi not connected. Unable to fetch content.");
  }

}// end of MoistureSensorHandler::fetchHTMLFromWebsite
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MoistureSensorHandler::~MoistureSensorHandler (destructor)
//
/**
 *
 * This gets called when the object is destroyed. All objects and memory allocated by this object
 * should be released here.
 *
 */

MoistureSensorHandler::~MoistureSensorHandler()
{

    Serial.print(className); Serial.println(": destructor called...");

}// end of MoistureSensorHandler::~MoistureSensorHandler (destructor)
//--------------------------------------------------------------------------------------------------

//end of class MoistureSensorHandler
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
