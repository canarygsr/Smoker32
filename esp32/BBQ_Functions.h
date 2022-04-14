String settingsstring;
String settingxml;


//double temp;
float temp;
bool  redrawScreen = false;
int   alm;
/*struct Config {
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
*/
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
  uint8_t     alm     : 1;  // set after a reconnect event so updateSetpoint can sync the slider values
  uint8_t     OverTemp         : 1;      // if probe has exceeded tempreature by x percent
  uint8_t     ApproachSetpoint : 1;     // warning that probe is approaching set tempreature x percent
  uint8_t     Pit              : 1;
  uint8_t     PitColdWarning   : 1;       // pit probe warining that pit is cold!!
  uint16_t    stringwidth;
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
  Probe[i].alm = 0;
  alm = 0; // no alarms for meat
    if (Probe[i].Pit == 1)
    { Probe[i].PitColdWarning = true;
      alm = 1;}  // low pit/oven temp alarm
}else{ Probe[i].PitColdWarning = false;}
if ((Probe[i].CurrentTemp > Probe[i].ProbePreWarning) && (Probe[i].alm != 1)) // will triger stall temp alarm
{
   Probe[i].ApproachSetpoint = true;
  if (alm == 1) {alm = 1;} else {alm = 2;} 
Probe[i].alm = 1;
}
else {Probe[i].ApproachSetpoint = false;}

if ((Probe[i].CurrentTemp > Probe[i].Setpoint) && (Probe[i].alm != 2)) // will trigger set tempreatre alarm
{
  Probe[i].SetpointReached = true;
  alm = 1;
  Probe[i].alm = 2;
}
else {Probe[i].SetpointReached = false;}

if ((Probe[i].CurrentTemp > Probe[i].ProbeOTWarning) && (Probe[i].alm != 3)) // trigger pit/oven over temp
{
Probe[i].OverTemp = true;
alm = 1;
Probe[i].alm = 3;
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
 

     }   Serial.println(alm);
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

void writetospiffs() {
  //timenow ();
 // File f = SPIFFS.open("/temp_log.csv", "a");
//  if (!f) {
//    Serial.println("file open failed");
//  }
//  else {

//    Serial.println("====== Writing to SPIFFS file{i} =========");
    // write  strings to file
    Serial.println("[");
    Serial.print("timenow"); //change to timenow()
    for (int i = 0; i < NUM_MAX31856; i++) {
      Serial.print(", ");
     if (Probe[i].Present == true){
      Serial.print(Probe[i].CurrentTemp);
    }
    else 
    Serial.print(Probe[i].Setpoint);}
    
  
  Serial.println("],");
  Serial.println("SPIFFS updated");

}

void maxstringlength(){
  int max_string_length_array[4];
  for(int i=0;i< 4 ;i++){
   Serial.println((Probe[i].ProbeName).length());
   max_string_length_array[i] = (Probe[i].ProbeName).length();
   Serial.println(max_string_length_array[i]);
  }
unsigned kmax=0;
unsigned smax=0;
for (byte k=0; k<4; k++)
  if ( max_string_length_array[k] > smax) {
     smax =  max_string_length_array[k];
     kmax = k;
  }
  //Serial.println(kmax);
  Serial.print("smax=");
  Probe[0].stringwidth = smax;
  Serial.println(Probe[0].stringwidth);
//  max now contains the largest spectral value.
// kmax contains the index to the largest spectral value.
  
}
