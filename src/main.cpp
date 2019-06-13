#include "main.hpp"
#include "threads/SensorsThread.hpp"

typedef SensorsThread<THREAD_STACKSIZE_TINY + THREAD_EXTRA_STACKSIZE_PRINTF> Sensors;
typedef ServoMotor<50, 544, 2400, 180> Sg90;
typedef Lcd44780<16, 2> Lcd1602;

Lcd1602 lcd(
        GPIO_PIN(PORT_B, 12),
        GPIO_PIN(PORT_B, 13),
        GPIO_PIN(PORT_A, 9),
        GPIO_PIN(PORT_A, 10),
        GPIO_PIN(PORT_A, 11),
        GPIO_PIN(PORT_A, 12)
);

Screen screen(&lcd);
RotaryEncoder encoder(GPIO_PIN(PORT_A, 6), GPIO_PIN(PORT_A, 5), GPIO_PIN(PORT_A, 7));
Sg90 servo(0, 0);
StopWatch stopWatch(0, 2, 0);

Sensors sensorsThread("Sensors", THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_WOUT_YIELD);

int main()
{
    encoder.onClick = []() {
        if (stopWatch.isRunning()) {
            stopWatch.stop();
        } else {
            stopWatch.start();
        }
    };

    encoder.onKeyDown = []() {
        gpio_toggle(LED0_PIN);
    };

    encoder.onKeyPressed = []() {
    };

    encoder.onClockWise = []() {
        gpio_clear(LED0_PIN);
    };

    encoder.onCounterClockWise = []() {
        gpio_set(LED0_PIN);
    };

    stopWatch.onTimeout = []() {
        servo.setDegree(0);
        xtimer_sleep(DEFAULT_SLEEP_TIME);
        servo.detach();
    };

    board_init();
    encoder.init();
    servo.init();
    screen.init();

    sensorsThread.create();

    servo.setDegree(180);
    xtimer_sleep(DEFAULT_SLEEP_TIME);
    servo.detach();

    msg_t msg;
    screen.showStatic();

    loop {
        if (msg_try_receive(&msg)) {
            auto* message = static_cast<Sensors::SensorMessage*>(msg.content.ptr);
            auto temperature = message->temperature;

            screen.showTemperature(temperature, message->hasActualValue);
        }

        screen.showTime(stopWatch);

        gpio_toggle(LED0_PIN);
        xtimer_usleep(100000);
    }

    return 0;
}