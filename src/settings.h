#pragma once

/* Motor Settings */
const uint32_t CURRENT_LIMIT_MA = 2500;

// 1,2,4,8,16...,256
const uint16_t STEP_MODE = 64;

// Slow                = 0b000,
// SlowIncMixedDec     = 0b001,
// Fast                = 0b010,
// Mixed               = 0b011,
// SlowIncAutoMixedDec = 0b100,
// AutoMixed           = 0b101,
const uint8_t DECAY_MODE = 0b101;

// Dynamics
const float MAX_SPEED_DEG_S = 114;
const float ACCEL_DEG_S_S = 57;

// Params
static const uint16_t STEPS_PER_REV = 200;
static const float MAX_ANGLE_DEG = 90;
