#include <avr/io.h>

// === NOTE FREQUENCY DEFINITIONS === //
// #define C
// #define D
// #define E
// #define F
// #define G
// #define A
// #define B

#define C2 65.410
#define D2 73.420
#define E2 82.410
#define F2 87.310
#define G2 98.000
#define A2 110.00
#define B2 123.47

#define C3 130.810
#define D3 146.830
#define E3 164.810
#define F3 174.610
#define G3 196.000
#define A3 220.000
#define B3 246.940

#define C4 261.630
#define D4 293.660
#define E4 329.630
#define F4 349.230
#define G4 392.000
#define A4 440.000
#define B4 493.880

#define C5 523.250
#define D5 587.330
#define E5 659.250
#define F5 698.460
#define G5 783.990
#define A5 880.000
#define B5 987.770

#define C6 1046.500
#define D6 1174.660
#define E6 1318.510
#define F6 1396.910
#define G6 1567.980
#define A6 1760.000
#define B6 1975.530

void enableBuzzer();

void playTone( float frequency, uint16_t duration );
