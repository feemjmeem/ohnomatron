## an esp8266 countdown clock for a video game

So I've been playing [Outer Wilds](https://www.mobiusdigitalgames.com/outer-wilds.html) and there's a 22-minute countdown in the game.

This is a clock that does that.

BOM:

* [NodeMCU ESP8266](https://www.amazon.com/ESP8266-NodeMcu-Internet-Development-Micropython/dp/B07L8W9SP3)
* [SSD1306 4-pin I2C OLED display](https://www.amazon.com/gp/product/B07X245RPC/)
* [PS1240 Piezo Buzzer](https://www.adafruit.com/product/160)
* [OMRON B3F Tactile Button](https://www.adafruit.com/product/367) x 2
* THT DIN0207 10k Resistor x2

Any substitution components should work but you'll need to mess with the pinouts.

I designed a PCB for this and eventually I'll clean it up for release.

Important note on design: the knockoff SSD1306 I used has four pins in THIS ORDER: ```GND VCC SCL SDA```
Some versions of this board use a different order and pins will need to be changed in the build for those changes. If GND and VCC are swapped, the board I've designed will not work. Be aware of the pin order.

Features:
* Press 'RESTART' button to restart the countdown, or to start it after first powering on/rebooting
* Hold 'RESTART' button to reboot device
* Press 'PAUSE' button to pause countdown

Loose roadmap:

* Add wifi sync to game agent for automatic resets
* Add ability to specify arbitrary time using keypresses
* ???
