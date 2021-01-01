

/*------------------------------------------------------------------------------
  10/01/2018
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: simple_ota.ino
  ------------------------------------------------------------------------------
  ------------------------------------------------------------------------------*/

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <SimpleTimer.h>

#define NUM_MAX31856   4 // number of Max31856 chips to use

#include "Quad_MAX31856.h"
#include "zBBQ_Functions.h"
#include "5timer.h"
#include "3SPIFFS.h"

  /* ================================================== Timer functions =========================================================================== */

  // stagger the intervals to allow time for the server messages to complete. Also, use primes in case that helps.
#define PROBE_DELAY     1000      // how often probe temperatures are read in msec
#define TEMP_DELAY      5000      // how often dashboard temperature display values are pushed
#define LCD_DELAY       3000      // how often physical LCD is updated (slow enough to minimize flicker)
#define LOG_DELAY       30000      // how often data is saved to the log file (should add code that checks for more than delta x change)
#define PUSH_DELAY      30000     // how often push notifications for setpoint reached are done in msec


  SimpleTimer timer;


void setup() {

  //pinMode(2, OUTPUT);
  Serial.begin(115200);
  Serial.println("Booting");
  startSPIFFS();
  delay(500);
  loadConfiguration();
  
      Serial.println(config.client_SSID);
   WiFi.hostname(config.hostName);
  //WiFi.mode(WIFI_STA);
  WiFi.mode(WIFI_AP_STA);
  WiFi.hostname(config.hostName);
  WiFi.softAP(config.AP_SSID, config.AP_password);
  WiFi.begin(config.client_SSID, config.client_password);
   //not tried yet
    if(strlen(config.client_SSID) == 0) {
    Serial.println("Client Mode not Setup, go to 192.168.1.4...");
  } else {                
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
   ESP.restart();
  }
  }
 Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Ready");


  setupQuad_MAX31856();

//  setupOTA();

                 // Start the SPIFFS and list all contents

  startMDNS();                 // Start the mDNS responder

  startServer();               // Start a HTTP server with a file read handler and an upload handler

  startUDP();                  // Start listening for UDP messages to port 123

 // Blynksetup();                // Start Blynk Server

  WiFi.hostByName(ntpServerName, timeServerIP); // Get the IP address of the NTP server
  Serial.print("Time server IP:\t");
  Serial.println(timeServerIP);
  ntpstuff(); // run early to ge the time
  //sendNTPpacket(timeServerIP);


  Serial.println("simple timer started");

  timer.setInterval(PROBE_DELAY, GettempQuad_MAX31856);
  timer.setInterval(PROBE_DELAY, updateProbes);
//  timer.setInterval(PUSH_DELAY, updatesettings);
  timer.setInterval(LOG_DELAY, writetospiffs);
  //timer.setInterval(LCD_DELAY, BlynkPush);
  timer.setInterval(60000, ntpstuff);
  //timer.setInterval(60000, makesettingsjson);


}



void loop() {
  server.handleClient();
//  OTAloop();
//  Serial.println();
//  updateProbes();
//  GettempQuad_MAX31856();
//readQuad_MAX31856(0);
 
//  readQuad_MAX31856(3);
 
//  readQuad_MAX31856(2);
 
//  readQuad_MAX31856(1);

//digitalWrite(2, !digitalRead(2));
//  delay(500);
  timer.run();
}
