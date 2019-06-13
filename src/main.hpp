#pragma once

#include <board.h>
#include <thread.h>
#include <periph_conf.h>
#include <periph/gpio.h>
#include <xtimer.h>
#include <msg.h>

#include "ds18.h"
#include <cmath>

#include "drivers/ServoMotor.hpp"
#include "drivers/Lcd44780.hpp"
#include "drivers/RotaryEncoder.hpp"
#include "StopWatch.hpp"

typedef Lcd44780<16, 2> Lcd1602;
#include "Screen.hpp"

#define loop while(1)

const uint8_t DEFAULT_SLEEP_TIME = 1;
