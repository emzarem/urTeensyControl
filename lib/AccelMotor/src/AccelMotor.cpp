#include "AccelMotor.h"

#include <Arduino.h>

std::vector<std::pair<uint8_t, AccelMotor *> > AccelMotor::itr_list =
    std::vector<std::pair<uint8_t, AccelMotor *> >();

long AccelMotor::calibrate_time_us = 0;

/* Function: calibrate
 * Inputs:
 *              mtrs - vector of motors to calibrate
 * Outputs:
 *              None
 */
void AccelMotor::calibrate(std::vector<AccelMotor *> mtrs) {
    bool done = false;

    for (auto &mtr : mtrs) mtr->_direction = DIRECTION_CCW;

    while (!done) {
        done = true;
        for (AccelMotor *mtr : mtrs) {
            if (!mtr->m_at_limit) mtr->step(0);
            done &= mtr->m_at_limit;
        }
        delayMicroseconds(calibrate_time_us);
    }
}

/* Function: AccelMotor
 * Inputs:
 *              cs_pin - chip select pin for this motor
 *              decay_mode - see drv8711
 *              current_lim_mA - current limit in mA
 *              step_mode - MicroStep mode
 * Outputs:
 *              None
 */
AccelMotor::AccelMotor(uint8_t cs_pin,
                       uint8_t lim_pin,
                       uint8_t encA_pin,
                       uint8_t encB_pin,
                       uint16_t full_steps_per_rev,
                       HPSDDecayMode decay_mode,
                       uint16_t current_lim_mA,
                       HPSDStepMode step_mode,
                       bool use_enc)
    : AccelStepper(AccelStepper::FUNCTION),
      m_at_limit(false),
      m_max_angle_deg(90),
      m_no_enc(!use_enc) {
    // Setup stepper
    m_hpsd = new HighPowerStepperDriver();
    m_hpsd->setChipSelectPin(cs_pin);
    m_hpsd->resetSettings();
    m_hpsd->clearStatus();
    m_hpsd->setDecayMode(decay_mode);
    m_hpsd->setCurrentMilliamps36v4(current_lim_mA);
    m_hpsd->setStepMode(step_mode);
    m_hpsd->enableDriver();

    // Get steps per rev
    m_steps_per_rev = (uint16_t)step_mode * full_steps_per_rev;
    m_deg_to_step = (float)m_steps_per_rev / 360;

    long cal_time_us = 40000/(uint16_t)step_mode; // do a (full) step period of 40000 during cal
    if (AccelMotor::calibrate_time_us < cal_time_us)
        AccelMotor::calibrate_time_us = cal_time_us;

    // Setup interrupts
    pinMode(lim_pin, INPUT);
    itr_list.push_back(std::make_pair(lim_pin, this));
    AccelMotor::limit_switch_isr();  // in case we start against a switch
    attachInterrupt(lim_pin, AccelMotor::limit_switch_isr, CHANGE);
}


/* Function: ~AccelMotor
 * Inputs:
 *              None
 * Outputs:
 *              None
 */
AccelMotor::~AccelMotor() {
    delete m_hpsd;
}

/* Function: <set_max_angle>
 * Inputs:
 *              angle - angle to limit
 * Outputs:
 *              None
 */
void AccelMotor::set_max_angle(float angle) {
    m_max_angle_deg = angle;
}

/* Function: <set_angle>
 * Inputs:
 *              angle_degrees - angle to move to (degrees)
 *              absolute - true if absolute angle, false if relative
 * Outputs:
 *              bool - true if success (not currently checking fail)
 */
bool AccelMotor::set_angle(float angle_degrees, bool absolute) {
    bool success = true;

    if (angle_degrees > m_max_angle_deg) {
        angle_degrees = m_max_angle_deg;
        success = false;
    } else if (angle_degrees < 0) {
        angle_degrees = 0;
        success = false;
    }

    int16_t new_step_target =
        (int16_t)(angle_degrees * m_deg_to_step) % m_steps_per_rev;

    if (absolute)
        moveTo(new_step_target);
    else
        move(new_step_target);
    return success;
}


/* Function: <step>
 * Inputs:
 *              long - unused here (for compatibility)
 * Outputs:
 *              None
 */
void AccelMotor::step(long) {
    if (m_hpsd->getDirection() != _direction) m_hpsd->setDirection(_direction);
    m_hpsd->step();
}


/* Function: <limit_switch_isr>
 * Inputs:
 *              mtr - pointer to the motor
 * Outputs:
 *              None
 */
void AccelMotor::limit_switch_isr(void) {
    for (auto &mtr : itr_list) {
        if (digitalRead(mtr.first) == HIGH) {
            mtr.second->m_at_limit = true;
            long tmp = mtr.second->targetPosition();
            mtr.second->setCurrentPosition(0);
            mtr.second->moveTo(tmp);
            //            mtr.second->m_enc->write(0);
        } else {
            mtr.second->m_at_limit = false;
        }
    }
}
