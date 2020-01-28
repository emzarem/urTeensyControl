#pragma once

#include <stdint.h>

/* Pinout */
// Sensors
const uint8_t PIN_LIM2 = 15; // M2
const uint8_t PIN_LIM3 = 16;
const uint8_t PIN_LIM1 = 17; // M1

// LEDs
const uint8_t STD_LED = 13;

// SPI
const int PIN_MOSI = 11;
const int PIN_MISO = 12;
const int PIN_SCK = 14;

// Motors
const int PIN_N_SLP = 6;
const int PIN_RST = 7;
const int PIN_M2_CS = 8; // M2
const int PIN_M3_CS = 9;
const int PIN_M1_CS = 10; // M1

/* Properties */
// Serial
const uint32_t SER_BAUD_RATE = 115200;

// SPI
const uint32_t SPI_FREQ = 500000;

