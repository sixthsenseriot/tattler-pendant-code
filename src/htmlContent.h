#ifndef _HTML_CONTENT_H
#define _HTML_CONTENT_H

#include <Arduino.h>

class HtmlPage 
{

public:


    inline static const String CAPTIVE_PORTAL_HTML_PAGE_HEADER = R"rawliteral(
<!DOCTYPE html>
    <html>
    <head>
    <title>Tattler Setup Page</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 2em; }
        h1 { color: #333; }
        form { max-width: 400px; margin: auto; text-align: left; }
        label { display: block; margin: 8px 0 4px; font-weight: bold; }
        input { width: 100%; padding: 8px; margin-bottom: 16px; border: 1px solid #ccc; border-radius: 4px; }
        button { padding: 10px 15px; background-color: #007bff; color: white; border: none; border-radius: 4px; cursor: pointer; }
        button:hover { background-color: #0056b3; }
        form {
        margin: 0;
        }

        .checkbox-container {
            display: flex;
            align-items: center;
            gap: 5px;
        }
        .checkbox-container input[type="checkbox"] {
            width: auto;
            margin-bottom: 0;
        }
    </style>
    </head>
    <body>


    <img id="DogInDiaperLogo" alt="Dog in Diaper" style="width:100px; height:auto;" />


        <h1>Welcome to the Tattler</h1>
    <p>Please configure your device by filling out the form below:</p>
    <form action="/save" method="POST">
        <label for="wifiName">Your Wi-Fi Name</label>
        <input type="text" id="wifiName" name="wifiName" required>
        
        <label for="wifiPassword">Your Wi-Fi Password</label>
        <input type="password" id="wifiPassword" name="wifiPassword" required>

        <div class="checkbox-container">
            <input type="checkbox" id="showPasswordButton" onclick="showPassword()">
            <label for="showPasswordButton">Show Password</label>
        </div>

        <br>

        <label for="petName">Pet Name</label>
        <input type="text" id="petName" name="petName" required>
        
        <label for="accountNumber">Your Tattler Account Number</label>
        <input type="text" id="accountNumber" name="accountNumber" required>
        
        <button type="submit">Save</button>
        <div id="error-message"></div>
        <br>
        <br>
        <br>
        <br>
        <br>
        <br>
        <br>
        <br>
        <br>
        <br>
        <p>Version: 2501171534.1</p>
    </form>
    <script>
    function showPassword() {
    var x = document.getElementById("wifiPassword");
    if (x.type === "password") {
        x.type = "text";
    } else {
        x.type = "password";
    }
    }
    </script>

)rawliteral";
 


inline static const String CAPTIVE_PORTAL_HTML_PAGE_FOOTER = R"rawliteral(



    <!-- 3) The script that sets the <img> .src -->
    <script>
    document.addEventListener('DOMContentLoaded', function () {
        const base64Data = document.getElementById('hiddenBase64').textContent.trim();
        const imgElem = document.getElementById('DogInDiaperLogo');
        imgElem.src = 'data:image/jpeg;base64,' + base64Data;
    });
    </script>


    </body>
    </html>
    )rawliteral";

inline static const String CAPTIVE_PORTAL_INITIALIZED_HTML_PAGE_HEADER = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
    <title>TATTLER INITIALIZED PAGE</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 2em; }
        h1 { color: #333; }
        form { max-width: 400px; margin: auto; text-align: left; }
        label { display: block; margin: 8px 0 4px; font-weight: bold; }
        input { width: 100%; padding: 8px; margin-bottom: 16px; border: 1px solid #ccc; border-radius: 4px; }
        button { padding: 10px 15px; background-color: #007bff; color: white; border: none; border-radius: 4px; cursor: pointer; }
        button:hover { background-color: #0056b3; }
        form {
        margin: 0;
        }
    </style>
    </head>
    <body>
    <center><h1>THE TATTLER IS ON THE JOB!</h1></center>
)rawliteral";

inline static const String CAPTIVE_PORTAL_INITIALIZED_HTML_PAGE_FOOTER = R"rawliteral(
    
    </body>
    </html>
)rawliteral";

inline static const String DATA_SAVED_HTML_PAGE = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>Data Saved</title>
<style>
    body { font-family: Arial, sans-serif; margin: 2em; }
    h1 { color: #333; }
    form { max-width: 400px; margin: auto; text-align: left; }
    label { display: block; margin: 8px 0 4px; font-weight: bold; }
    input { width: 100%; padding: 8px; margin-bottom: 16px; border: 1px solid #ccc; border-radius: 4px; }
    button { padding: 10px 15px; background-color: #007bff; color: white; border: none; border-radius: 4px; cursor: pointer; }
    button:hover { background-color: #0056b3; }
    form {
    margin: 0;
    }
</style>
</head>
<body>
    <h1>Data Saved!</h1>
    <p>Configuration was saved successfully.</p>
</body>
</html>
)rawliteral";

inline static const String ADMIN_PORTAL_HTML_PAGE = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>Admin Portal</title>
<style>
    body { font-family: Arial, sans-serif; margin: 2em; }
    h1 { color: #333; }
    form { max-width: 400px; margin: auto; text-align: left; }
    label { display: block; margin: 8px 0 4px; font-weight: bold; }
    input { width: 100%; padding: 8px; margin-bottom: 16px; border: 1px solid #ccc; border-radius: 4px; }
    button { padding: 10px 15px; background-color: #007bff; color: white; border: none; border-radius: 4px; cursor: pointer; }
    button:hover { background-color: #0056b3; }
    form {
    margin: 0;
    }
</style>
</head>
<body>
    <h1>Admin Portal</h1>
     <a href="/sendTattlerSMS.html?user=Khanh" id="navigateLink">
        <button>SMS Khanh</button>
    </a> 
    <a href="/sendTattlerSMS.html?user=Mike" id="navigateLink">
        <button>SMS Mike</button>
    </a>
    <a href="/sendTattlerSMS.html?user=Debra" id="navigateLink">
        <button>SMS Debra</button>
    </a>  
</body>
</html>
)rawliteral";

inline static const String SUCCESS_HTML_PAGE = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>Success</title>
<style>
    body { font-family: Arial, sans-serif; margin: 2em; }
    h1 { color: #333; }
    form { max-width: 400px; margin: auto; text-align: left; }
    label { display: block; margin: 8px 0 4px; font-weight: bold; }
    input { width: 100%; padding: 8px; margin-bottom: 16px; border: 1px solid #ccc; border-radius: 4px; }
    button { padding: 10px 15px; background-color: #007bff; color: white; border: none; border-radius: 4px; cursor: pointer; }
    button:hover { background-color: #0056b3; }
    form {
    margin: 0;
    }
</style>
</head>
<body>
    <h1>SUCCESS!</h1>
</body>
</html>
)rawliteral";

// Use this command Windows Powershell to encode an image as Base64 - Works with jpg, gif, png
// [Convert]::ToBase64String((Get-Content "image.jpg" -Encoding byte)) | Out-File -NoNewline base64.txt

inline static const String BASE64_IMAGE_STRING_TATTLER = R"rawliteral(
    <div id="hiddenBase64" style="display: none;">


   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

      </div>
)rawliteral";

}; // end of class HtmlPage

#endif // _HTML_CONTENT_H