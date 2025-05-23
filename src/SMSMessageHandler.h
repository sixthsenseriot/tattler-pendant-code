
#ifndef _SMS_MESSAGE_HANDLER_H
#define _SMS_MESSAGE_HANDLER_H

// the above preprocessor directives are referred to as 'Header Guards'

#include <Arduino.h>
#include <WebServer.h>
#include <HTTPClient.h>

#include "htmlContent.h"
#include "DelayHandler.h"

//--------------------------------------------------------------------------------------------------
// class SMSMessageHandler
//
/**
 *
 * This class handles the sending of SMS messages.
 *
 */

class SMSMessageHandler{

	public:         // functions ~ all classes can see these

                    // WARNING: if the parameter names used here do no match those used in the
                    // .cpp file, no error will occur...but for clarity they SHOULD match

                    // for SKOONIE projects, all parameters begin with 'p'

                    // use const prefix for all parameters where possible...this helps the
                    // compiler catch errors and can result in more efficient code

		explicit SMSMessageHandler(WebServer * const pServerPtr, const String pObjectName, const int pObjectNumber);

		void init();

		void process();

        virtual ~SMSMessageHandler();

        void handleSendSMS(const String pUser, const bool pUseBrowserRedirect);

        void fetchHTMLFromWebsite(const String& url);

	protected:      // functions ~ only this class and derived (child) classes can see these
    

    private:        // functions ~ only this class can see these ( use sparingly as it is often
                    //    becomes necessary for derived classes to access parent functions )


	public:         // variables ~ all classes can see these

        String className;

    protected:      // variables ~ only this class and derived (child) classes can see these

        int objectNumber;

        String objectName;

        WebServer * const serverPtr;

        int rateLimiterStartMS;

        const int SMS_SEND_RATE_LIMIT_MS = 60000;

        DelayHandler smsSendDelayHandler;

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

        static const int SMS_SEND_DELAY_HANDLER_OBJECT_NUMBER = 1;


    public:         // put static const variables down here where they are easy to find

};// end of class SMSMessageHandler
//--------------------------------------------------------------------------------------------------

#endif // _SMS_MESSAGE_HANDLER_H