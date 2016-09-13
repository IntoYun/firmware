#include "application.h"

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

void setup() {
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  //WiFi.hasCredentials();  // Problem 1: never connects. Comment out to fix.
	Serial.print(".");
  if (IntoRobot.connected() == false) {
    Serial.println("Connecting");
    IntoRobot.connect();
  } else {
    WiFi.hasCredentials();  // Problem 2: dropped connections. Comment out to fix.
  }
}
