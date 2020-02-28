#pragma once

#include <AccelStepper.h>
#include <HighPowerStepperDriver.h>
#include <utility>
#include <vector>

/* Forward Declarations */

/* Class to Control Stepper Motors -- Uses AccelStepper */
class AccelMotor : public AccelStepper {
public:
    AccelMotor(uint8_t cs_pin,
               uint8_t lim_pin,
               uint8_t encA_pin,
               uint8_t encB_pin,
               HPSDDecayMode mode = HPSDDecayMode::AutoMixed,
               uint16_t current_lim_mA = 1000,
               HPSDStepMode step_mode = HPSDStepMode::MicroStep1,
               bool use_enc = false);  // Constructor
    ~AccelMotor();                     // Destructor

    bool set_angle(float angle_degrees, bool absolute);  // set the angle

    // Overrides
    void step(long step) override;


    static void calibrate(
        std::vector<AccelMotor *> mts);  // step until limit hit

    static const float max_angle_degrees = 90;  // as measured from the limit

private:
    /* Static functions */
    static void limit_switch_isr(void);

    /* Members */
    HighPowerStepperDriver *m_hpsd;

    // Relationships / Ratios
    uint16_t m_steps_per_rev;  // based on step mode
    float m_deg_to_step;

    // Flags
    bool m_at_limit;
    bool m_no_enc;

    /* Static Members */
    static std::vector<std::pair<uint8_t, AccelMotor *> > itr_list;

    static const uint16_t full_steps_per_rev = 200;  // 1.8 deg per step
    static const int16_t step_tol = 5;  // How close to target
};
