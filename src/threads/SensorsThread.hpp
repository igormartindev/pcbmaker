#pragma once

#include "../main.hpp"
#include "Thread.hpp"


template <uint16_t stackSize>
class SensorsThread : public Thread<stackSize>
{
public:
    using Thread<stackSize>::Thread;

    /**
     * Get temperature
     * @return float
     */
    float getTemperature()
    {
        return temperature;
    }

protected:
    const uint16_t SLEEP_TIME = 1;

    volatile float temperature = 0;

    void* run(void*) override
    {
        uint16_t rawTemperature = 0;

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
                xtimer_sleep(SLEEP_TIME);
            }
        }

        loop {
            if (ds18_get_temperature(&sensor, (int16_t*) &rawTemperature) != DS18_OK) {
                DEBUG("Error: Could not read temperature\n");
                xtimer_sleep(SLEEP_TIME);
                continue;
            }

            this->temperature = (float)rawTemperature / 100;
            xtimer_sleep(SLEEP_TIME);
        }
    }
};
