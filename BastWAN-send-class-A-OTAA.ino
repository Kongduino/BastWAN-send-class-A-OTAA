/**
   Example of OTAA device
   Authors:
          Ivan Moreno
          Eduardo Contreras
    June 2019
   This code is beerware; if you see me (or any other collaborator
   member) at the local, and you've found our code helpful,
   please buy us a round!
   Distributed as-is; no warranty is given.
*/
#include <lorawan.h> // Beelan library
#include "Helper.h"
#include "secret.h"
// OTAA credentials - create a "secret.h" file with:
// const char *devEui = "0000000000000000";
// const char *appEui = "0000000000000000";
// const char *appKey = "00000000000000000000000000000000";

bool DEBUG = true;
const unsigned long interval = 30000; // 300-second interval to send message
unsigned long previousMillis = 0; // will store last time message sent
unsigned int counter = 0; // message counter

char myStr[50];
char outStr[255];
byte recvStatus = 0;

const sRFM_pins RFM_pins = {
  .CS = SS,
  .RST = RFM_RST,
  .DIO0 = RFM_DIO0,
  .DIO1 = RFM_DIO1,
  .DIO2 = RFM_DIO2,
  .DIO5 = RFM_DIO5,
};

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(115200);
  delay(2000);
  if (DEBUG) Serial.println("\n\nOTAA Test");
  initTFT();
  if (!lora.init()) {
    if (DEBUG) Serial.println("RFM95 not detected");
    logMsg("RFM95 not detected");
    while (1);
    return;
  }
  // Set LoRaWAN Class change CLASS_A or CLASS_C
  logMsg("* setDeviceClass");
  lora.setDeviceClass(CLASS_A);
  // Set Data Rate
  logMsg("* setTxPower");
  // Set TX Power
  lora.setTxPower(20, PA_BOOST_PIN);
  // Set Data Rate
  logMsg("* setDataRate");
  lora.setDataRate(SF10BW125);
  // set channel to random
  logMsg("* setChannel");
  lora.setChannel(MULTI);
  // Put OTAA Key and DevAddress here
  logMsg("* setDevEUI");
  lora.setDevEUI(devEui);
  logMsg("* setAppEUI");
  lora.setAppEUI(appEui);
  logMsg("* setAppKey");
  lora.setAppKey(appKey);
  // Join procedure
  bool isJoined;
  logMsg("* Joining...");
  if (DEBUG) Serial.print("Joining...");
  do {
    isJoined = lora.join();
    //wait for 10s to try again
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
    Serial.write('.');
  } while (!isJoined);
  if (DEBUG) {
    Serial.println("\n\nJoined network!");
    logMsg("* Joined network!");
  } else {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
  }
  digitalWrite(13, LOW);
}

void loop() {
  // Check interval overflow
  if (millis() - previousMillis > interval) {
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    previousMillis = millis();
    sprintf(myStr, "Counter-%d", counter);
    if (DEBUG) Serial.print("Sending: ");
    if (DEBUG) Serial.println(myStr);
    char buf[128];
    sprintf(buf, "Sending: %s", myStr);
    logMsg(buf, true);
    lora.sendUplink(myStr, strlen(myStr), 0, 1);
    logMsg("* Done!");
    counter++;
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
  }
  recvStatus = lora.readData(outStr);
  if (recvStatus) {
    if (DEBUG) Serial.println(outStr);
  }
  // Check Lora RX
  lora.update();
  recvStatus = lora.readData(outStr);
  if (recvStatus) {
    if (DEBUG) Serial.print("Rcv: ");
    if (DEBUG) Serial.println(outStr);
    logMsg("* Receiving:");
    logMsg(outStr);
  }
}
