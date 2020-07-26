#include <AceButton.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <CountDown.h>
#include <Wire.h>
#include "config.h"
using namespace ace_button;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// init I2C SSD1306 4-wire display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// init countdown timer
CountDown CD(CountDown::SECONDS);

// button configs
ButtonConfig resetConfig, pauseConfig;

// buttons
AceButton resetButton(&resetConfig), pauseButton(&pauseConfig);

// pin setup
const int RESET_PIN = D6;
const int PAUSE_PIN = D7;
const int buzzer = D5;

// init globals
int final_countdown = 0;
bool paused = false;

// prototypes
void handleResetEvent(AceButton*, uint8_t, uint8_t);
void handlePauseEvent(AceButton*, uint8_t, uint8_t);

void setup() {
    // let's get the display going
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextWrap(false);
    display.setTextSize(4);
    display.setTextColor(WHITE);
    display.setCursor(5,15);
    display.print("[]:[]");
    display.display();

    // plz to be configs
    resetConfig.setEventHandler(handleResetEvent);
    resetConfig.setFeature(ButtonConfig::kFeatureLongPress);
    pauseConfig.setEventHandler(handlePauseEvent);

    // let there be buttons
    pinMode(RESET_PIN, INPUT);
    resetButton.init(RESET_PIN, LOW);
    pinMode(PAUSE_PIN, INPUT);
    pauseButton.init(PAUSE_PIN, LOW);

    // im a fukken bee
    pinMode(buzzer, OUTPUT);
}

void loop() {
    int rem = CD.remaining();
    int m = rem / 60;
    int s = rem % 60;

    if (CD.isRunning() || paused) {
        display.clearDisplay();
        if (paused) {
            display.setTextSize(1);
            display.setCursor(48,0);
            display.printf("PAUSED");
        } else {
            if (rem <= FINAL_COUNT && final_countdown != rem) {
                tone(buzzer, 1400, 100);
                final_countdown = rem;
            }
        }
        display.setTextSize(4);
        display.setCursor(7,15);
        display.printf("%02d:%02d", m, s);
        display.display();
    } else {
        display.clearDisplay();
        display.setCursor(7,15);
        display.printf("XX:XX");
        display.display();
        final_countdown = 0;
    }
    resetButton.check();
    pauseButton.check();
}

void handleResetEvent(AceButton* /* button */, uint8_t eventType, uint8_t /* buttonState */) {
    switch (eventType) {
        case AceButton::kEventReleased:
            paused = false;
            tone(buzzer, 1500, 100);
            delay(50);
            tone(buzzer, 1900, 100);
            CD.start(CD_DAYS, CD_HOURS, CD_MINUTES, CD_SECONDS);
            final_countdown = 0;
            break;
        case AceButton::kEventLongPressed:
            tone(buzzer, 500, 100);
            delay(150);
            tone(buzzer, 500, 100);
            delay(150);
            tone(buzzer, 500, 300);
            delay(300);
            ESP.restart();
            break; // this isn't needed but yolo
    }
}

void handlePauseEvent(AceButton* /* button */, uint8_t eventType, uint8_t /* buttonState */) {
    switch (eventType) {
        case AceButton::kEventPressed:
            if (paused) {
                CD.cont();
                paused = false;
            } else {
                CD.stop();
                paused = true;
            }
            tone(buzzer, 1000, 100);
            break;
    }
}
