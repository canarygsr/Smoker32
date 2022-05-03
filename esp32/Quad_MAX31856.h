#include <MAX31856.h>

#define SDO    19//12
#define SDI    23//13
#define SCK    18//14
#define CS0    27//5
#define CS1    26//16
#define CS2    25//2
#define CS3    33//0
//#define NUM_MAX31856   4 // number of Max31856 chips to use
float  temperature;
float  tempreatureArray[NUM_MAX31856];
// MAX31856 Initial settings (see MAX31856.h and the MAX31856 datasheet)
// The default noise filter is 60Hz, suitable for the USA
#define CR0_INIT  (CR0_AUTOMATIC_CONVERSION + CR0_OPEN_CIRCUIT_FAULT_TYPE_K /* + CR0_NOISE_FILTER_50HZ */)
#define CR1_INIT  (CR1_AVERAGE_2_SAMPLES + CR1_THERMOCOUPLE_TYPE_K)
#define MASK_INIT (~(MASK_VOLTAGE_UNDER_OVER_FAULT + MASK_THERMOCOUPLE_OPEN_FAULT))

// array that holds current temp readings


// Create the temperature object, defining the pins used for communication
MAX31856 *TemperatureSensor[NUM_MAX31856] = {
  new MAX31856(SDI, SDO, CS0, SCK),
  new MAX31856(SDI, SDO, CS1, SCK),
  new MAX31856(SDI, SDO, CS2, SCK),
  new MAX31856(SDI, SDO, CS3, SCK)
};

void setupQuad_MAX31856() {
  Serial.println("MAX31856 Setup Started");
  // Initializing the MAX31855's registers
  for (int i = 0; i < NUM_MAX31856; i++) {
    TemperatureSensor[i]->writeRegister(REGISTER_CR0, CR0_INIT);
    TemperatureSensor[i]->writeRegister(REGISTER_CR1, CR1_INIT);
    TemperatureSensor[i]->writeRegister(REGISTER_MASK, MASK_INIT);
  }
  Serial.println("finish define pins");
  // Wait for the first samples to be taken
  delay(200);
  Serial.println("MAX31856 setup finished");
}
/*

  // Print the temperature, or the type of fault
  void printTemperature(double temperature) {
  switch ((int) temperature) {
    case FAULT_OPEN:
      Serial.print("FAULT_OPEN");
      break;
    case FAULT_VOLTAGE:
      Serial.print("FAULT_VOLTAGE");
      break;
    case NO_MAX31856:
      Serial.print("NO_MAX31856");
      break;
    default:
      Serial.print(temperature);
      break;
  }
  Serial.print(" ");
  }

  void loopQuad_MAX31856 () {
  for (int i=0; i<NUM_MAX31856; i++) {
    // Display the junction (IC) temperature first
    // Sometimes the junction temperature is not provided until a thermocouple is attached
    double temperature = TemperatureSensor[i]->readJunction(CELSIUS);
    if (temperature == NO_MAX31856)
      continue;
    Serial.print("J");
    Serial.print(i);
    Serial.print("=");
    printTemperature(temperature);

    // Display the thermocouple temperature
    temperature = TemperatureSensor[i]->readThermocouple(CELSIUS);
    if (temperature == NO_MAX31856)
      continue;
    Serial.print("T");
    Serial.print(i);
    Serial.print("=");
    printTemperature(temperature);

    Serial.print("\t");
  }

  Serial.println();
  delay(1000);
  }
*/


void GettempQuad_MAX31856 () {
 //delay(50);
  //   save temperature into array
  for (int i = 0; i < NUM_MAX31856; i++) {
    // Display the thermocouple temperature
    tempreatureArray[i] = TemperatureSensor[i]->readThermocouple(CELSIUS);
    if (tempreatureArray[i] >10000) {tempreatureArray[i] = -1;}
 //delay(50); 
 }
}

void readQuad_MAX31856 (int probenumber) {
  GettempQuad_MAX31856 ();
  temperature = tempreatureArray[probenumber];
  switch ((int) temperature) {
    case FAULT_OPEN:
      Serial.print("FAULT_OPEN");
      break;
    case FAULT_VOLTAGE:
      Serial.print("FAULT_VOLTAGE");
      break;
    case NO_MAX31856:
      Serial.print("NO_MAX31856");
      break;
    default:
      Serial.print (temperature);
      break;
  }
  Serial.println(" ");
}
