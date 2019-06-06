#pragma once

#include <board.h>
#include <thread.h>
#include <periph_conf.h>
#include <periph/gpio.h>
#include <xtimer.h>

#include "ds18.h"
#include <cmath>

#include "drivers/ServoMotor.hpp"
#include "drivers/Lcd44780.hpp"

#define loop while(1)

#define ENABLE_DEBUG (1)
#include "debug.h"

#if ENABLE_DEBUG
const uint16_t MIN_STACK_SIZE = THREAD_STACKSIZE_TINY + THREAD_EXTRA_STACKSIZE_PRINTF;
#else
const uint16_t MIN_STACK_SIZE = THREAD_STACKSIZE_TINY;
#endif

const uint8_t DEFAULT_SLEEP_TIME = 1;
