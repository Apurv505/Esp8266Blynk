/*************************************************************
Download latest Blynk library here:
https://github.com/blynkkk/blynk-library/releases/latest

Note: This requires ESP8266 support package:
https://github.com/esp8266/Arduino
*************************************************************/
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

char auth[] = "auth_token"; //Put Your Auth Token Here

//Array to store Network Credentials for different WiFi Networks
const char* KNOWN_SSID[] = {"ssid1", "ssid2", "ssid3"}; //Put all Your WiFi Network Names
const char* KNOWN_PASSWORD[] = {"pass1", "pass2", "pass3"}; //Put the WiFi Passwords in same order. For Open networks leave the password blank inside the double quotes.

const int KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]); // number of known networks


// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  // Request Blynk server to re-send latest values for all pins
  Blynk.syncAll();

  // You can also update individual virtual pins like this:
  //Blynk.syncVirtual(V0, V2);

  // Let's write your hardware uptime to Virtual Pin 2
  int value = millis() / 1000;
  Blynk.virtualWrite(V2, value);
}

BLYNK_WRITE(V0)
{
  // Use of syncAll() will cause this function to be called
  // Parameter holds last slider value
  int sliderValue0 = param.asInt();
}

BLYNK_WRITE(V2)
{
  // You'll get uptime value here as result of syncAll()
  int uptime = param.asInt();
}


void setup() {
boolean wifiFound = false;
int i, n;

Serial.begin(9600);

// ----------------------------------------------------------------
// Set WiFi to station mode and disconnect from an AP if it was previously connected
// ----------------------------------------------------------------
WiFi.mode(WIFI_STA);
WiFi.disconnect();
delay(100);
Serial.println("Setup done");

// ----------------------------------------------------------------
// WiFi.scanNetworks will return the number of networks found
// ----------------------------------------------------------------
Serial.println(F("Scan start"));
int nbVisibleNetworks = WiFi.scanNetworks();
Serial.println(F("Scan Completed"));
if (nbVisibleNetworks == 0) {
Serial.println(F("No networks found. Reset to try again"));
while (true); // no need to go further, hang in there, will auto launch the Soft WDT reset
}

// ----------------------------------------------------------------
// if you arrive here at least some networks are visible
// ----------------------------------------------------------------
Serial.print(nbVisibleNetworks);
Serial.println(" network(s) found");

// ----------------------------------------------------------------
// check if we recognize one by comparing the visible networks
// one by one with our list of known networks
// ----------------------------------------------------------------

for (i = 0; i < nbVisibleNetworks; ++i) {
Serial.println(WiFi.SSID(i)); // Print current SSID
for (n = 0; n < KNOWN_SSID_COUNT; n++) { // walk through the list of known SSID and check for a match
if (strcmp(KNOWN_SSID[n], WiFi.SSID(i).c_str())) {
Serial.print(F("\tNot matching "));
Serial.println(KNOWN_SSID[n]);
} else { // we got a match
wifiFound = true;
break; // n is the network index we found
}
} // end for each known wifi SSID
if (wifiFound) break; // break from the "for each visible network" loop
}

if (!wifiFound) {
Serial.println(F("No Known Network identified. Reset to try again"));
while (true);

}

const char* ssid = (KNOWN_SSID[n]);
const char* pass = (KNOWN_PASSWORD[n]);
Serial.println(WiFi.localIP());
Blynk.begin(auth, ssid, pass);
Serial.println("Blynk Connected"); //Connected and Authenticated with Blynk Server
}

void loop()
{
Blynk.run();
}          

