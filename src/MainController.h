
#ifndef _MAIN_CONTROLLER_H
#define _MAIN_CONTROLLER_H

// the above preprocessor directives are referred to as 'Header Guards'

#include <cxxabi.h>
#include <WiFi.h>  
#include <WebServer.h>  
#include <DNSServer.h>
#include <Preferences.h>
#include <HTTPClient.h>

#include "htmlContent.h"
#include "Tools.h"
#include "EventHandlerInterface.h"
#include "MoistureSensorHandler.h"
#include "SMSMessageHandler.h"
#include "DigitalOutputHandler.h"
#include "BuzzerHandler.h"

/**
 *
 * All comments which start with <slash>** are doxygen comments and will be used by doxygen to
 * create the code documenation.
 *
 */

/**
 *
 * HOW TO CREATE A NEW CLASS BY COPYING
 *
 * Copy the .h and .cpp files from a small class in your project OR copy one of the classes from
 * this template to your project.
 *
 * For example, if this MainController class is used as the source files to create new class
 * NewClass:
 *
 * Copy and paste MainController.h and MainController.cpp and rename them to your new
 * class name:
 *
 *  MainController.h        ->      NewClass.h
 *  MainController.cpp      ->      NewClass.cpp
 *
 * In the new .h file:
 *
 *      change header guards to the new name (3 places in the file):
 *
 *          #ifndef _MAIN_CONTROLLER_H      ->      #ifndef _NEW_CLASS_H
 *          #define _MAIN_CONTROLLER_H      ->      #define _NEW_CLASS_H
 *          ...
 *          ...
 *          #endif // MAIN_CONTROLLER_H     ->      endif // _NEW_CLASS_H
 *
 *      use ctrl-h to change all references to the old class name to the new class name:
 *
 *          MainController      ->      NewClass
 *
 *      delete all function and variable definitions EXCEPT:
 *
 *          NewClass();           // the constructor
 *          void init();          // change parameters as appropriate for new class
 *          void process();       // keep if needed for the new class
 *          virtual ~NewClass();  // the constructor
 *
 * In the new .cpp file:
 *
 *      use ctrl-h to change all references to the old class name to the new class name:
 *
 *          MainController      ->      NewClass
 *
 *      delete all function and variable definitions EXCEPT:
 *
 *          NewClass::NewClass();           // the constructor
 *          NewClass::init();               // change parameters as appropriate for new class
 *          void NewClass::process();       // keep if needed for the new class
 *          virtual NewClass::~NewClass();  // the constructor
 *
 */

//--------------------------------------------------------------------------------------------------
// class MainController
//
/**
 *
 * This class is the main Controller (as in MVC structure).
 *
 * This class creates all other objects and is the main point of action. It inherits
 * EventHandlerInterface so that it can receive events from other objects.
 *
 */


class MainController : public EventHandlerInterface {

	public:         // functions ~ all classes can see these

		explicit MainController(); //constructor

		void init();

        void setupAccessPoint();

        void configureDataResetInputPin();

        void checkDataResetButtonAndResetDataIfActive(); 
    
        void setupWebServerForCaptivePortal();

        void setupWebServerForNormalUse();

        static void sendSMSToUserParsedFromHTMLArgList();

        bool wifiConnectionSuccessful = false;

        static void saveData(const String pKey, const String pValue, const String pNamespaceName);

        static String loadData(const String pKey, const String pNamespaceName);

        static void connectToWiFi();

        static void handleWiFiConnectionError();

        static void fetchHTMLFromWebsite(const String& url);

        static void handleCaptivePortalPageSend();

        void connectToWiFiAndFetchSkooniePage();

        static void sendWelcomePage();

        static void handleSave();

        void handleFetchWebsite();

        static void handleNotFound();

        void clearAllPersistentData();

        void startSerialPort();

		void process();

        virtual ~MainController(); //destructor

	protected:      // functions ~ only this class and derived (child) classes can see these


    private:        // functions ~ only this class can see these ( use sparingly as it is often
                    //    becomes necessary for derived classes to access parent functions )


	public:         // variables ~ all classes can see these

        String className;

    protected:      // variables ~ only this class and derived (child) classes can see these

        int processCounter;

        SMSMessageHandler smsMessageHandler;

        static SMSMessageHandler *smsMessageHandlerPtr;

        MoistureSensorHandler moistureSensorHandler;

        MoistureSensorHandler *moistureSensorHandlerPtr;

        static Preferences preferences;

        DNSServer dnsServer;

        static WebServer server;

        const byte DNS_PORT = 53;

        const char* ssid = "Tattler";

        const char* password = "42424242";

        const int DATA_RESET_BUTTON_PIN = A5; // new pin = A5 on HUZZAH32 // old pin = 2 on ESP32 D1 Mini

        DigitalOutputHandler onboardLEDHandler;

        BuzzerHandler buzzerHandler;

    private:        // variables ~ only this class can see these ( use sparingly as it is often
                    //    becomes necessary for derived classes to access parent variables )

        static const int MOISTURE_SENSOR_HANDLER_OBJECT_NUMBER = 1;

        static const int SMS_MESSAGE_HANDLER_OBJECT_NUMBER = 2;

        static const int ONBOARD_LED_HANDLER_OBJECT_NUMBER = 3;

        static const int BUZZER_HANDLER_OBJECT_NUMBER = 4;

};// end of class MainController
//--------------------------------------------------------------------------------------------------

#endif // MAIN_CONTROLLER_H