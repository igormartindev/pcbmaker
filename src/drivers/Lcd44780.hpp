#pragma once

#include <cstdarg>
#include "hd44780.h"

/**
 * Lcd driver template
 * @tparam COLUMNS_COUNT
 * @tparam ROWS_COUNT
 */
template <uint8_t COLUMNS_COUNT, uint8_t ROWS_COUNT>
class Lcd44780
{
public:
    typedef uint8_t CharMap[8];

    explicit Lcd44780(gpio_t rs, gpio_t en, gpio_t d4, gpio_t d5, gpio_t d6, gpio_t d7) noexcept;
    void init();
    void clear();
    void home();
    void setCursorPosition(uint8_t column, uint8_t row);
    void print(const char* string);
    void printf(const char* pattern, ...);
    void createCustomChar(uint8_t location, const CharMap *charMap);
    void printCustomChar(uint8_t location);

private:
    const uint8_t HD44780_PIN_NONE = HD44780_RW_OFF;
    const uint8_t CUSTOM_CHARACTERS_MAX = 8;

    /**
     * Addition symbols for end of string '\0'
     */
    char buffer[ROWS_COUNT * COLUMNS_COUNT + ROWS_COUNT];

    hd44780_t device = {};
};

/**
 * Constructor
 * @param rs Display RS pin
 * @param en Display E pin
 * @param d4 Data pin
 * @param d5 Data pin
 * @param d6 Data pin
 * @param d7 Data pin
 */
template <uint8_t COLUMNS_COUNT, uint8_t ROWS_COUNT>
Lcd44780<COLUMNS_COUNT, ROWS_COUNT>::Lcd44780(gpio_t rs, gpio_t en, gpio_t d4, gpio_t d5, gpio_t d6, gpio_t d7)
{
    this->device.p = {
        .cols   = COLUMNS_COUNT,
        .rows   = ROWS_COUNT,
        .rs     = rs,
        .rw     = HD44780_RW_OFF,
        .enable = en,
        .data   = {d4, d5, d6, d7, HD44780_PIN_NONE, HD44780_PIN_NONE, HD44780_PIN_NONE, HD44780_PIN_NONE}
    };
}

/**
 * Init the display
 */
template <uint8_t COLUMNS_COUNT, uint8_t ROWS_COUNT>
void Lcd44780<COLUMNS_COUNT, ROWS_COUNT>::init()
{
    hd44780_init(&this->device, &this->device.p);
    this->clear();
    this->home();
}

/**
 * Clear the display
 */
template <uint8_t COLUMNS_COUNT, uint8_t ROWS_COUNT>
void Lcd44780<COLUMNS_COUNT, ROWS_COUNT>::clear()
{
    hd44780_clear(&this->device);
}

/**
 * Set cursor to home position (0, 0)
 */
template <uint8_t COLUMNS_COUNT, uint8_t ROWS_COUNT>
void Lcd44780<COLUMNS_COUNT, ROWS_COUNT>::home()
{
    hd44780_home(&this->device);
}

/**
 * Set cursor position
 */
template <uint8_t COLUMNS_COUNT, uint8_t ROWS_COUNT>
void Lcd44780<COLUMNS_COUNT, ROWS_COUNT>::setCursorPosition(uint8_t column, uint8_t row)
{
    hd44780_set_cursor(&this->device, column, row);
}

/**
 * Display const string
 * @param const char* string
 */
template <uint8_t COLUMNS_COUNT, uint8_t ROWS_COUNT>
void Lcd44780<COLUMNS_COUNT, ROWS_COUNT>::print(const char* string)
{
    hd44780_print(&this->device, string);
}

/**
 * Print formatted string
 * @param pattern
 * @param ...
 */
template <uint8_t COLUMNS_COUNT, uint8_t ROWS_COUNT>
void Lcd44780<COLUMNS_COUNT, ROWS_COUNT>::printf(const char* pattern, ...)
{
    va_list args;

    va_start(args, pattern);
    vsprintf(this->buffer, pattern, args);
    va_end(args);

    this->print(this->buffer);
}

/**
 * Create custom character
 * @param location Character GRAM position
 * @param charMap Bitmap of the character (5 x 8 pixels)
 */
template <uint8_t COLUMNS_COUNT, uint8_t ROWS_COUNT>
void Lcd44780<COLUMNS_COUNT, ROWS_COUNT>::createCustomChar(uint8_t location, const CharMap *charMap)
{
    assert(location <= CUSTOM_CHARACTERS_MAX);
    hd44780_create_char(&this->device, location, (uint8_t*)charMap);
}

/**
 * Print custom character
 * @param location Character GRAM position
 */
template <uint8_t COLUMNS_COUNT, uint8_t ROWS_COUNT>
void Lcd44780<COLUMNS_COUNT, ROWS_COUNT>::printCustomChar(uint8_t location)
{
    assert(location <= CUSTOM_CHARACTERS_MAX);
    hd44780_write(&this->device, location);
}
