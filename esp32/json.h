// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include "FS.h"
#include <LITTLEFS.h>

struct Config {
  char MDSNname[64];
  char password[64];
  char client_SSID[64];
  char client_password[64];
  char AP_SSID[64];
  char AP_password[64];
  char hostName[64];
  int stringwidth;
};
Config config;


// Our configuration structure.
//
// Never use a JsonDocument to store the configuration!
// A JsonDocument is *not* a permanent storage; it's only a temporary storage
// used during the serialization phase.

//const size_t capacity = JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(5) + 3*JSON_OBJECT_SIZE(6) + 220;
const size_t capacity = 6*JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(10) + 190;
  DynamicJsonDocument doc(capacity);

void loadConfiguration(const char * path, Config &config) {
String aLine;
File file = LITTLEFS.open(path, "r");
    if (!file) {
    Serial.print("Cannot open ");
    Serial.println(path);
    return;}
    while (file.available()) {
          aLine = file.readStringUntil('\n');
          // Serial.println(aLine);
          DeserializationError error = deserializeJson(doc, aLine);
            if (error) {Serial.print(F("deserializeJson() failed: "));
            
            } else {Serial.print(F("deserializeJson() status:"));
            }
            Serial.println(error.c_str());
                              }//while

  strlcpy(config.hostName,                  // <- destination
          doc["hostName"] | "default.example.com",  // <- source | and default value, change for
          sizeof(config.hostName));        // <- destination's capacity
  strlcpy(config.password, doc["password"] | "password_default", sizeof(config.password));
  strlcpy(config.client_password, doc["client_password"] | "client_password_default", sizeof(config.client_password));
  strlcpy(config.client_SSID, doc["client_SSID"] | "default", sizeof(config.client_SSID));
  strlcpy(config.hostName, doc["hostName"] | "hostName_default", sizeof(config.hostName));
  strlcpy(config.AP_SSID, doc["AP_SSID"] | "AP_SSID_default", sizeof(config.AP_SSID));
  strlcpy(config.AP_password, doc["AP_password"] | "AP_password_default", sizeof(config.AP_password));

}

void makesettingsjson(const char * path) {
//  const size_t capacity = JSON_ARRAY_SIZE(4) + 4 * JSON_OBJECT_SIZE(6);
//  DynamicJsonDocument doc(capacity);
  doc.clear();
  JsonObject settings = doc.to<JsonObject>();
  //save settiings to json 
  doc["hostName"] = config.hostName;
  doc["password"] = config.password;
  doc["client_SSID"] = config.client_SSID;
  doc["client_password"] = config.client_password;

  Serial.printf("Writing file: %s\r\n", path);
    File file = LITTLEFS.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(serializeJson(doc, file))){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void LoadBBQsettings(const char * path, Config &config) { //not checked it working yet
String aLine;
File file = LITTLEFS.open(path, "r");
    if (!file) {
    Serial.print("Cannot open ");
    Serial.println(path);
    return;}
    while (file.available()) {
          aLine = file.readStringUntil('\n');
          Serial.println(aLine);
          DeserializationError error = deserializeJson(doc, aLine);
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
                              }//while

 for ( int i = 0; i < NUM_MAX31856; i++ ) {

const char*  ProbeName = doc["Name"][i];
   Probe[i].ProbeName = ProbeName;
   //Probe[i].ProbeName = doc["Name"][i];  //why does this not work??
   //strlcpy(Probe[i].Setpoint, doc["SetTemp"][i] | 10, sizeof(Probe[i].Setpoint));
//   strlcpy(Probe[i].ProbeName, doc["Name"][i] | "Probe", sizeof(Probe[i].ProbeName)); //cant use char for some reason
   Probe[i].Setpoint = doc["SetTemp"[i]] | 65;
   Probe[i].ProbePreWarning = doc["PreWarning"][i] | 60;
   Probe[i].ProbeOTWarning = doc["OTWarning"][i] | 70;
   Probe[i].Pit = doc["PitProbe"][i] | 0;
   Probe[i].Present = doc["Used"][i] | 1;
}
  Probe[0].stringwidth = doc["maxstringlength"];
}

void SaveBBQsettings(const char * path) { //not checked it working yet
//  const size_t capacity = JSON_ARRAY_SIZE(4) + 4 * JSON_OBJECT_SIZE(6);
//  DynamicJsonDocument doc(capacity);
  maxstringlength();
  doc.clear();
  JsonObject settings = doc.to<JsonObject>();
  JsonArray Names = settings.createNestedArray("Name");
  JsonArray SetTemp = settings.createNestedArray("SetTemp");
  JsonArray PreWarning = settings.createNestedArray("PreWarning");
  JsonArray OTWarning = settings.createNestedArray("OTWarning");
  JsonArray PitProbe = settings.createNestedArray("Pit");
  JsonArray Used = settings.createNestedArray("Present");
  doc["maxstringlength"] = Probe[0].stringwidth; //capture the sring length
for ( int i = 0; i < NUM_MAX31856; i++ ) {
  Names.add(Probe[i].ProbeName);
  SetTemp.add(Probe[i].Setpoint);
  PreWarning.add(Probe[i].ProbePreWarning);
  OTWarning.add(Probe[i].ProbeOTWarning);
  PitProbe.add(Probe[i].Pit);
  Used.add(Probe[i].Present);
}
  
  
  Serial.printf("Writing file: %s\r\n", path);
    File file = LITTLEFS.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(serializeJson(doc, file))){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
    maxstringlength();
}
