#pragma once

#include <HighPowerStepperDriver.h>
#include <utility>
#include <vector>

class StepperMotor : public HighPowerStepperDriver {
public:
    StepperMotor(
        uint8_t cs_pin,
        uint8_t lim_pin,
        HPSDDecayMode mode = HPSDDecayMode::AutoMixed,
        uint16_t current_lim_mA = 1000,
        HPSDStepMode step_mode = HPSDStepMode::MicroStep1);  // Constructor
    ~StepperMotor();                                         // Destructor

    bool inc_steps();  // step the motor if not at target yet
    bool set_angle(float angle_degrees, bool absolute);  // set the angle

private:
    static void limit_switch_isr(void);

    int16_t m_step_target;  // target step
    int16_t m_step_crnt;    // current step counter (rolls over)

    uint32_t m_last_step_us;  // micros() from the last step

    uint16_t m_steps_per_rev;  // based on step mode
    float m_deg_to_step;
    bool m_at_limit;

    static std::vector<std::pair<uint8_t, StepperMotor*> > itr_list;

    static const uint32_t step_period_us = 2000;     // step_period_us
    static const uint16_t full_steps_per_rev = 200;  // 1.8 deg per step
    static const bool forward_direction = 1;         // reference
};
