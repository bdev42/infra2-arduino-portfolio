#include <avr/io.h>

#define LOW 0
#define HIGH 1
/* Define shift register pins used for seven segment display */
#define LATCH_DIO PD4
#define CLK_DIO PD7
#define DATA_DIO PB0

#define LSBFIRST 0
#define MSBFIRST 1
#define NUMBER_OF_SEGMENTS 8

#define sbi(register, bit) (register |= _BV(bit))
#define cbi(register, bit) (register &= ~_BV(bit))

// Extra Glyphs // [DPgfedcba]
#define GLYPH_SPACE 0xFF
#define GLYPH_EQUALS 0b10110111

void initDisplay();
void blankDisplay();

void writeGlyphToSegment(uint8_t segment, uint8_t glyph);
void writeNumberToSegment(uint8_t segment, uint8_t value);
void writeNumber(int number);
void writeNumberAndWait(int number, int delay);
void writeCharToSegment(uint8_t segment, char character);
void writeCharAndWait(uint8_t segment, char character, int delay);
void writeString(char* str);
void writeStringAndWait(char* str, int delay);
void writeHexToSegment(uint8_t segment, uint8_t nibble);
void writeHexWordAndWait(uint16_t word, int delay);
