#pragma once

#include "ServoMotor.hpp"

class Sg90 : public ServoMotor<50, 544, 2400, 180>
{
public:
    using ServoMotor::ServoMotor;
};
