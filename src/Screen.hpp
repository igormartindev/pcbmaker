#pragma once

#include "main.hpp"

/**
 * Screen
 */
class Screen
{
public:
    explicit Screen(Lcd1602* lcd) noexcept;

    void init();
    void showStatic();
    void showTemperature(float temperature, bool hasActualValue);
    void showTime(StopWatch& timer);

private:
    const uint8_t CHAR_THERMOMETER = 0;
    const uint8_t CHAR_CELSIUS     = 1;
    const uint8_t CHAR_CLOCK       = 2;
    const uint8_t CHAR_SERVO       = 3;
    const uint8_t CHAR_BEEPER      = 4;

    Lcd1602* lcd;

    const Lcd1602::CharMap thermometer = {
        0b00100,
        0b01010,
        0b01010,
        0b01010,
        0b01010,
        0b10001,
        0b10001,
        0b01110,
    };

    const Lcd1602::CharMap celsius = {
        0b10000,
        0b00110,
        0b01000,
        0b01000,
        0b00110,
        0b00000,
        0b00000,
        0b00000,
    };

    const Lcd1602::CharMap clock = {
        0b01110,
        0b00100,
        0b01110,
        0b10101,
        0b10111,
        0b10001,
        0b01110,
        0b00000,
    };

    const Lcd1602::CharMap servo = {
        0b01000,
        0b11101,
        0b10101,
        0b10111,
        0b10111,
        0b10100,
        0b11100,
        0b01000,
    };

    const Lcd1602::CharMap beeper = {
        0b00100,
        0b01110,
        0b01010,
        0b01010,
        0b01010,
        0b11111,
        0b00100,
        0b00000,
    };
};

/**
 * Constructor
 * @param lcd Lcd1602
 */
Screen::Screen(Lcd1602* lcd)
{
    this->lcd = lcd;
}

/**
 * Init the screen
 */
void Screen::init()
{
    lcd->init();

    lcd->createCustomChar(CHAR_THERMOMETER, &thermometer);
    lcd->createCustomChar(CHAR_CELSIUS, &celsius);
    lcd->createCustomChar(CHAR_CLOCK, &clock);
    lcd->createCustomChar(CHAR_SERVO, &servo);
    lcd->createCustomChar(CHAR_BEEPER, &beeper);
}

/**
 * Display static part of graphic screen
 */
void Screen::showStatic()
{
    lcd->setCursorPosition(3, 0);
    lcd->print("PCB Maker");

    lcd->setCursorPosition(0, 1);
    lcd->printCustomChar(CHAR_THERMOMETER);
    lcd->setCursorPosition(4, 1);
    lcd->printCustomChar(CHAR_CELSIUS);

    lcd->setCursorPosition(6, 1);
    lcd->printCustomChar(CHAR_CLOCK);

    lcd->setCursorPosition(9, 1);
    lcd->print(":");

    lcd->setCursorPosition(13, 1);
    lcd->printCustomChar(CHAR_SERVO);

    lcd->setCursorPosition(15, 1);
    lcd->printCustomChar(CHAR_BEEPER);
}

/**
 * Display the temperature
 * @param temperature    Temperature
 * @param hasActualValue Actual temperature flag
 */
void Screen::showTemperature(float temperature, bool hasActualValue)
{
    lcd->setCursorPosition(1, 1);
    if (hasActualValue && temperature <= 127 && temperature > -100) {
        if (temperature > -10 && temperature < 100) {
            lcd->print(" ");
        }

        lcd->printf("%2d", (int8_t)temperature);
    } else {
        lcd->setCursorPosition(1, 1);
        lcd->print(" --");
    }
}

/**
 * Display stopwatch time left
 * @param timer Stopwatch
 */
void Screen::showTime(StopWatch& stopWatch)
{
    auto minutes = (uint8_t)(stopWatch.getHours() * 60 + stopWatch.getMinutes());

    lcd->setCursorPosition(7, 1);
    lcd->printf("%02d", minutes);
    lcd->setCursorPosition(10, 1);
    lcd->printf("%02d", stopWatch.getSeconds());
}
