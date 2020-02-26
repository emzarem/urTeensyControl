#include "StepperMotor.h"

#include <Arduino.h>
#include <Encoder.h>

std::vector<std::pair<uint8_t, StepperMotor *> > StepperMotor::itr_list =
    std::vector<std::pair<uint8_t, StepperMotor *> >();

/* Function: calibrate
 * Inputs:
 *              None
 * Outputs:
 *              None
 */
void StepperMotor::calibrate(std::vector<StepperMotor *> mtrs) {
    bool done = false;
    for (StepperMotor *mtr : mtrs) {
        mtr->m_step_target = 0;
        mtr->setDirection(!StepperMotor::forward_direction);
    }

    while (!done) {
        done = true;
        for (StepperMotor *mtr : mtrs) {
            if (!mtr->m_at_limit) mtr->time_safe_step();
            done &= mtr->m_at_limit;
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
                           uint8_t encA_pin,
                           uint8_t encB_pin,
                           HPSDDecayMode decay_mode,
                           uint16_t current_lim_mA,
                           HPSDStepMode step_mode,
                           bool use_enc)
    : m_last_step_us(0),
      m_step_target(0),
      m_step_crnt(0),
      m_step_bkup(0),
      m_at_limit(false),
      m_no_enc(!use_enc) {
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

    // Setup encoder
    m_enc = new Encoder(encA_pin, encB_pin);
    m_enc->write(-1 * enc_cpr);  // -1 because encoder counts in opposite
                                 // direction to our convention

    // Setup interrupts
    pinMode(lim_pin, INPUT);
    itr_list.push_back(std::make_pair(lim_pin, this));
    StepperMotor::limit_switch_isr();  // in case we start against a switch
    attachInterrupt(lim_pin, StepperMotor::limit_switch_isr, CHANGE);
}


/* Function: ~StepperMotor
 * Inputs:
 *              None
 * Outputs:
 *              None
 */
StepperMotor::~StepperMotor() {
    delete m_enc;
}


/* Function: inc_steps
 * Inputs:
 *              None
 * Outputs:
 *              bool - success or not
 */
bool StepperMotor::inc_steps() {
    update_step_count();
    if (abs(m_step_crnt - m_step_target) < StepperMotor::step_tol) {
        return true;
    }

    bool dir = getDirection();

    // Correct direction
    if (m_step_crnt < m_step_target && !dir) {
        setDirection(StepperMotor::forward_direction);
    } else if (m_step_crnt > m_step_target && dir) {
        setDirection(!StepperMotor::forward_direction);
    }

    time_safe_step();
    m_step_bkup = dir ? m_step_crnt + 1 : m_step_crnt - 1;

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

    int16_t new_step_target =
        (int16_t)(angle_degrees * m_deg_to_step) % m_steps_per_rev;
    m_step_target =
        absolute ? new_step_target : m_step_target + new_step_target;
    return success;
}

/* Function: time_safe_step
 * Inputs:
 *              None
 * Outputs:
 *              None
 */
void StepperMotor::time_safe_step() {
    // Ensure not going faster than step period
    uint32_t elapsed_time = micros() - m_last_step_us;
    if (elapsed_time < step_period_us) {
        delayMicroseconds(step_period_us - elapsed_time);
    }

    step();
    m_last_step_us = micros();
}


/* Function: update_step_count
 * Inputs:
 *              None
 * Outputs:
 *              None
 */
void StepperMotor::update_step_count() {
    m_step_crnt =
        m_no_enc
            ? m_step_bkup
            : (int16_t)(StepperMotor::step_weight * m_step_crnt +
                        StepperMotor::enc_weight * enc_to_step(m_enc->read()));
}

/* Function: <limit_switch_isr>
 * Inputs:
 *              mtr - pointer to the motor
 * Outputs:
 *              None
 */
void StepperMotor::limit_switch_isr(void) {
    for (auto &mtr : itr_list) {
        if (digitalRead(mtr.first) == HIGH) {
            mtr.second->m_at_limit = true;
            mtr.second->m_step_crnt = 0;
            mtr.second->m_enc->write(0);
        } else {
            mtr.second->m_at_limit = false;
        }
    }
}
