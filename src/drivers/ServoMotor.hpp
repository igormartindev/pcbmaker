#pragma once

#include "servo.h"

/**
 * ServoMotor driver
 *
 * @tparam FREQUENCY  Working frequency of an servo motor
 * @tparam MIN_PULSE  Min pulse duration (that corresponds to 0 degree)
 * @tparam MAX_PULSE  Max pulse duration (that corresponds to MAX_DEGREE degree)
 * @tparam MAX_DEGREE Max degree, in which a servo can rotate (typically 180)
 */
template <uint16_t FREQUENCY, uint16_t MIN_PULSE, uint16_t MAX_PULSE, uint16_t MAX_DEGREE>
class ServoMotor
{
private:
    const float    ONE_DEGREE = (float)(MAX_PULSE - MIN_PULSE) / MAX_DEGREE;
    const uint16_t RESOLUTION = (uint16_t)(US_PER_SEC / FREQUENCY);

    servo_t* device;
    pwm_t pwm;
    uint8_t pwmChannel;

public:
    explicit ServoMotor(uint8_t pwm, uint8_t channel);
    bool init();
    void setDegree(uint16_t degree);
    void detach();
};

#include "ServoMotor_impl.hpp"
