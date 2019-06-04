#include "main.hpp"

#include "hd44780.h"

const uint8_t DEFAULT_SLEEP_TIME = 1;
char tempMonitorStack[MIN_STACK_SIZE];
int16_t temperature = 0;

void* tempMonitorThread(void *)
{
    ds18_t sensor = {
        .params = {
            .pin      = GPIO_PIN(PORT_B, 1),
            .out_mode = GPIO_OD_PU,
            .in_mode  = GPIO_IN_PU,
        }
    };

    if (ds18_init(&sensor, &sensor.params) != DS18_OK) {
        DEBUG("Error: The sensor pin could not be initialized\n");

        loop {
            xtimer_sleep(DEFAULT_SLEEP_TIME);
        }
    }

    loop {
        if (ds18_get_temperature(&sensor, &temperature) != DS18_OK) {
            DEBUG("Error: Could not read temperature\n");
            xtimer_sleep(DEFAULT_SLEEP_TIME);
            continue;
        }

        if (ENABLE_DEBUG) {
            DEBUG_PRINT("Temperature: %d.%02d *C\n", temperature / 100, abs(temperature) % 100);
        }

        xtimer_sleep(DEFAULT_SLEEP_TIME);
    }
}

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
    sprintf(lcdLine, "%d.%02d *C", temperature / 100, abs(temperature) % 100);

    hd44780_print(&lcd, lcdLine);
}

int main()
{
    board_init();

    thread_create(
            tempMonitorStack,
            sizeof(tempMonitorStack),
            0,
            THREAD_CREATE_WOUT_YIELD,
            tempMonitorThread,
            NULL,
            "Temperature monitor"
        );

    lcdTest();
    servoTest();

    loop {
        gpio_toggle(LED0_PIN);
        xtimer_sleep(DEFAULT_SLEEP_TIME);
    }

    return 0;
}