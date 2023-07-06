#include "Adafruit_ThinkInk.h"
#include "Texts.h"
#include <LowPower.h>

// Pin definitions
#define EPD_CS 10
#define EPD_DC 9
#define SRAM_CS 8
#define EPD_RESET 5
#define EPD_BUSY 3

// Color settings
#define BLACK EPD_BLACK
#define RED EPD_RED


//----Display selector----//

//Flexible display
//Adafruit_IL0373 display(212, 104, EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);
//#define FLEXIBLE_213 // uncomment for flexible display size setting (required)

// 2.13" Monochrome displays with 250x122 pixels and SSD1680 chipset
ThinkInk_213_Mono_BN display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

const int TimeToWaitInSeconds = (86400 / 8);  // amount of seconds to wait devided by the 8 seconds of powerdown. 86400 seconds = a day

const int ENAPin = 4;  // Ultra low power EPD pin

char buffer[MAX_TEXT_LENGTH + 1];  // Buffer to store the retrieved text

int counter = 0;

void setup() {

  display.begin();
#if defined(FLEXIBLE_213) || defined(FLEXIBLE_290)  // for flexible displays
  display.setBlackBuffer(1, false);
  display.setColorBuffer(1, false);
#endif
  drawimageEPD(getRandomText(), BLACK);
  pinMode(EPD_RESET, OUTPUT);
  pinMode(ENAPin, OUTPUT);
}

void loop() {
  if (counter >= TimeToWaitInSeconds) {
    delay(10);
    digitalWrite(ENAPin, HIGH);  // Enable screen
    delay(50);
    drawimageEPD(getRandomText(), BLACK);
    delay(10);
    digitalWrite(ENAPin, LOW);  // Disable screen
    counter = 0;
  }
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  // go into sleep mode for extreme power saving
  counter = counter + 1;
}

void drawimageEPD(const char* text, uint16_t color) {
  display.begin();
  display.clearBuffer();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(color);
  display.setTextWrap(true);
  display.print(text);
  display.display();
}

const char* getRandomText() {
  int randomIndex = random(0, TEXT_COUNT);
  strncpy_P(buffer, (char*)pgm_read_word(&(Texts[randomIndex])), MAX_TEXT_LENGTH);
  buffer[MAX_TEXT_LENGTH] = '\0';
  return buffer;
}