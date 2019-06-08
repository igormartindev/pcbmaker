/**
 * Constructor
 *
 * @param pwm     Number of pwm
 * @param channel Pwm channel
 */
template <uint16_t FREQUENCY, uint16_t MIN_PULSE, uint16_t MAX_PULSE, uint16_t MAX_DEGREE>
ServoMotor<FREQUENCY, MIN_PULSE, MAX_PULSE, MAX_DEGREE>::ServoMotor(uint8_t pwm, uint8_t channel)
{
    this->pwm = PWM_DEV(pwm);
    this->pwmChannel = channel;
}

/**
 * Setup pwm frequency and servo positions
 */
template <uint16_t FREQUENCY, uint16_t MIN_PULSE, uint16_t MAX_PULSE, uint16_t MAX_DEGREE>
void ServoMotor<FREQUENCY, MIN_PULSE, MAX_PULSE, MAX_DEGREE>::init()
{
    servo_init(&this->device, this->pwm, this->pwmChannel, MIN_PULSE, MAX_PULSE);

    // Update frequency
    this->device.scale_nom = pwm_init(pwm, PWM_LEFT, FREQUENCY, RESOLUTION);
    this->device.scale_den = FREQUENCY;
}

/**
 * Rotate the servo to a specified degree
 *
 * @param degree The degree, that a servo must be rotated
 */
template <uint16_t FREQUENCY, uint16_t MIN_PULSE, uint16_t MAX_PULSE, uint16_t MAX_DEGREE>
void ServoMotor<FREQUENCY, MIN_PULSE, MAX_PULSE, MAX_DEGREE>::setDegree(uint16_t degree)
{
    servo_set(&this->device, (uint16_t)(MIN_PULSE + (ONE_DEGREE * degree)));
}

/**
 * Disable the servo retention
 *
 * Use set degree to enable retention
 */
template <uint16_t FREQUENCY, uint16_t MIN_PULSE, uint16_t MAX_PULSE, uint16_t MAX_DEGREE>
void ServoMotor<FREQUENCY, MIN_PULSE, MAX_PULSE, MAX_DEGREE>::detach()
{
    pwm_set(this->device.device, (uint8_t)this->device.channel, 0);
}
