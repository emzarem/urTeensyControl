#pragma once

/* Pinout */
// LEDs
const uint8_t STD_LED = 13;

// SPI
const int PIN_MOSI = 11;
const int PIN_MISO = 12;
const int PIN_SCK = 14;

// Motors
const int PIN_M1_CS = 10;
const int PIN_M2_CS = STD_LED;
const int PIN_M3_CS = STD_LED;

/* Properties */
// Serial
const uint32_t SER_BAUD_RATE = 115200;

// SPI
const uint32_t SPI_FREQ = 500000;

