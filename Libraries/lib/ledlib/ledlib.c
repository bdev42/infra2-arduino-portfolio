#include <avr/io.h>
#include <ledlib.h>
// to allow variables as parameter for the _delay-functions (must be placed before the include of delay.h)
#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h>

void lightUpLed(uint8_t lednumber) {
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB &= ~( 1 << ( PB2 + lednumber ));  //Check the tutorial on "Bit Operations" to know what happens in this line.
}

void lightUpMultipleLeds(uint8_t leds) {
    PORTB &= ~(leds & ALL_LEDS_MASK);
}

void lightUpAllLeds() {
    PORTB &= ~ALL_LEDS_MASK;
}


void lightDownLed(uint8_t lednumber) {
    if ( lednumber < 0 || lednumber > 3 ) return;
    PORTB |= ( 1 << ( PB2 + lednumber ));   //Make sure you understand this line as well!
}

void lightDownMultipleLeds(uint8_t leds) {
    PORTB |= leds & ALL_LEDS_MASK;
}

void lightDownAllLeds() {
    PORTB |= ALL_LEDS_MASK;
}


void enableLed(uint8_t lednumber) {
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));    //Check the tutorial "Writing to a Pin". We know from the documentation on
                                            //the multifunctional shield that the LEDs start at PB2
    lightDownLed(lednumber);
}

void enableMultipleLeds(uint8_t leds) {
    DDRB |= leds & ALL_LEDS_MASK;
    lightDownMultipleLeds(leds);
}

void enableAllLeds() {
    DDRB |= ALL_LEDS_MASK;
    lightDownAllLeds();
}


void lightToggleOneLed(uint8_t lednumber) {
    if ( lednumber < 0 || lednumber > 3 ) return;
    PORTB ^= ( 1 << ( PB2 + lednumber ));
}

void lightToggleMultipleLed(uint8_t leds) {
    PORTB ^= leds & ALL_LEDS_MASK;
}

void lightToggleAllLeds() {
    PORTB ^= ALL_LEDS_MASK;
}



#define PWM_INCREMENT_MS 1 // Influences the smoothness of the PWM (less time = more steps ie. "more smooth")

void dimLed(uint8_t lednumber, uint8_t percentange, int duration) {
    double onTime = (percentange * PWM_INCREMENT_MS)/100.0;
    double offTime = ((100 - percentange) * PWM_INCREMENT_MS)/100.0;

    for(int i = 0; i < duration; i+=PWM_INCREMENT_MS) {
        lightUpLed(lednumber);
        _delay_ms(onTime);
        lightDownLed(lednumber);
        _delay_ms(offTime);
    }
}

// Note: duration cannot be smaller than 100 ms
// to combat this we might increase the step between percentages,
// (or just use floats for the durations)
// but this is more trouble than it is worth, since 100ms is quite
// a fast speed already and we have a limited amount of memory
// In the rare instance that a faster fading speed is needed,
// we can just call the dimLed() function on our own.
void fadeInLed(uint8_t lednumber, int duration) {
    int stepDuration = duration/100; 

    for(int i = 0; i < 100; i++) {
        dimLed(lednumber, i, stepDuration);
    }
}

// Note: duration cannot be smaller than 100 ms
// See -> fadeInLed
void fadeOutLed(uint8_t lednumber, int duration) {
    int stepDuration = duration/100;

    for(int i = 100; i > 0; i--) {
        dimLed(lednumber, i, stepDuration);
    }
}
