#include <avr/io.h>
#include <avr/interrupt.h>

#define BUTTON1 PC1
#define BUTTON2 PC2
#define BUTTON3 PC3

#define BUTTON_MASK (_BV(BUTTON1) | _BV(BUTTON2) | _BV(BUTTON3))

#define BUTTONS_INTERRUPT PCINT1_vect
#define BUTTONS_PIN PINC

void enableButton( uint8_t button );

uint8_t isEdgeEvent(uint8_t *lastState);

uint8_t buttonPushed( uint8_t button );

uint8_t buttonReleased( uint8_t button );

void enableButtonInterrupts();
