#include "StepperMotor.h"


/* Function: StepperMotor
 * Inputs:
 *              cs_pin - chip select pin for this motor
 * Outputs:
 *              None
 */
StepperMotor(uint8_t cs_pin) {
    setChipSelectPin(cs_pin);
}

/* Function: ~StepperMotor
 * Inputs:
 *              None
 * Outputs:
 *              None
 */
~StepperMotor() {}
