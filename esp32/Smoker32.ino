#include <WiFi.h>
#include <SimpleTimer.h>

#define NUM_MAX31856   4 // number of Max31856 chips to use
#include "Quad_MAX31856.h"
#include "BBQ_Functions.h"
#include "littleFS.h"
#include "json.h"
#include "timer.h"
#include "webserver.h"
#include "LCD.h"


// stagger the intervals to allow time for the server messages to complete. Also, use primes in case that helps.
#define PROBE_DELAY     10000      // how often probe temperatures are read in msec
#define TEMP_DELAY      5000      // how often dashboard temperature display values are pushed
#define LCD_DELAY       500      // how often physical LCD is updated (slow enough to minimize flicker)
#define LOG_DELAY       30000      // how often data is saved to the log file (should add code that checks for more than delta x change)
#define PUSH_DELAY      30000     // how often push notifications for setpoint reached are done in msec
SimpleTimer timer;



void setup() {

  Serial.begin(115200);
  Serial.println("Booting");
  setuplcd();
  startlittleFS();
  setupQuad_MAX31856();
  delay(500);
loadConfiguration("/config.json", config);
LoadBBQsettings("/BBQSettings.json", config);
SaveBBQsettings("/BBQSettings.json");

    Serial.print("Connecting to ");
    Serial.println(config.client_SSID);
    Serial.println(config.client_password);
    debugtoLCD(config.client_SSID);
    Serial.println("WiFi.begin");
    WiFi.begin(config.client_SSID, config.client_password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

//  setupOTA();
  startMDNS();                 // Start the mDNS responder
  startServer();               // Start a HTTP server with a file read handler and an upload handler
  startUDP();                  // Start listening for UDP messages to port 123
  WiFi.hostByName(ntpServerName, timeServerIP); // Get the IP address of the NTP server
  Serial.print("Time server IP:\t");
  Serial.println(timeServerIP);
  ntpstuff(); // run early to ge the time
  //sendNTPpacket(timeServerIP);


  Serial.println("simple timer started");

  timer.setInterval(PROBE_DELAY, GettempQuad_MAX31856);
  timer.setInterval(PROBE_DELAY, updateProbes);
//  timer.setInterval(PUSH_DELAY, updatesettings);
//  timer.setInterval(LOG_DELAY, timenow);
//  timer.setInterval(LOG_DELAY, writetospiffs);
  timer.setInterval(LCD_DELAY, updatedisplay);
  timer.setInterval(60000, ntpstuff);
//  timer.setInterval(6000, writeFile(SPIFFS, "/config.json", makesettingsjson());


}



void loop() {
  server.handleClient();
  timer.run();
  //Serial.println(timenow());
}
