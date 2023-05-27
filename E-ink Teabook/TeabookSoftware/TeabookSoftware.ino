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
const unsigned long DISPLAY_INTERVAL = 30000;  // 30 seconds

const int ENAPin = 4;  // Ultra low power EPD pin

char buffer[MAX_TEXT_LENGTH + 1];  // Buffer to store the retrieved text

unsigned long previousDisplayTime = 0;
unsigned long interval = DISPLAY_INTERVAL;  // Desired interval in milliseconds
volatile int i = 0;

void setup() {
  display.begin();
  Serial.begin(9600);
#if defined(FLEXIBLE_213) || defined(FLEXIBLE_290)  // for flexible displays
  display.setBlackBuffer(1, false);
  display.setColorBuffer(1, false);
#endif
  drawimageEPD(getRandomText(), BLACK);
  pinMode(EPD_RESET, OUTPUT);
  pinMode(ENAPin, OUTPUT);
}

void loop() {

  MCUSR = 0;                                         // Allow changes, disable reset
  WDTCSR |= (1 << WDCE) | (1 << WDE);                // Set interrupt mode and an interval
  WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0);  // Enable watchdog interrupt, 8 seconds delay
  wdt_reset();                                       // Pat the dog
    // Enter low-power sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  noInterrupts();

  // Turn off brown-out detection
  MCUCR = bit(BODS) | bit(BODSE);
  MCUCR = bit(BODS);

  interrupts();
  sleep_cpu();

  // Execution resumes here after waking up from sleep
  sleep_disable();
  Serial.println("wakeup");
  if (i > 20) { // 21*8 = 168 SEC
    Serial.println("E-ink time");
    drawimageEPD(getRandomText(), BLACK);
    i = 0;

    //digitalWrite(ENAPin, LOW);  // Disable screen
  }
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
}

const char* getRandomText() {
  int randomIndex = random(0, TEXT_COUNT);
  strncpy_P(buffer, (char*)pgm_read_word(&(Texts[randomIndex])), MAX_TEXT_LENGTH);
  buffer[MAX_TEXT_LENGTH] = '\0';
  return buffer;
}

// Watchdog Timer Interrupt Service Routine
ISR(WDT_vect) {
  wdt_disable();  // Disable the watchdog timer
  ++i;
}
