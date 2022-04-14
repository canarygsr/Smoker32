
//#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>
//#include <ESP8266HTTPUpdateServer.h>
//#include <ESP8266WebServer.h>

//#include <WiFi.h>
//#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPUpdateServer.h>

#include <FS.h>
//#include "6json.h"

WebServer server(80);             // create a web server on port 80
HTTPUpdateServer httpUpdater;
File fsUploadFile;                                    // a File variable to temporarily store the received file

String getContentType(String filename) { // determine the filetype of a given filename, based on the extension
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}


/*__________________________________________________________SPIFFS_HANDLERS__________________________________________________________*/

/*void startSPIFFS() { // Start the SPIFFS and list all contents
  SPIFFS.begin();                             // Start the SPI Flash File System (SPIFFS)


  Serial.println("SPIFFS started. Contents:");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    Serial.print(dir.fileName());
    if (dir.fileSize()) {
      File f = dir.openFile("r");
      Serial.println(f.size());
    }
  }
}


void spiffscontence() {
  Serial.println("SPIFFS updated Contents below:");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    Serial.print(dir.fileName() + ":");
    if (dir.fileSize()) {
      File f = dir.openFile("r");
      Serial.println(f.size());
    }
  }
}


void writetospiffs() {
  //timenow ();
  File f = SPIFFS.open("/temp_log.csv", "a");
  if (!f) {
    Serial.println("file open failed");
  }
  else {

    Serial.println("====== Writing to SPIFFS file{i} =========");
    // write  strings to file
    f.print("[");
    f.print(timenow());
    for (int i = 0; i < NUM_MAX31856; i++) {
      f.print(", ");
     if (Probe[i].Present == true){
      f.print(Probe[i].CurrentTemp);
    }
    else 
    {f.print(Probe[i].Setpoint);}
    
  }
  f.println("],");
  }
  f.close();
  Serial.println("SPIFFS updated");

 
}

/*__________________________________________________________WEB_FUNCTIONS__________________________________________________________*/


bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if (LITTLEFS.exists(pathWithGz) || LITTLEFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
    if (LITTLEFS.exists(pathWithGz))                         // If there's a compressed version available
      path += ".gz";                                         // Use the compressed verion
    File file = LITTLEFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
  return false;
}

void handleFileUpload() { // upload a new file to the SPIFFS
  HTTPUpload& upload = server.upload();
  String path;
  if (upload.status == UPLOAD_FILE_START) {
    path = upload.filename;
    if (!path.startsWith("/")) path = "/" + path;
    if (!path.endsWith(".gz")) {                         // The file server always prefers a compressed version of a file
      String pathWithGz = path + ".gz";                  // So if an uploaded file is not compressed, the existing compressed
      if (LITTLEFS.exists(pathWithGz))                     // version of that file must be deleted (if it exists)
        LITTLEFS.remove(pathWithGz);
    }
    Serial.print("handleFileUpload Name: "); Serial.println(path);
    fsUploadFile = LITTLEFS.open(path, "w");               // Open the file for writing in SPIFFS (create if it doesn't exist)
    path = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {                                   // If the file was successfully created
      fsUploadFile.close();                               // Close the file again
      Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
      server.sendHeader("Location", "/success.html");     // Redirect the client to the success page
      server.send(303);
    } else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}

void handleNotFound() { // if the requested file or page doesn't exist, return a 404 not found error
  if (!handleFileRead(server.uri())) {        // check if the file exists in the flash memory (SPIFFS), if so, send it
    server.send(404, "text/plain", "404: File Not Found");
  }
}



void handlesettings() {
  for ( int i = 0; i < NUM_MAX31856; i++ ) {
    //char ProbeName = "Probe" + i + "Name";
    Probe[i].ProbeName = server.arg("Probe" + String(i + 1) + "Name"); //.toInt(); // remove toInt
    Probe[i].Setpoint = server.arg("Probe" + String(i + 1) + "SetTemp").toInt();
    Probe[i].ProbePreWarning = server.arg("Probe" + String(i + 1) + "PreWarning").toInt();
    Probe[i].ProbeOTWarning = server.arg("Probe" + String(i + 1) + "OTWarning").toInt();
    if (server.arg("Probe" + String(i + 1) + "Used") == "on")
    {
      Probe[i].Present = 1;
    }
    else
    {
      Probe[i].Present = 0;
    }

    if (server.arg("Probe" + String(i + 1) + "PitProbe") == "on") {
      Probe[i].Pit = 1;
    }
    else
    {
      Probe[i].Pit = 0;
    }
  }

SaveBBQsettings("/BBQSettings.json");
  server.send(200, "text/plain", "Settings updated press back");
}

void startServer() { // Start a HTTP server with a file read handler and an upload handler

  server.on("/edit.html",  HTTP_POST, []() {  // If a POST request is sent to the /edit.html address,
    server.send(200, "text/plain", "");
  }, handleFileUpload);                       // go to 'handleFileUpload'

  server.onNotFound(handleNotFound);          // if someone requests any other file or page, go to function 'handleNotFound'
  // and check if the file exists

  server.on("/restart", []() {
    server.send(200, "text/plain", "Restarting...");
    delay(1000);
    ESP.restart();
  });

/*  server.on("/setflag", []() {
    server.send(200, "text/plain", "Setting flag...");
    ota_flag = true;
    time_elapsed = 0;
  });
*/
  server.on("/del", []() {
    server.send(200, "text/plain", "Deleting files...");
    deleteFile(SPIFFS, "/temp_log.csv");
    listDir(SPIFFS, "/", 0);
    //server.send(200, "text/plain", "Restarting...");
  });

  server.on("/settings.html", []() {
    handlesettings();
  });

  server.on("/updatevalues.csv", []() {
    server.send(200, "text/plain", settingsstring);
  });

  server.on("/loadconfig", []() {
    server.send(200, "text/plain", settingsstring);
//    loadConfiguration();
  });

  server.on("/data/updatevalues.xml", []() {
    buildsettingxml();
    server.send(200, "text/xml", settingxml);
  });
  httpUpdater.setup(&server);
  server.begin();                             // start the HTTP server
  Serial.println("HTTP server started.");
}
