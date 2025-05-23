//--------------------------------------------------------------------------------------------------
// SMSMessageHandler.cpp
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

#include "SMSMessageHandler.h"

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// class SMSMessageHandler
//
/**
 *
 * This class handles the sending of SMS messages.
 *
 */

//--------------------------------------------------------------------------------------------------
// SMSMessageHandler::SMSMessageHandler (constructor)
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

SMSMessageHandler::SMSMessageHandler(WebServer * const pServerPtr, const String pObjectName, const int pObjectNumber) :
    serverPtr(pServerPtr),
    objectName(pObjectName),
    objectNumber(pObjectNumber),
    rateLimiterStartMS(0),
    smsSendDelayHandler(SMS_SEND_RATE_LIMIT_MS, "SMS Send Delay Handler",
                                                           SMS_SEND_DELAY_HANDLER_OBJECT_NUMBER)
{

    // NOTE:s
    // In many CPP programs, variables are initialized here instead of in the initializer list
    // above. For example:
    //
    //      INSTEAD OF THE ABOVE:
    //          objectNumber(pObjectNumber)
    //      THIS WOULD BE PLACED HERE IN THE FUNCTION:
    //          objectNumber = objectNumber;

    className = "  SMSMessageHandler:" + objectName;

    Serial.print(className); Serial.println(": constructor called...");

}// end of SMSMessageHandler::SMSMessageHandler (constructor)
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// SMSMessageHandler::init
//
/**
 *
 * Initializes the object...call immediately after instantiation.
 *
 */

void SMSMessageHandler::init()
{

    Serial.print(className); Serial.println(": init called...");

}// end of SMSMessageHandler::init
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// SMSMessageHandler::process
//
/**
 *
 * This function should be called by the Arduino 'loop' function which is in the .ino file for
 * Arduino IDE projects and in the main.cpp file for VSCode projects.
 *
 */

void SMSMessageHandler::process()
{



}// end of SMSMessageHandler::process
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// SMSMessageHandler::handleSendSMS
//
/**
 *
 * Handles sending an SMS for a given user, while preventing rapid repeat sends using a delay timer.
 * 
 */

 void SMSMessageHandler::handleSendSMS(const String pUser, const bool pUseBrowserRedirect)
 {

    if(smsSendDelayHandler.isRunning()){
        Serial.println("SMS not sent as timeout not complete.");
        return;
    } else {
        smsSendDelayHandler.start();
    }

    Serial.println("Sending SMS to: " + pUser);
    
    String url = "http://159.89.154.88/sms" + pUser + ".html";
    Serial.println("Redirecting to " + url);

    return; // Debug KTN -- remove this to send SMS

    if(pUseBrowserRedirect){
        serverPtr->sendHeader("Location", url, true);
        serverPtr->send(302, "text/plain", "Redirecting...");
    } else {
        fetchHTMLFromWebsite(url);
    }

 }// end of SMSMessageHandler::handleSendSMS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// SMSMessageHandler::fetchHTMLFromWebsite
//
/**
 *
 * Performs an HTTP GET request to a specified URL and prints the response content and status code 
 * to the serial console.
 * 
 */

 void SMSMessageHandler::fetchHTMLFromWebsite(const String& url)
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
 
 }// end of SMSMessageHandler::fetchHTMLFromWebsite
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// SMSMessageHandler::~SMSMessageHandler (destructor)
//
/**
 *
 * This gets called when the object is destroyed. All objects and memory allocated by this object
 * should be released here.
 *
 */

SMSMessageHandler::~SMSMessageHandler()
{

    Serial.print(className); Serial.println(": destructor called...");

}// end of SMSMessageHandler::~SMSMessageHandler (destructor)
//--------------------------------------------------------------------------------------------------

//end of class SMSMessageHandler
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
