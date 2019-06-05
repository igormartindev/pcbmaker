#include "main.hpp"
#include "threads/SensorsThread.hpp"
#include "hd44780.h"

const uint8_t DEFAULT_SLEEP_TIME = 1;

typedef SensorsThread<MIN_STACK_SIZE> Sensors;
Sensors sensorsThread("Sensors", THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_WOUT_YIELD);

void servoTest()
{
    Sg90 servo(0, 0);

    if (!servo.init()) {
        DEBUG("Errors while initializing servo");
        return;
    }

    DEBUG("Servo initialized.");

    servo.setDegree(0);
    xtimer_sleep(DEFAULT_SLEEP_TIME);

    servo.setDegree(180);
    xtimer_sleep(DEFAULT_SLEEP_TIME);

    servo.setDegree(90);
    xtimer_sleep(DEFAULT_SLEEP_TIME);

    servo.detach();
}

void lcdTest()
{
    const uint8_t HD44780_PIN_NONE = HD44780_RW_OFF;

    char lcdLine[16 + 1];

    hd44780_t lcd = {};
    hd44780_params_t lcdParams = {
        .cols   = 16,
        .rows   = 2,
        .rs     = GPIO_PIN(PORT_B, 12),
        .rw     = HD44780_PIN_NONE,
        .enable = GPIO_PIN(PORT_B, 13),
        .data   = {
            GPIO_PIN(PORT_A, 9),
            GPIO_PIN(PORT_A, 10),
            GPIO_PIN(PORT_A, 11),
            GPIO_PIN(PORT_A, 12),
            HD44780_PIN_NONE,
            HD44780_PIN_NONE,
            HD44780_PIN_NONE,
            HD44780_PIN_NONE
        }
    };

    if (hd44780_init(&lcd, &lcdParams) != 0) {
        puts("Error lcd init");
    }

    hd44780_clear(&lcd);
    hd44780_home(&lcd);

    hd44780_print(&lcd, "Temperature:");

    hd44780_set_cursor(&lcd, 0, 1);

    sprintf(
        lcdLine,
        "%d.%02d *C",
        (uint16_t)sensorsThread.getTemperature(),
        abs((uint16_t)(sensorsThread.getTemperature() * 100) % 100)
    );

    hd44780_print(&lcd, lcdLine);
}

int main()
{
    board_init();

    sensorsThread.create();

    lcdTest();
    servoTest();

    loop {
        gpio_toggle(LED0_PIN);

        printf(
            "Temperature: %d.%02d *C\n",
            (uint16_t)sensorsThread.getTemperature(),
            abs((uint16_t)(sensorsThread.getTemperature() * 100) % 100)
        );

        xtimer_sleep(DEFAULT_SLEEP_TIME);
    }

    return 0;
}