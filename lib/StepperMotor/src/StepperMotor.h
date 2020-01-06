#pragma once

#include <HighPowerStepperDriver.h>

class StepperMotor : public HighPowerStepperDriver {
public:
    StepperMotor(uint8_t cs_pin);  // Constructor - needs chip select
    ~StepperMotor();               // Destructor

private:
    bool update_steps();                  // step the motor if not at target yet
    bool set_angle(float angle_degrees);  // set the angle
};
