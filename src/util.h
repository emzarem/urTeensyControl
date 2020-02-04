#pragma once

#include <stdint.h>

/* Pinout */
// Sensors
const uint8_t PIN_LIM2  = 15; // M2
const uint8_t PIN_LIM3  = 16;
const uint8_t PIN_LIM1  = 17; // M1

const uint8_t PIN_ENC1A = 18;
const uint8_t PIN_ENC1B = 19;
const uint8_t PIN_ENC2A = 20;
const uint8_t PIN_ENC2B = 21;
const uint8_t PIN_ENC3A = 22;
const uint8_t PIN_ENC3B = 23;

// LEDs
const uint8_t STD_LED = 13;

// SPI
const uint8_t PIN_MOSI = 11;
const uint8_t PIN_MISO = 12;
const uint8_t PIN_SCK = 14;

// Motors
const uint8_t PIN_N_SLP = 6;
const uint8_t PIN_RST = 7;
const uint8_t PIN_M2_CS = 8; // M2
const uint8_t PIN_M3_CS = 9;
const uint8_t PIN_M1_CS = 10; // M1

/* Properties */
// Serial
const uint32_t SER_BAUD_RATE = 115200;

// SPI
const uint32_t SPI_FREQ = 500000;

