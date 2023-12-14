// Load Wi-Fi library
#include <WiFi.h>
#include <SPIFFS.h>
#include <WiFiSettings.h>

#include "ADValToTA.h"
#include "ADHandler.h"

#if !defined( ESP32 )
	#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#define SIMU

// Replace with your network credentials
String hostname = "dose1";
int    portOfServer = 80;
String ssid = "";
String password = "";

int16_t  m_arADVal[] = { -20200, -19451, -18664, -6690, 2350, 9459, 14452, 18465, 18740, 20192 };
int val = 0;

// Set web server port number to 80
WiFiServer* pServer = NULL;

// ADS1115 handler
CADHandler adHandler;

// Auxiliar variables to store the current output state

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


void WiFiSettingsSetup()
{

    // Use stored credentials to connect to your WiFi access point.
    // If no credentials are stored or if the access point is out of reach,
    // an access point will be started with a captive portal to configure WiFi.
   WiFiSettings.hostname = "TATransmitter";
 
  hostname = WiFiSettings.string( "server_host", hostname);
  portOfServer = WiFiSettings.integer("server_port", portOfServer);


 
 WiFiSettings.connect(true,30);

  ssid = WiFiSettings.string("wifi-ssid");
  password = WiFiSettings.string("wifi-password");


}

void setup() {
  Serial.begin(115200);

  SPIFFS.begin(true);  // Will format on the first run after failing to mount



    // Use stored credentials to connect to your WiFi access point.
    // If no credentials are stored or if the access point is out of reach,
    // an access point will be started with a captive portal to configure WiFi.
 WiFiSettings.hostname = ssid;
  //WiFiSettings.onConnect = &SettingConnect;
 
  WiFiSettingsSetup();

  setupOrginal();

}
void setupOrginal() {
  // Serial.begin(115200);


  // // Initialize the output variables as outputs
  // // Set outputs to LOW

  // // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.disconnect(true,true);

  WiFi.setHostname(hostname.c_str());
  WiFi.enableAP(true);
 //WiFi.softAPConfig(local_IP, gateway, subnet)
  WiFi.softAP(ssid, password);
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);
  int nRepeat = 0;
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
     nRepeat ++;
     if (nRepeat > 50)
      break;
   }
   if (WiFi.status() != WL_CONNECTED) {
      WiFiSettingsSetup();
   }
  // //Print local IP address and start web server
   Serial.println("");
   Serial.println("WiFi connected.");
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
   Serial.println("Hostname: ");
   Serial.println(WiFiSettings.hostname);
   pServer = new WiFiServer (portOfServer);
  delay(100);
  pServer->begin();
  
#ifndef SIMU
  adHandler.init();
#endif
  delay(1000);
  Serial.println("ende setup");
  
}

void loop(){

    WiFiClient client = pServer->available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    while (client.connected() )
    {
      while (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
      }
#ifdef SIMU
      if (1)
#else
      if (adHandler.hasValue())
#endif
      {
        currentTime = millis();

        static float taVal = 0.0;
        static int index = 0;
#ifdef SIMU
        val = m_arADVal[index];
        index = (index + 1)%sizeof(m_arADVal);
#else
        val = adHandler.getADValue();
#endif
        taVal = CADValToTA::getTA(val);
        String s (taVal);
        client.println(s);
#ifdef SIMU
        Serial.print("delta ms: ");
        Serial.println(currentTime - previousTime);
        Serial.print(" In: " ); Serial.print( val ); Serial.print(" Out: " );
        Serial.println(s);
#endif
        previousTime = currentTime; 

        //delay(1);
      } 
#ifdef SIMU
      delay(1000); // 1 Werte die Sekunde
#else
      delay(20); // 20 ca 33 Werte die Sekunde
#endif
      
    }
  }
  delay(1000);
}