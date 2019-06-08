#include "main.hpp"
#include "threads/SensorsThread.hpp"
#include "hd44780.h"

typedef SensorsThread<MIN_STACK_SIZE> Sensors;
typedef ServoMotor<50, 544, 2400, 180> Sg90;
typedef Lcd44780<16, 2> Lcd1602;

RotaryEncoder encoder(GPIO_PIN(PORT_A, 6), GPIO_PIN(PORT_A, 5), GPIO_PIN(PORT_A, 7));
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

uint8_t time = 0;

int main()
{
    encoder.onClick = []() {
        gpio_toggle(LED0_PIN);
    };

    encoder.onClockWise = []() {
        gpio_clear(LED0_PIN);
        if (time < 180) {
            time++;
        }
    };

    encoder.onCounterClockWise = []() {
        if (time > 0) {
            time--;
        }

        gpio_set(LED0_PIN);
    };

    board_init();
    encoder.init();
    servo.init();
    lcd.init();

    sensorsThread.create();

    servo.setDegree(180);
    xtimer_sleep(DEFAULT_SLEEP_TIME);
    servo.detach();

    lcd.print("Temperature:");

    loop {
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

        lcd.setCursorPosition(10, 1);
        lcd.printf("%d   ", time);

        xtimer_usleep(100000);
    }

    return 0;
}