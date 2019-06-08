#pragma once

#include "../main.hpp"
#include "Thread.hpp"


template <uint16_t STACK_SIZE>
class SensorsThread : public Thread<STACK_SIZE>
{
public:
    using Thread<STACK_SIZE>::Thread;

    /**
     * Get temperature
     * @return float
     */
    inline float getTemperature()
    {
        return temperature;
    }

    /**
     * Check for actual value exists
     * @return bool
     */
    inline bool hasTemperature()
    {
        return this->hasActualTemperature;
    }

protected:
    const uint16_t SLEEP_TIME = 1;

    bool hasActualTemperature = false;
    volatile float temperature;

    void run() override
    {
        int16_t rawTemperature = 0;

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
            if (ds18_get_temperature(&sensor, &rawTemperature) != DS18_OK) {
                DEBUG("Error: Could not read temperature\n");
                this->hasActualTemperature = false;

                xtimer_sleep(SLEEP_TIME);
                continue;
            }

            this->temperature = (float)rawTemperature / 100;
            this->hasActualTemperature = true;

            xtimer_sleep(SLEEP_TIME);
        }
    }
};
