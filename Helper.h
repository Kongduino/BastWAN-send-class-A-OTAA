#include <Adafruit_ILI9341.h>
#include <BBQ10Keyboard.h>
#include "/Users/dda/Kongduino/Arduino/libraries/SdFatAdafruit/src/SdFat.h"
#include "Fonts.h"
#include "RAKBlueLogo.h"

void initTFT();
void displayBattery(bool);
void logMsg(char *, bool);
void showScreen();

#define TFT_CS 9
#define TFT_DC 10
#define SD_CS 5
#define batteryLeft 1
#define batteryTop 1
#define batteryWidth 35
#define batteryHeight 16

Adafruit_ILI9341 tft(&SPI1, TFT_DC, TFT_CS, -1);
//Adafruit_STMPE610 ts(STMPE_CS, &SPI1);
BBQ10Keyboard keyboard;
// File system object.
SdFat sd(&SPI1);
// Log file.
SdFile file;
double lastBatteryCheck = 0;
uint16_t lastBattery = 0, py = 60;

void logMsg(char *msg, bool bold = false) {
  if (py > 220) {
    py = 60;
    showScreen();
  }
  tft.setCursor(10, py);
  tft.print(msg);
  if (bold) {
    tft.setCursor(11, py);
    tft.print(msg);
  }
  py += 15;
}

void showScreen() {
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setRotation(1);
  tft.setFont(&FreeMonoBold12pt7b);
  tft.setCursor(5, 40);
  tft.print("LoRaWAN Mobile");
  displayBattery(true);
  tft.drawRGBBitmap(320 - RAKBlueLogo_width, 0, RAKBlueLogo, RAKBlueLogo_width, RAKBlueLogo_height);
  tft.setFont(&FreeMono9pt7b);
}

void initTFT() {
  // ---- TFT STUFF ----
  tft.begin();
  SerialUSB.println(" . TFT begin");
  showScreen();
}

void displayBattery(bool force) {
  double t0 = millis();
  if (t0 - lastBatteryCheck < 30000 && !force) return;
  uint16_t battery = analogRead(A0) / 8.6;
  if (battery != lastBattery) {
    SerialUSB.println("Last Battery: " + String(lastBattery) + " vs current: " + String(battery));
    tft.drawRoundRect(batteryLeft, batteryTop, batteryWidth, batteryHeight, 4, ILI9341_BLACK);
    tft.fillRect(batteryLeft + batteryWidth, batteryTop + 4, 3, 8, ILI9341_BLACK);
    tft.fillRect(batteryLeft + 2, batteryTop + 3, batteryWidth - 6, batteryHeight - 6, ILI9341_WHITE);
    uint16_t cl = ILI9341_GREEN;
    if (battery < 25) cl = ILI9341_RED;
    if (battery < 50) cl = ILI9341_ORANGE;
    if (battery < 75) cl = ILI9341_GREENYELLOW;
    tft.fillRect(batteryLeft + 3, batteryTop + 3, (batteryWidth - 6) * battery / 100, batteryHeight - 6, cl);
    lastBattery = battery;
    lastBatteryCheck = t0;
    // uint16_t zone[batteryWidth * batteryHeight];
    // tft.readGRAM(batteryLeft+batteryWidth, batteryTop, zone, batteryWidth, batteryHeight);
    // hexDump((uint8_t*)zone, batteryWidth * batteryHeight * 2);
    // This doesn't seem to work
  }
}
