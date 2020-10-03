
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>


const char THING_ID[] = "250a70ea-83d3-479f-af9c-730f53fe6b1a";

char* SSID = SECRET_SSID;    // Network SSID (name)
const char *PASS = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)

void onSetAlarmChange();

float Alt;
float brightness;
float Pres;
float temperatura;
float setAlarm;
bool ledRojo;

void initProperties(){

  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(brightness, READ, 30 * SECONDS, NULL);
  ArduinoCloud.addProperty(Pres, READ, 30 * SECONDS, NULL);
  ArduinoCloud.addProperty(temperatura, READ, 30 * SECONDS, NULL);
   ArduinoCloud.addProperty(setAlarm, READWRITE, ON_CHANGE, onSetAlarmChange);
  ArduinoCloud.addProperty(ledRojo, READ, 30 * SECONDS, NULL);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
