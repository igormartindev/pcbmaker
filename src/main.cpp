#include <board.h>
#include <thread.h>
#include <periph_conf.h>
#include <periph/gpio.h>
#include <xtimer.h>

#include "ds18.h"
#include <cmath>

#define loop while(1)

const uint8_t DEFAULT_SLEEP_TIME = 1;

char tempMonitorStack[THREAD_STACKSIZE_MAIN];
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
        puts("Error: The sensor pin could not be initialized\n");

        loop {
            xtimer_sleep(DEFAULT_SLEEP_TIME);
        }
    }

    loop {
        if (ds18_get_temperature(&sensor, &temperature) != DS18_OK) {
            puts("Error: Could not read temperature\n");
            xtimer_sleep(DEFAULT_SLEEP_TIME);
            continue;
        }

        printf("Temperature: %d.%02d *C\n", temperature / 100, abs(temperature) % 100);
        xtimer_sleep(DEFAULT_SLEEP_TIME);
    }
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

    loop {
        gpio_toggle(LED0_PIN);
        xtimer_sleep(DEFAULT_SLEEP_TIME);
    }

    return 0;
}