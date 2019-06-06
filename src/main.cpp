#include "main.hpp"
#include "threads/SensorsThread.hpp"
#include "hd44780.h"

typedef SensorsThread<MIN_STACK_SIZE> Sensors;
typedef ServoMotor<50, 544, 2400, 180> Sg90;
typedef Lcd44780<16, 02> Lcd1602;

Sg90 servo(0, 0);
Lcd1602 lcd(
    GPIO_PIN(PORT_B, 12),
    GPIO_PIN(PORT_B, 13),
    GPIO_PIN(PORT_A, 9),
    GPIO_PIN(PORT_A, 10),
    GPIO_PIN(PORT_A, 11),
    GPIO_PIN(PORT_A, 12)
);

Sensors sensorsThread("Sensors", THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_WOUT_YIELD);

int main()
{
    board_init();
    servo.init();
    lcd.init();

    sensorsThread.create();

    lcd.print("Temperature:");

    loop {
        gpio_toggle(LED0_PIN);

        lcd.setCursorPosition(0, 1);
        if (sensorsThread.hasTemperature()) {
            lcd.printf(
                "%d.%02d *C",
                (uint16_t)sensorsThread.getTemperature(),
                abs((uint16_t)(sensorsThread.getTemperature() * 100) % 100)
            );
        } else {
            lcd.print("--.-- *C");
        }

        xtimer_sleep(DEFAULT_SLEEP_TIME);
    }

    return 0;
}