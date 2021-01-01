String settingsstring;
String settingxml;


//double temp;
float temp;
bool  redrawScreen = false;
int   alarm;
struct Config {
  char MDSNname[64];
  char password[64];
  char client_SSID[64];
  char client_password[64];
  char AP_SSID[64];
  char AP_password[64];
  char hostName[64];
};
Config config;

typedef struct {
  uint16_t    Setpoint;         // setpoint
  uint16_t    ProbePreWarning;
  uint16_t    ProbeOTWarning;
  String    ProbeName;  // int
  //const char* ProbeName;
  //char ProbeName[10];
  uint16_t    SavedSetpoint;        // copy to save for a disconnected probe
  float       CurrentTemp;        // current temperature.
  uint8_t     SetpointReached  : 1;  // if set temperature has been reached
  uint8_t     Present          : 1;  // true if probe is connected
  uint8_t     TempChanged      : 1;  // true if temp has changed - throttles updates to Blynk dashboard
  uint8_t     alarm     : 1;  // set after a reconnect event so updateSetpoint can sync the slider values
  uint8_t     OverTemp         : 1;      // if probe has exceeded tempreature by x percent
  uint8_t     ApproachSetpoint : 1;     // warning that probe is approaching set tempreature x percent
  uint8_t     Pit              : 1;
  uint8_t     PitColdWarning   : 1;       // pit probe warining that pit is cold!!
} ProbeType;

ProbeType Probe[NUM_MAX31856];
//SimpleTimer timer;

void updateProbes () {
  for ( int i = 0; i < NUM_MAX31856; i++ ) {
temp = tempreatureArray[i];
    
    if (temp > 1000) {
      Probe[i].CurrentTemp = temp; //Probe[i].Setpoint;
      Probe[i].Present = false;
      //Probe[i].Setpoint = 0;
      Probe[i].SetpointReached = false;
      //redrawScreen = true;
     
    }
    else {

    // get probe temps & handle disconnects/reconnects
    if ( temp != Probe[i].CurrentTemp ) {
      Probe[i].CurrentTemp = temp;
      Probe[i].TempChanged = true;
      Probe[i].Present = true;
    

  /*  if ( temp >= Probe[i].ProbePreWarning) {
      Probe[i].ApproachSetpoint = true;
    }
    else
    { Probe[i].ApproachSetpoint = false;
    }
    if ( temp >= Probe[i].Setpoint ) {
      Probe[i].SetpointReached = true;
    }
    else
    { Probe[i].SetpointReached = false;
    }
    if ( temp > (double) Probe[i].ProbeOTWarning) {
      Probe[i].OverTemp = true;
    }
    else
    { Probe[i].OverTemp = false;
    }
    if ( (temp < (double) Probe[i].ProbePreWarning) && Probe[i].Pit ) {
      Probe[i].PitColdWarning = true;
    }
    else
    { Probe[i].PitColdWarning = false;
    }
 */   
  }
  // /*
  if (Probe[i].CurrentTemp < Probe[i].ProbePreWarning)
{
  Probe[i].alarm = 0;
  alarm = 0; // no alarms for meat
    if (Probe[i].Pit == 1)
    { Probe[i].PitColdWarning = true;
      alarm = 1;}  // low pit/oven temp alarm
}else{ Probe[i].PitColdWarning = false;}
if ((Probe[i].CurrentTemp > Probe[i].ProbePreWarning) && (Probe[i].alarm != 1)) // will triger stall temp alarm
{
   Probe[i].ApproachSetpoint = true;
  if (alarm == 1) {alarm = 1;} else {alarm = 2;} 
Probe[i].alarm = 1;
}
else {Probe[i].ApproachSetpoint = false;}

if ((Probe[i].CurrentTemp > Probe[i].Setpoint) && (Probe[i].alarm != 2)) // will trigger set tempreatre alarm
{
  Probe[i].SetpointReached = true;
  alarm = 1;
  Probe[i].alarm = 2;
}
else {Probe[i].SetpointReached = false;}

if ((Probe[i].CurrentTemp > Probe[i].ProbeOTWarning) && (Probe[i].alarm != 3)) // trigger pit/oven over temp
{
Probe[i].OverTemp = true;
alarm = 1;
Probe[i].alarm = 3;
}
else {Probe[i].OverTemp = false;}
    }
 // */   
     
     
     Serial.print(i);          
   Serial.print("-");
      Serial.print(Probe[i].PitColdWarning);                      //For Debugging
   Serial.print("-");
      Serial.print(Probe[i].ApproachSetpoint);                      //For Debugging
   Serial.print("-");
      Serial.print(Probe[i].SetpointReached);                      //For Debugging
   Serial.print("-");
      Serial.print(Probe[i].OverTemp);                      //For Debugging      
   Serial.print("-");
      Serial.print(Probe[i].ProbeName);                      //For Debugging
   Serial.print("-"); 
      Serial.print(Probe[i].Setpoint);                      // setpoint
   Serial.print("-");
      Serial.print(Probe[i].ProbePreWarning);                      //For Debugging
   Serial.print("-");
      Serial.print(Probe[i].ProbeOTWarning);                      //For Debugging
   Serial.print("-");
      Serial.print(Probe[i].SavedSetpoint);                      //For Debugging
   Serial.print("-");
      Serial.print(Probe[i].CurrentTemp);                      //For Debugging
   Serial.print("-");
      Serial.print(Probe[i].Present);                      //For Debugging
   Serial.print("-");
      Serial.print(Probe[i].TempChanged);                      //For Debugging
   Serial.print("-");
      Serial.print(Probe[i].Pit);                      //For Debugging

   Serial.println();
 

     }   Serial.println(alarm);
}

void buildsettingxml (void) {
settingxml = "";
settingxml += "<?xml version='1.0' encoding='UTF-8'?>\n";
settingxml += "<settings>\n";
for ( int i = 0; i < NUM_MAX31856; i++ ) {
settingxml += "<probe>\n";
settingxml += "    <ProbeName>";
settingxml += Probe[i].ProbeName + "</ProbeName>\n";
settingxml += "    <ProbeSetTemp>" + String(Probe[i].Setpoint) + "</ProbeSetTemp>\n";
settingxml += "    <ProbePreWarning>" + String(Probe[i].ProbePreWarning) + "</ProbePreWarning>\n";
settingxml += "    <ProbeOTWarning>" + String(Probe[i].ProbeOTWarning) + "</ProbeOTWarning>\n";
settingxml += "    <ProbePitProbe>" + String(Probe[i].Pit) + "</ProbePitProbe>\n";
settingxml += "    <ProbeUsed>" + String(Probe[i].Present) + "</ProbeUsed>\n";
settingxml += "    <ProbeTemp>" + String(Probe[i].CurrentTemp) + "</ProbeTemp>\n";
settingxml += "</probe>\n";
}
settingxml += "</settings>\n";
//Serial.print(settingxml); //uncomment to view output
}
