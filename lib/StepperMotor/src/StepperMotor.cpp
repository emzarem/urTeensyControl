#include "StepperMotor.h"

#include <Arduino.h>

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
                           HPSDDecayMode decay_mode,
                           uint16_t current_lim_mA,
                           HPSDStepMode step_mode)
    : m_last_step_us(0), m_step_target(0), m_step_crnt(0) {
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

bool StepperMotor::set_angle(float angle_degrees, bool absolute) {
    int16_t new_step_target = (int16_t)(angle_degrees*m_deg_to_step) % m_steps_per_rev;
    m_step_target = absolute ? new_step_target : m_step_target + new_step_target;
    return true;
}
