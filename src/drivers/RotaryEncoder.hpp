#pragma once

#include <board.h>
#include <periph/gpio.h>
#include <cmath>

/**
 * Rotary encoder driver
 */
class RotaryEncoder
{
public:
    explicit RotaryEncoder(gpio_t pinA, gpio_t pinB, gpio_t button) noexcept;
    void init();
    void onClick();
    void onClockWise();
    void onCounterClockWise();

private:
    /**
     * Ignoring time for the irq to prevent contact debounce
     */
    static const uint32_t BUTTON_IRQ_IGNORE_TIME   = 100000;
    static const uint32_t ROTATION_IRQ_IGNORE_TIME =  70000;

    enum IrqSource {
        PIN_A,
        PIN_B,
    };

    gpio_t pinA;
    gpio_t pinB;
    gpio_t button;

    uint32_t buttonLastIrqTimestamp = 0;
    uint32_t pinALastIrqTimestamp   = 0;
    uint32_t pinBLastIrqTimestamp   = 0;

    static void buttonIrqHandler(void* args);
    static void pinAIrqHandler(void *args);
    static void pinBIrqHandler(void *args);

    inline void rotationHandler(IrqSource irqSource);
};

/**
 * Constructor
 * @param pinA   Output A of the encoder
 * @param pinB   Output B of the encoder
 * @param button Button pin of the encoder
 */
RotaryEncoder::RotaryEncoder(gpio_t pinA, gpio_t pinB, gpio_t button) : pinA{pinA}, pinB{pinB}, button{button}
{
}

/**
 * Setup encoder pins
 */
void RotaryEncoder::init()
{
    /**
     * Used different handlers to prevent heap usage
     */
    gpio_init_int(button, GPIO_IN_PU, GPIO_RISING, RotaryEncoder::buttonIrqHandler, this);
    gpio_init_int(pinA, GPIO_IN_PU, GPIO_FALLING, RotaryEncoder::pinAIrqHandler, this);
    gpio_init_int(pinB, GPIO_IN_PU, GPIO_FALLING, RotaryEncoder::pinBIrqHandler, this);
}

/**
 * Button click event
 */
void RotaryEncoder::onClick()
{
}

/**
 * Clockwise encoder rotation event
 */
void RotaryEncoder::onClockWise()
{
}

/**
 * Counterclockwise encoder rotation event
 */
void RotaryEncoder::onCounterClockWise()
{
}

/**
 * Button pin irq handler
 * @param args Instance og RotaryEncoder
 */
void RotaryEncoder::buttonIrqHandler(void* args)
{
    auto instance = (RotaryEncoder*) args;

    gpio_irq_disable(instance->button);

    uint32_t now = xtimer_usec_from_ticks(xtimer_now());

    if ((now - instance->buttonLastIrqTimestamp) < BUTTON_IRQ_IGNORE_TIME) {
        instance->buttonLastIrqTimestamp = now;
        gpio_irq_enable(instance->button);
        return;
    }

    instance->onClick();
    instance->buttonLastIrqTimestamp = now;

    gpio_irq_enable(instance->button);
}

/**
 * Pin A irq handler
 * @param args Instance og RotaryEncoder
 */
void RotaryEncoder::pinAIrqHandler(void *args)
{
    auto instance = (RotaryEncoder*) args;

    gpio_irq_disable(instance->pinA);
    instance->rotationHandler(IrqSource::PIN_A);
    gpio_irq_enable(instance->pinA);
}

/**
 * Pin B irq handler
 * @param args Instance og RotaryEncoder
 */
void RotaryEncoder::pinBIrqHandler(void *args)
{
    auto instance = (RotaryEncoder*) args;

    gpio_irq_disable(instance->pinB);
    instance->rotationHandler(IrqSource::PIN_B);
    gpio_irq_enable(instance->pinB);
}

/**
 * Rotation irq handler
 * @param irqSource
 */
inline void RotaryEncoder::rotationHandler(IrqSource irqSource)
{
    uint32_t now = xtimer_usec_from_ticks(xtimer_now());
    uint32_t* timestamp = irqSource == IrqSource::PIN_A ? &this->pinALastIrqTimestamp : &this->pinBLastIrqTimestamp;

    if (now - *timestamp < ROTATION_IRQ_IGNORE_TIME) {
        *timestamp = now;
        return;
    }

    switch (irqSource) {
        case IrqSource::PIN_A:
            if (gpio_read(this->pinB)) {
                this->onClockWise();
            }
            break;

        case IrqSource::PIN_B:
            if (gpio_read(this->pinA)) {
                this->onCounterClockWise();
            }
            break;

        default:
            return;
    }

    *timestamp = now;
}
