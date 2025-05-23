//-----------------------------------------------------------------------------------------------------------
// MainController.cpp
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

#include "MainController.h"

// NOTE: it is best to do all other includes in the header file. If you do them here, the header
// file cannot see them and it often needs to see them to define variables. Putting the includes
// in the header file means both the header file and this cpp file can see them since this cpp
// file includes the header file which in turn includes the other files.
//

//#define DEBUG_VERBOSE_CONTROLLER 1	// displays extra debugging messages on serial port

//--------------------------------------------------------------------------------------------------
// static variables

WebServer MainController::server(80);

Preferences MainController::preferences;

SMSMessageHandler * MainController::smsMessageHandlerPtr = nullptr;

//--------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------
// MainController::MainController (constructor)
//
/**
 *
 * Class constructor.
 *
 * The ONLY thing that should be done here is initializing objects and variables and storing
 * parameters to class variables.
 *
 */

MainController::MainController() :
    smsMessageHandler(&server, "SMS Message Handler", SMS_MESSAGE_HANDLER_OBJECT_NUMBER),
    moistureSensorHandler(&smsMessageHandler, &server, "Moisture Sensor Handler", MOISTURE_SENSOR_HANDLER_OBJECT_NUMBER),
    className(""),
    processCounter(0),
    moistureSensorHandlerPtr(&moistureSensorHandler),
    onboardLEDHandler(13, "Onboard LED Handler", ONBOARD_LED_HANDLER_OBJECT_NUMBER),
    buzzerHandler(27, "Buzzer Handler", BUZZER_HANDLER_OBJECT_NUMBER)

    // The above is the initializer list...all statically declared classes and super classes should
    // be initialized here. Some experts recommend initializing ALL variables here for clarity,
    // such as is done for processCounter above...this is equivalent to: processCounter = 0
    //
    // NOTE: ALL objects should be initialized in the initializer list or they end up being
    //         initialized twice by C++ which is wasteful.
    //
    //      Initializing variables in the initializer list is optional, but is a good practice.
    //
    //      ALL variables should be explicitly initialized...the default initialization should not
    //      be trusted as it can be difficult to remember when and where default initialization
    //      is done.
    //

{
    moistureSensorHandlerPtr = &moistureSensorHandler;

    smsMessageHandlerPtr = &smsMessageHandler;

    className = "MainController";

    //
    // The above is a crude, inflexible way of setting the classname, but the compiler for ESP32
    // uses flag -fno-rtti which disallows the use of typeid.
    //
    // Below is the preferred method as it works with polymorphism:
    //
    // int status;
    // className = abi::__cxa_demangle(typeid(this).name(),0,0,&status);
    //

    startSerialPort();

    Serial.print(className); Serial.println(": constructor called...");

}// end of MainController::MainController (constructor)
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::init
//
/**
 *
 * Initializes the object...call immediately after instantiation.
 *
 */

void MainController::init()
{

    // Serial.begin(115200); // Debug KTN

    Serial.print(className); Serial.println(": init called...");

    moistureSensorHandlerPtr->init();

    smsMessageHandlerPtr = &smsMessageHandler;

    smsMessageHandlerPtr->init();

    delay(2500);

    Serial.println("");

    configureDataResetInputPin();

    onboardLEDHandler.init();
    onboardLEDHandler.startPulseSequence(5000, 1000, 3000, 2000, 2000, 1000);

    buzzerHandler.init();
//    buzzerHandler.startPulseSequence(2000, 500, 3000, 1000, 4000, 1500);

    checkDataResetButtonAndResetDataIfActive();

    String initialized = loadData("initialized", "tattler-app");
    Serial.println("Tattler Client Mode Initialized: " + (initialized.isEmpty() ? "Not Set" : initialized));

    if (initialized == "Not Found" || initialized.isEmpty())
    {
        WiFi.softAP(ssid, password);
        Serial.println("Access Point Started");
        Serial.print("IP Address: ");
        Serial.println(WiFi.softAPIP());

        setupAccessPoint();
        setupWebServerForCaptivePortal();

    } else {
        connectToWiFi();
        if (WiFi.status() != WL_CONNECTED) {
            handleWiFiConnectionError();
            return;
        }
        setupWebServerForNormalUse();
    }

    Serial.println("Loading saved data...");
    Serial.println("Wi-Fi Name: " + loadData("wifiName", "tattler-app"));
    Serial.println("Wi-Fi Password: " + loadData("wifiPassword", "tattler-app"));
    Serial.println("Pet Name: " + loadData("petName", "tattler-app"));
    Serial.println("Account Number: " + loadData("accountNumber", "tattler-app"));

}// end of MainController::init
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::configureDataResetInputPin
//
/**
 *
 *
 *
 */

void MainController::configureDataResetInputPin()
{

    pinMode(DATA_RESET_BUTTON_PIN, INPUT);
    pinMode(DATA_RESET_BUTTON_PIN, INPUT_PULLUP);

}// end of MainController::configureDataResetInputPin
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::setupAccessPoint
//
/**
 *
 *
 *
 */

void MainController::setupAccessPoint()
{

    WiFi.softAP(ssid, password);
    Serial.println("Access Point Started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    if (dnsServer.start(DNS_PORT, "*", WiFi.softAPIP())) {
        Serial.println("DNS Server Started");
    } else {
        Serial.println("Failed to Start DNS Server");
    }

}// end of MainController::setupAccessPoint
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::checkDataResetButtonAndResetDataIfActive
//
/**
 *
 *
 *
 */

void MainController::checkDataResetButtonAndResetDataIfActive()
{

    if (digitalRead(DATA_RESET_BUTTON_PIN) == LOW)
    {
        clearAllPersistentData();
        delay(500);

    }

}// end of MainController::checkDataResetButtonAndResetDataIfActive
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::setupWebServerForCaptivePortal
//
/**
 *
 * Prepares Web Server to deliver Captive Portal page. Only useful when device has been factory
 * reset.
 *
 */

void MainController::setupWebServerForCaptivePortal()
{

    server.on("/", HTTP_ANY, handleCaptivePortalPageSend);
    server.on("/hotspot-detect.html", HTTP_ANY, handleCaptivePortalPageSend);
    server.on("/save", HTTP_POST, handleSave);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("Web Server Started For Captive Portal Delivery");

}// end of MainController::setupWebServerForCaptivePortal
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::setupWebServerForNormalUse
//
/**
 *
 *
 *
 */

void MainController::setupWebServerForNormalUse()
{

    server.on("/sendTattlerSMS.html", HTTP_GET, sendSMSToUserParsedFromHTMLArgList);

    server.on("/index.html", HTTP_GET, sendWelcomePage);
    server.on("/admin", HTTP_GET, sendWelcomePage);
    server.on("/", HTTP_GET, sendWelcomePage);

    server.begin();
    Serial.println("Web Server Started For Normal Use");

}// end of MainController::setupWebServerForNormalUse
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::sendSMSToUserParsedFromHTMLArgList
//
/**
 *
 *
 *
 */

 void MainController::sendSMSToUserParsedFromHTMLArgList()
 {

    if (!server.hasArg("user")) {
        server.send(400, "text/plain", "Missing 'user' parameter");
        return;
    }

    String user = server.arg("user");
    Serial.println("Handling SMS to " + user);

    printf("Calling SMSMessageHandler::handleSendSMS using pointer: %%p:  x    = %p\n", smsMessageHandlerPtr);
    smsMessageHandlerPtr->handleSendSMS(user, true);

}// end of MainController::sendSMSToUserParsedFromHTMLArgList
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::saveData
//
/**
 *
 * Saves a key-value pair to non-volatile memory using a specified namespace.
 *
 * @param pKey              key for the key-value pair
 * @param pValue            value for the key-value pair
 * @param pNamespaceName    namespace for the key-value pair
 *
 */

void MainController::saveData(const String pKey, const String pValue, const String pNamespaceName)
{

    preferences.begin(pNamespaceName.c_str(), false);
    preferences.putString(pKey.c_str(), pValue);
    preferences.end();

}// end of MainController::saveData
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::loadData
//
/**
 *
 * Saves a key-value pair to non-volatile memory using a specified namespace.
 *
 * @param pKey              key for the key-value pair
 * @param pNamespaceName    namespace for the key-value pair
 *
 */

String MainController::loadData(const String pKey, const String pNamespaceName)
{

    preferences.begin(pNamespaceName.c_str(), true);
    String value = preferences.getString(pKey.c_str(), "Not Found");
    preferences.end();
    return value;

}// end of MainController::loadData
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::connectToWiFi
//
/**
 *
 *
 *
 */

void MainController::connectToWiFi()
{

    Serial.print("Attempting to connect to WiFi...");

    String wifiName = loadData("wifiName", "tattler-app");
    String wifiPassword = loadData("wifiPassword", "tattler-app");

    Serial.print("Wifi Name: "); Serial.println(wifiName);
    Serial.print("Wifi Password: "); Serial.println(wifiPassword);

    WiFi.begin(wifiName.c_str(), wifiPassword.c_str());
    Serial.println("Connecting to Wi-Fi...");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWi-Fi Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nFailed to connect to Wi-Fi.");
    }

}// end of MainController::connectToWiFi
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::handleWiFiConnectionError
//
/**
 *
 *
 *
 */

void MainController::handleWiFiConnectionError()
{

  String response = HtmlPage::CAPTIVE_PORTAL_HTML_PAGE_HEADER;
  response += HtmlPage::BASE64_IMAGE_STRING_TATTLER;
  response += HtmlPage::CAPTIVE_PORTAL_HTML_PAGE_FOOTER;
response += "<p style='color: red;'>Failed to connect to Wi-Fi. Please try again and check that you have the correct Wi-Fi name and password.</p>"; // Added for wifi error handling
  server.send(200, "text/html", response);

}// end of MainController::handleWiFiConnectionError
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::fetchHTMLFromWebsite
//
/**
 *
 *
 *
 */

void MainController::fetchHTMLFromWebsite(const String& url)
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

}// end of MainController::fetchHTMLFromWebsite
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::handleCaptivePortalPageSend
//
/**
 *
 *
 *
 */

void MainController::handleCaptivePortalPageSend()
{

    String initialized = loadData("initialized", "tattler-app");
    Serial.println("Loaded initialized in handleCaptivePortalPageSend(): " + initialized);

    if (initialized == "Not Found") {
        Serial.println("Flag 'initialized' not found. Displaying setup page...");
        String response = HtmlPage::CAPTIVE_PORTAL_HTML_PAGE_HEADER;
        response += HtmlPage::BASE64_IMAGE_STRING_TATTLER;
        response += HtmlPage::CAPTIVE_PORTAL_HTML_PAGE_FOOTER;
        server.send(200, "text/html", response);


        // Tried code below but it was not faster.vvvv

        // server.setContentLength(CONTENT_LENGTH_UNKNOWN);
        // server.send(200, "text/html", "");  // starts chunked response
        // server.sendContent(HtmlPage::CAPTIVE_PORTAL_HTML_PAGE_HEADER);
        // server.sendContent(HtmlPage::BASE64_IMAGE_STRING_TATTLER);
        // server.sendContent(HtmlPage::CAPTIVE_PORTAL_HTML_PAGE_FOOTER);
        // server.sendContent("");



    } else {
        Serial.println("Flag 'initialized' found. Displaying welcome page...");
        String htmlContent = HtmlPage::CAPTIVE_PORTAL_INITIALIZED_HTML_PAGE_HEADER;
        htmlContent += "<br><br><br><br>";
        htmlContent += loadData("petName", "tattler-app") + " is doing fine! <br><br>";
        htmlContent += "No incidents to report today. <br><br><br><br>";
        htmlContent += HtmlPage::CAPTIVE_PORTAL_INITIALIZED_HTML_PAGE_FOOTER;

        server.send(200, "text/html", htmlContent);
  }

}// end of MainController::handleCaptivePortalPageSend
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::connectToWiFiAndFetchSkooniePage
//
/**
 *
 * Connects to WiFi, fetches skoonie.com html page and prints it to the serial port.
 *
 */

void MainController::connectToWiFiAndFetchSkooniePage()
{

    connectToWiFi();

    if (WiFi.status() != WL_CONNECTED) {
        handleWiFiConnectionError();
        return;
    }

    String initialized = loadData("initialized", "tattler-app");
    Serial.println("Tattler Client Mode Initialized: " + initialized);

    // Fetch and print Skoonie.com's raw HTML
    fetchHTMLFromWebsite("https://skoonie.com");

}// end of MainController::connectToWiFiAndFetchSkooniePage
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::sendWelcomePage
//
/**
 *
 *
 *
 */

void MainController::sendWelcomePage()
{

    String response = HtmlPage::ADMIN_PORTAL_HTML_PAGE;
    // Acknowledge the user
    server.send(200, "text/html", response);

}// end of MainController::sendWelcomePage
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::handleSave
//
/**
 *
 * Saves a key-value pair to non-volatile memory using a specified namespace.
 *
 */

void MainController::handleSave()
{

    static String wifiName = server.arg("wifiName"); // REFACTOR
    static String wifiPassword = server.arg("wifiPassword"); // REFACTOR
    static String petName = server.arg("petName");
    static String accountNumber = server.arg("accountNumber");

    // Log received data
    Serial.println("Form Data Received:");
    Serial.println("Wi-Fi Name: " + wifiName);
    Serial.println("Wi-Fi Password: " + wifiPassword);
    Serial.println("Pet Name: " + petName);
    Serial.println("Tattler Account Number: " + accountNumber);


    // Save data to non-volatile memory
    saveData("wifiName", wifiName, "tattler-app");
    saveData("wifiPassword", wifiPassword, "tattler-app");
    saveData("petName", petName, "tattler-app");
    saveData("accountNumber", accountNumber, "tattler-app");

    saveData("initialized", "True", "tattler-app");

    String response = HtmlPage::DATA_SAVED_HTML_PAGE;
    // Acknowledge the user
    server.send(200, "text/html", response);

    Serial.println("Rebooting in 5 seconds...");
    delay(5000);
    Serial.println("Rebooting...");

    ESP.restart();

}// end of MainController::handleSave
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::handleFetchWebsite
//
/**
 *
 *
 *
 */

void MainController::handleFetchWebsite()
{

    if (server.hasArg("url")) {
        String url = server.arg("url");
        Serial.println("Fetching content from URL: " + url);
        fetchHTMLFromWebsite(url);
        server.send(200, "text/html", "<!DOCTYPE html><html><body><h1>Content Fetched!</h1><p>Check the Serial Monitor for raw HTML content.</p></body></html>");
    } else {
        server.send(400, "text/html", "<!DOCTYPE html><html><body><h1>Error</h1><p>URL parameter is missing.</p></body></html>");
    }

}// end of MainController::handleFetchWebsite
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::handleNotFound
//
/**
 *
 *
 *
 */

void MainController::handleNotFound()
{

    Serial.println("Unhandled request: " + server.uri());
    if (server.uri() == "/favicon.ico") {
        server.send(204);
    } else {
        server.sendHeader("Location", "/", true);
        server.send(302, "text/plain", "Redirecting to root");
    }

}// end of MainController::handleNotFound
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::clearAllPersistentData
//
/**
 *
 *
 *
 */

void MainController::clearAllPersistentData()
{

    Serial.println("Clearing all persistent data.");

    // Open the preferences namespace
    if (!preferences.begin("tattler-app", false)) {
        Serial.println("Failed to open preferences namespace");
        return;
    } else {
        Serial.println("Preferences successfully opened");
    }

    // Clear all preferences in the namespace
    if (preferences.clear()) {
        Serial.println("Preferences cleared successfully");
    } else {
        Serial.println("Failed to clear preferences");
    }

    // Close the preferences namespace
    preferences.end();

}// end of MainController::clearAllPersistentData
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::startSerialPort
//
/**
 *
 * Starts the serial port.
 *
 *  A lot of Arduino examples wait for the Serial port using:
 *    while(!Serial){}
 *
 * This causes some processors to hang up forever if the Serial is not available, but a delay
 * is required before using the port if you don't wait for it.
 *
 */

void MainController::startSerialPort()
{

    Serial.begin(115200);
    delay(3000);

}// end of MainController::startSerialPort
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::process
//
/**
 *
 * This function should be called by the Arduino 'loop' function which is in the .ino file for
 * Arduino IDE projects and in the main.cpp file for VSCode projects.
 *
 */

void MainController::process()
{

    // clearAllPersistentData(); // enable this function to clear all persistent data

    if (WiFi.getMode() == WIFI_AP) {
        // Process DNS requests only in AP mode
        dnsServer.processNextRequest();
    }

    server.handleClient(); // Handle web server requests

    moistureSensorHandlerPtr->process();

    onboardLEDHandler.process();

    buzzerHandler.process();

}// end of MainController::process
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// MainController::~MainController (destructor)
//
/**
 *
 * This gets called when the object is destroyed. All objects and memory allocated by this object
 * should be released here.
 *
 */

MainController::~MainController()
{

    Serial.print(className); Serial.println(": destructor called...");

}// end of MainController::~MainController (destructor)
//--------------------------------------------------------------------------------------------------

//end of class MainController
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
