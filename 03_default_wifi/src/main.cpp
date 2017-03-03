#include <Arduino.h>
#include <debug.h>
#include <ESP8266WiFi.h>

// place your own WiFi credentials here
const char* ssid     = "ThingForwardWiFi";
const char* password = "thingforwardiot";

int num_seconds_to_sleep = 5;

void setup() {
	Serial.begin(9600);
	// this causes ESP Wifi code to write debug about
	// when connecting etc.
	Serial.setDebugOutput(true);

	Serial.println("setup()");

	pinMode(D0,WAKEUP_PULLUP);

	if ( WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while ( WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
    }
	}
}

void loop() {
  Serial.println("in loop()");
	// do something with WiFi here
	Serial.println("We're connected, do something with wifi..");
	delay(500);

  Serial.print("deep sleep, num_seconds="); Serial.println(num_seconds_to_sleep);
	// go do deep sleep, keep RF on
  ESP.deepSleep(num_seconds_to_sleep * 1000000, RF_DEFAULT);
}
