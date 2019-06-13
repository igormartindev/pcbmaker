#pragma once

#include <cstdint>

/**
 * StopWatch
 */
class StopWatch
{
public:
    explicit StopWatch(uint8_t hours, uint8_t minutes, uint8_t seconds) noexcept;

    void (*onTimeout)() = nullptr;
    void (*onTick)() = nullptr;

    void start();
    void stop();

    bool isRunning();
    uint8_t getSeconds();
    uint8_t getMinutes();
    uint8_t getHours();

private:
    static const uint32_t TIMER_PERIOD = 1 * US_PER_SEC;

    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;

    xtimer_t secondsTimer = {};
    bool isEnabled = false;

    void decrementSeconds();
    static void tick(void *args);
};

/**
 * Constructor
 * @param hours
 * @param minutes
 * @param seconds
 */
StopWatch::StopWatch(uint8_t hours, uint8_t minutes, uint8_t seconds) noexcept
{
    this->hours = hours;
    this->minutes = minutes;
    this->seconds = seconds;

    this->secondsTimer.callback = StopWatch::tick;
    this->secondsTimer.arg = this;
}

/**
 * Start the stopwatch
 */
void StopWatch::start()
{
    xtimer_set(&this->secondsTimer, TIMER_PERIOD);
    this->isEnabled = true;
}

/**
 * Stop (pause) the stopwatch
 */
void StopWatch::stop()
{
    xtimer_remove(&this->secondsTimer);
    this->isEnabled = false;
}

/**
 * Check for stopwatch is running
 * @return bool
 */
bool StopWatch::isRunning()
{
    return this->isEnabled;
}

/**
 * Get seconds
 * @return uint8_t
 */
uint8_t StopWatch::getSeconds()
{
    return this->seconds;
}

/**
 * Get minutes
 * @return uint8_t
 */
uint8_t StopWatch::getMinutes()
{
    return this->minutes;
}

/**
 * Get hours
 * @return uint8_t
 */
uint8_t StopWatch::getHours()
{
    return this->hours;
}

/**
 * Decrement seconds
 */
void StopWatch::decrementSeconds()
{
    if (seconds > 0) {
        seconds--;

        if (seconds == 0 && minutes == 0 && hours == 0) {
            this->stop();

            if (onTimeout) {
                this->onTimeout();
            }
        }
        return;
    }

    if (minutes > 0) {
        minutes--;
        seconds = 59;
        return;
    }

    if (hours > 0) {
        hours--;
        minutes = 59;
        seconds = 59;
        return;
    }
}

/**
 * Ticks handler
 * @param args StopWatch*
 */
void StopWatch::tick(void *args)
{
    auto stopWatch = static_cast<StopWatch*>(args);
    xtimer_set(&stopWatch->secondsTimer, TIMER_PERIOD);
    stopWatch->decrementSeconds();

    if (stopWatch->onTick) {
        stopWatch->onTick();
    }
}
