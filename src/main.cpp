#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <CountDown.h>
#include <Wire.h>
#include "config.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
CountDown CD(CountDown::SECONDS);
uint32_t start, stop;

const int resetButton = D4;
const int pauseButton = D5;
const int buzzer = D6;

int final_countdown = 0;
bool paused = false;

void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextWrap(false);
    display.setTextSize(4);
    display.setTextColor(WHITE);
    display.setCursor(5,15);
    display.print("[]:[]");
    display.display();
    pinMode(resetButton, INPUT);
    pinMode(pauseButton, INPUT);
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
    if (digitalRead(resetButton)) {
        paused = false;
        tone(buzzer, 1500, 100);
        delay(50);
        tone(buzzer, 1900, 100);
        CD.start(CD_DAYS, CD_HOURS, CD_MINUTES, CD_SECONDS);
        final_countdown = 0;
    }
    if (digitalRead(pauseButton)) {
        if (paused) {
            CD.cont();
            paused = false;
        } else {
            CD.stop();
            paused = true;
        }
        tone(buzzer, 1000, 100);
    }
    delay(100);
}