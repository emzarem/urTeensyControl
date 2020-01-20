#include "StepperMotor.h"

#include <Arduino.h>

std::vector<std::pair<uint8_t, StepperMotor*> > StepperMotor::itr_list =
    std::vector<std::pair<uint8_t, StepperMotor*> >();

/* Function: calibrate
 * Inputs:
 *              None
 * Outputs:
 *              None
 */
void StepperMotor::calibrate(std::vector<StepperMotor*> mtrs) {
    while (mtrs.size() > 0) {
        for (int i = 0; i < mtrs.size(); i++) {
            StepperMotor* mtr = mtrs[i];
            if (mtr->m_at_limit) {
                mtr->m_step_target = 0;
                mtrs.erase(mtrs.begin() + i);
                i--;
            } else {
                mtr->m_step_target = mtr->m_step_crnt + 1;
                mtr->inc_steps();
            }
        }
    }
}

/* Function: StepperMotor
 * Inputs:
 *              cs_pin - chip select pin for this motor
 *              decay_mode - see drv8711
 *              current_lim_mA - current limit in mA
 *              step_mode - MicroStep mode
 * Outputs:
 *              None
 */
StepperMotor::StepperMotor(uint8_t cs_pin,
                           uint8_t lim_pin,
                           HPSDDecayMode decay_mode,
                           uint16_t current_lim_mA,
                           HPSDStepMode step_mode)
    : m_last_step_us(0), m_step_target(0), m_step_crnt(0), m_at_limit(false) {
    // Setup stepper
    setChipSelectPin(cs_pin);
    resetSettings();
    clearStatus();
    setDecayMode(decay_mode);
    setCurrentMilliamps36v4(current_lim_mA);
    setStepMode(step_mode);
    enableDriver();

    // Get steps per rev
    m_steps_per_rev = (uint16_t)step_mode * StepperMotor::full_steps_per_rev;
    m_deg_to_step = (float)m_steps_per_rev / 360;

    // Setup interrupts
    pinMode(lim_pin, INPUT);
    itr_list.push_back(std::make_pair(lim_pin, this));
    attachInterrupt(lim_pin, StepperMotor::limit_switch_isr, CHANGE);
}


/* Function: ~StepperMotor
 * Inputs:
 *              None
 * Outputs:
 *              None
 */
StepperMotor::~StepperMotor() {}


/* Function: inc_steps
 * Inputs:
 *              None
 * Outputs:
 *              bool - success or not
 */
bool StepperMotor::inc_steps() {
    if (m_step_crnt == m_step_target) { return true; }

    bool dir = getDirection();

    // Correct direction
    if (m_step_crnt < m_step_target && !dir) {
        setDirection(StepperMotor::forward_direction);
    } else if (m_step_crnt > m_step_target && dir) {
        setDirection(!StepperMotor::forward_direction);
    }

    // Ensure not going faster than step period
    uint32_t elapsed_time = micros() - m_last_step_us;
    if (elapsed_time < step_period_us) {
        delayMicroseconds(step_period_us - elapsed_time);
    }

    step();
    m_step_crnt = dir ? m_step_crnt + 1 : m_step_crnt - 1; 
    m_last_step_us = micros();

    // Check for errors
    return false;
}

/* Function: <set_angle>
 * Inputs:
 *              angle_degrees - angle to move to (degrees)
 *              absolute - true if absolute angle, false if relative
 * Outputs:
 *              bool - true if success (not currently checking fail)
 */
bool StepperMotor::set_angle(float angle_degrees, bool absolute) {
    bool success = true;

    if (angle_degrees > max_angle_degrees) {
        angle_degrees = max_angle_degrees;
        success = false;
    } else if (angle_degrees < 0) {
        angle_degrees = 0;
        success = false;
    }

    int16_t new_step_target = (int16_t)(angle_degrees*m_deg_to_step) % m_steps_per_rev;
    m_step_target = absolute ? new_step_target : m_step_target + new_step_target;
    return success;
}

/* Function: <limit_switch_isr>
 * Inputs:
 *              mtr - pointer to the motor
 * Outputs:
 *              None
 */
void StepperMotor::limit_switch_isr(void) {
    for (auto& mtr : itr_list) {
        if (digitalRead(mtr.first) == LOW) {
            mtr.second->m_at_limit = true;
            mtr.second->m_step_crnt = 0;
        } else {
            mtr.second->m_at_limit = false;
        }
    }
}
