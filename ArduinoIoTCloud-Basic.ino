/*
  This sketch demonstrates how to exchange data between your board and the Arduino IoT Cloud.

  * Connect a potentiometer (or other analog sensor) to A0.
  * When the potentiometer (or sensor) value changes the data is sent to the Cloud.
  * When you flip the switch in the Cloud dashboard the onboard LED lights gets turned ON or OFF.

  IMPORTANT:
  This sketch works with WiFi, GSM, NB and Lora enabled boards supported by Arduino IoT Cloud.
  On a LoRa board, if it is configuered as a class A device (default and preferred option), values from Cloud dashboard are received
  only after a value is sent to Cloud.

  This sketch is compatible with:
   - MKR 1000
   - MKR WIFI 1010
   - MKR GSM 1400
   - MKR NB 1500
   - MKR WAN 1300/1310
   - ESP 8266
*/

#include "arduino_secrets.h"
#include "thingProperties.h"
#include <Adafruit_BMP280.h>

#define PIN_BRIGHT A0

Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor(); //get temperature data
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor(); //get pressure data

//float Temp, Alt, Pres, brightness;
int brightSensor;


float light;
float pressure;
float temperatura;

void setup() {
  /* Initialize serial and wait up to 5 seconds for port to open */
  Serial.begin(9600);
  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }

  /* Configure LED pin as an output */
  pinMode(LED_BUILTIN, OUTPUT);

  /* This function takes care of connecting your sketch variables to the ArduinoIoTCloud object */
  initProperties();

  /* Initialize Arduino IoT Cloud library */
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(DBG_INFO);
  ArduinoCloud.printDebugInfo();
  //Init BMP280 config
  Serial.println(F("BMP280 Sensor event test"));

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  bmp_temp->printSensorDetails();
  
  //INIT ADC
  pinMode(PIN_BRIGHT, INPUT);
  brightSensor = analogRead(PIN_BRIGHT);
  
}

void loop() {
  ArduinoCloud.update();
  //potentiometer = analogRead(A0);
  //seconds = millis() / 1000;

  brightSensor = analogRead(PIN_BRIGHT);
  light = 500 / (10.0 * (3.3 - (float)brightSensor / 1023 * 3.3) / ((float)brightSensor / 1023 * 3.3));
  
    float temperatura = bmp.readTemperature()*9/5 + 32 ; //Conversion from C to F
    Serial.print(F("Temperature = "));    
    Serial.print(temperatura);
    Serial.println(" *F");
    
    float pressure = bmp.readPressure()/3386.39;   //displaying the Pressure in inches of mercury, you can change the unit
    Serial.print(F("Pressure = "));
    Serial.print(pressure);
    Serial.println(" inHg");

    float Alt = bmp.readAltitude(1019.66)*3.28084;   //The "1019.66" is the pressure(hPa) at sea level in day in your region
    Serial.print(F("Approx altitude = "));        //If you don't know it, modify it until you get your current altitude
    Serial.print(Alt);
    Serial.println(" ft");

    Serial.println();
    delay(2000);
}

/*
 * 'onLedChange' is called when the "led" property of your Thing changes
 */
void onLedChange() {
  Serial.print("LED set to ");
  Serial.println(led);
  digitalWrite(LED_BUILTIN, led);
}
