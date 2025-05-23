
#ifndef _MOISTURE_SENSOR_HANDLER_H
#define _MOISTURE_SENSOR_HANDLER_H

// the above preprocessor directives are referred to as 'Header Guards'

#include <Arduino.h>
#include <WebServer.h>
#include <HTTPClient.h>

#include "htmlContent.h"
#include "SMSMessageHandler.h"

//--------------------------------------------------------------------------------------------------
// class MoistureSensorHandler
//
/**
 *
 * This class interfaces with the moisture sensor.
 *
 */

class MoistureSensorHandler{

	public:         // functions ~ all classes can see these

                    // WARNING: if the parameter names used here do no match those used in the
                    // .cpp file, no error will occur...but for clarity they SHOULD match

                    // for SKOONIE projects, all parameters begin with 'p'

                    // use const prefix for all parameters where possible...this helps the
                    // compiler catch errors and can result in more efficient code

		explicit MoistureSensorHandler(SMSMessageHandler * const pSMSMessageHandlerPtr, 
            WebServer * const pServerPtr, const String pObjectName, const int pObjectNumber);

		void init();

		void process();

        virtual ~MoistureSensorHandler();

        void configureIOPins();

        void checkMoistureSensorAndSendSMSMessageOnTrigger();

        void fetchHTMLFromWebsite(const String& url);

	protected:      // functions ~ only this class and derived (child) classes can see these
 
        bool prevState;

    private:        // functions ~ only this class can see these ( use sparingly as it is often
                    //    becomes necessary for derived classes to access parent functions )


	public:         // variables ~ all classes can see these

        String className;

    protected:      // variables ~ only this class and derived (child) classes can see these

        int objectNumber;

        String objectName;

        SMSMessageHandler * const smsMessageHandlerPtr;

        WebServer * const serverPtr;

        const int MOISTURE_SENSOR_ANALOG_INPUT_PIN = A7; // HUZZAH32 PIN 32 = A7 // OLD PIN 34 = A6 

        const int LED1 = 2;

        const int LED2 = 3;

        const int LED3 = 4;

        int sensor;

        const int delayTime = 1000; 

        int wetThreshold = 900;
        
        int dryThreshold = 4096;

                    // NOTE:
                    //
                    // for Arduino-based environments, String is part of the Arduino core libarary
                    // and you should use '#include <Arduino.h>' at the top of this file.
                    //
                    // For 'real' CPP programs, include the <string.h> library and use
                    // 'std::string objectName' instead of 'String objectName'.
                    //

    private:        // variables ~ only this class can see these ( use sparingly as it is often
                    //    becomes necessary for derived classes to access parent variables )

    public:         // put static const variables down here where they are easy to find

};// end of class MoistureSensorHandler
//--------------------------------------------------------------------------------------------------

#endif // _MOISTURE_SENSOR_HANDLER_H