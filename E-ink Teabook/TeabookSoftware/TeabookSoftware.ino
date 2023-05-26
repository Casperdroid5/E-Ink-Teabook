#include "Adafruit_ThinkInk.h"
#include "Texts.h"
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>

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

// Time interval before new text is displayed
const unsigned long DISPLAY_INTERVAL = 30000;  // One day: 86400

const int ENAPin = 4;  // Ultra low power EPD pin

char buffer[MAX_TEXT_LENGTH + 1];  // Buffer to store the retrieved text

unsigned long previousDisplayTime = 0;
unsigned long interval = DISPLAY_INTERVAL;  // Desired interval in seconds

void setup() {
  display.begin();
#if defined(FLEXIBLE_213) || defined(FLEXIBLE_290) // for flexible displays
  display.setBlackBuffer(1, false);
  display.setColorBuffer(1, false);
#endif
  drawimageEPD(getRandomText(), BLACK);
  pinMode(EPD_RESET, OUTPUT);
  pinMode(ENAPin, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousDisplayTime >= interval) {
    previousDisplayTime = currentMillis;
    drawimageEPD(getRandomText(), BLACK);
  }

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Set sleep mode to power down
  sleep_enable();                       // Enable sleep mode
  noInterrupts();                       // Disable interrupts
  wdt_reset();                          // Reset watchdog timer
  power_all_disable();                  // Disable all power-consuming modules

  // Enter sleep mode
  sleep_mode();

  // Code resumes execution here after waking up from sleep
  sleep_disable();                      // Disable sleep mode
  interrupts();                         // Enable interrupts
}

void drawimageEPD(const char* text, uint16_t color) {
  digitalWrite(ENAPin, HIGH);  // Enable screen
  display.clearBuffer();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(color);
  display.setTextWrap(true);
  display.print(text);
  display.display();
  digitalWrite(ENAPin, LOW);  // Disable screen
}

const char* getRandomText() {
  int randomIndex = random(0, TEXT_COUNT);
  strncpy_P(buffer, (char*)pgm_read_word(&(Texts[randomIndex])), MAX_TEXT_LENGTH);
  buffer[MAX_TEXT_LENGTH] = '\0';
  return buffer;
}
