// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <FS.h>


// Our configuration structure.
//
// Never use a JsonDocument to store the configuration!
// A JsonDocument is *not* a permanent storage; it's only a temporary storage
// used during the serialization phase.

//const size_t capacity = JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(5) + 3*JSON_OBJECT_SIZE(6) + 220;
const size_t capacity = 6*JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(10) + 190;
  DynamicJsonDocument doc(capacity);

void loadConfiguration() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
  }

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, configFile);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());

  }
  strlcpy(config.hostName,                  // <- destination
          doc["hostName"] | "default.example.com",  // <- source | and default value, change for
          sizeof(config.hostName));        // <- destination's capacity
  strlcpy(config.password, doc["password"] | "password_default", sizeof(config.password));
  strlcpy(config.client_password, doc["client_password"] | "client_password_default", sizeof(config.client_password));
  strlcpy(config.client_SSID, doc["client_SSID"] | "0", sizeof(config.client_SSID));
  strlcpy(config.hostName, doc["hostName"] | "hostName_default", sizeof(config.hostName));
  strlcpy(config.AP_SSID, doc["AP_SSID"] | "AP_SSID_default", sizeof(config.AP_SSID));
  strlcpy(config.AP_password, doc["AP_password"] | "AP_password_default", sizeof(config.AP_password));







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

  // Real world application would store these values in some variables for
  // later use.

  Serial.println("Loaded settings:");
buildsettingxml();
}


// Saves the configuration to a file

void saveConfiguration() {
  // Delete existing file, otherwise the configuration is appended to the file
  // Open file for writing
  File configfile = SPIFFS.open("/config.json", "w");
  if (!configfile) {
    Serial.println("file open failed");
  }
  serializeJson(doc, configfile);
  //file.print(Serial);
  configfile.close();
  //return;
  Serial.println("SPIFFS updated");
}


void makesettingsjson() {
//  const size_t capacity = JSON_ARRAY_SIZE(4) + 4 * JSON_OBJECT_SIZE(6);
//  DynamicJsonDocument doc(capacity);
  doc.clear();
  JsonObject settings = doc.to<JsonObject>();
  JsonArray Names = settings.createNestedArray("Name");
  JsonArray SetTemp = settings.createNestedArray("SetTemp");
  JsonArray PreWarning = settings.createNestedArray("PreWarning");
  JsonArray OTWarning = settings.createNestedArray("OTWarning");
  JsonArray PitProbe = settings.createNestedArray("Pit");
  JsonArray Used = settings.createNestedArray("Present");
for ( int i = 0; i < NUM_MAX31856; i++ ) {
  Names.add(Probe[i].ProbeName);
  SetTemp.add(Probe[i].Setpoint);
  PreWarning.add(Probe[i].ProbePreWarning);
  OTWarning.add(Probe[i].ProbeOTWarning);
  PitProbe.add(Probe[i].Pit);
  Used.add(Probe[i].Present);
}
  //save settiings to json 
  doc["hostName"] = config.hostName;
  doc["password"] = config.password;
  doc["client_SSID"] = config.client_SSID;
  doc["client_password"] = config.client_password;
  serializeJson(doc, Serial);
  Serial.println(Serial);
  saveConfiguration();

}

/*
bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    //return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  // StaticJsonDocument<512> json;
  const size_t capacity = JSON_OBJECT_SIZE(14)  + 830;;
  DynamicJsonDocument json(capacity);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  //  configFile.readBytes(buf.get(), size);

  // StaticJsonBuffer<200> jsonBuffer;
  //  JsonObject& json = jsonBuffer.parseObject(buf.get());
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(json, configFile);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonDocument to the Config
  strlcpy(config.MDSNname,                  // <- destination
          json["MDSNname"] | "default.example.com",  // <- source | and default value, change for
          sizeof(config.MDSNname));        // <- destination's capacity
  strlcpy(config.password, json["password"] | "password_default", sizeof(config.password));
  strlcpy(config.client_password, json["client_password"] | "client_password_default", sizeof(config.client_password));
  strlcpy(config.client_SSID, json["client_SSID"] | "client_SSID_default", sizeof(config.client_SSID));
  strlcpy(config.AP_SSID, json["AP_SSID"] | "AP_SSID_default", sizeof(config.AP_SSID));
  strlcpy(config.AP_password, json["AP_password"] | "AP_password_default", sizeof(config.AP_password));
  strlcpy(config.passcode0, json["passcode0"] | "passcode0_default", sizeof(config.passcode0));
  strlcpy(config.passcode1, json["passcode1"] | "passcode1_default", sizeof(config.passcode1));
  strlcpy(config.passcode2, json["passcode2"] | "passcode2_default", sizeof(config.passcode2));
  strlcpy(config.passcode3, json["passcode3"] | "passcod3_default", sizeof(config.passcode3));
  config.function1_gpio = json["function1_gpio"] | 1;
  config.function2_gpio = json["function2_gpio"] | 2;

  //Print settings to serial
  Serial.print("MDSNname: ");
  Serial.println(config.MDSNname);
  Serial.print("password: ");
  Serial.println(config.password);
  Serial.print("client_SSID: ");
  Serial.println(config.client_SSID);
  Serial.print("client_password: ");
  Serial.println(config.client_password);
  Serial.print("AP_SSID: ");
  Serial.println(config.AP_SSID);
  Serial.print("AP_password: ");
  Serial.println(config.AP_password);
  Serial.print("Passcode0: ");
  Serial.println(config.passcode0);
  Serial.print("Passcode1: ");
  Serial.println(config.passcode1);
  Serial.print("Passcode2: ");
  Serial.println(config.passcode2);
  Serial.print("Passcode3: ");
  Serial.println(config.passcode3);
  Serial.print("function1_gpio: ");
  Serial.println(config.function1_gpio);
  Serial.print("function2_gpio: ");
  Serial.println(config.function2_gpio);
  return true;
}

bool saveConfig() {
  const size_t capacity = JSON_OBJECT_SIZE(14) + 380;
  //StaticJsonDocument<512> json;
  DynamicJsonDocument json(capacity);

  //  JsonObject& json = jsonBuffer.createObject();
  json["MDSNname"] = config.MDSNname;
  json["password"] = config.password;
  json["client_SSID"] = config.client_SSID;
  json["client_password"] = config.client_password;
  json["AP_SSID"] = config.AP_SSID;
  json["AP_password"] = config.AP_password;
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  // Serialize JSON to file
  if (serializeJson(json, configFile) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  return true;
}
*/
