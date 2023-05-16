#include <avr/io.h>
#include <util/delay.h>
#include <usart.h>

void initTimer1()
{
    // STEP 1: choose the WAVE FORM and by consequence the Mode of Operation
    // We choose 8 bit Fast PWM; TCNT1 counts to 255. We need to make changes to both registers
    TCCR1A |= _BV(WGM10) | _BV(COM1B1); // WGM10 = 1 and WGM12 = 1 --> 8 bit Fast PWM Mode
    TCCR1B |= _BV(WGM12);

    // STEP 2: *always* set a PRESCALER, otherwise the timer won't count
    // The counting speed is determined by the CPU clock (16 Mhz) divided by this factor
    TCCR1B |= _BV(CS12) | _BV(CS10); // CS12 = 1 and CS10 = 1 --> prescaler factor is now 1024 (= every 64 us)

    // STEP 3: enable compare output mode for OCR1B which is the same port as PB2 the first led on MFT shield
    // Clear OC1B on Compare Match, set OC1B at overflow
    TCCR1A |= _BV(COM1B1);
}

int main()
{
    initUSART();
    initTimer1();
    DDRB |= _BV(PB2); // Place the LEDs in output mode

    while (1)
    {
        printf("**********************************************************************************\n");
        printf("*********************************** Tick Tock Demo *******************************\n");
        printf("**********************************************************************************\n");

        printf("Timer 1 is set to increment its TCNT1 register every 64 us up to a maximum of 255.\n");
        printf("Observe the LEDs...\n");

        printf("Enter the OCR1B value in 3 digit format (000-255): ");
        OCR1B = getNumber(); // OCR0A can always be chosen freely between 0 and 255

        printf("Timer 1 generates *in the background* the waveform:\n");
        printf("\t*)and sets when the ORC1B (%d) is reached.\n", OCR1B);
        printf("\t*)and clears when TOP (%d) is reached.\n\n", 255);
    }

    return 0;
}