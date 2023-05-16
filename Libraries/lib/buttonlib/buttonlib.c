#include <buttonlib.h>

void enableButton( uint8_t button ) {
    DDRC &= ~(1 << (PC0 + button));
}

uint8_t isEdgeEvent(uint8_t *lastState) {
    uint8_t res = (PINC ^ *lastState) == 0 ? 0 : 1;
    *lastState = PINC;
    return res;
}

uint8_t buttonPushed(uint8_t button) {
    if(button <= 0 || button > 3) return 0;

    if(bit_is_clear(PINC, (PC0 + button))) return 1;

    return 0;
}

uint8_t buttonReleased(uint8_t button) {
    if(button <= 0 || button > 3) return 0;

    if(bit_is_set(PINC, (PC0 + button))) return 1;

    return 0;
}

void enableButtonInterrupts() {
    PCICR |= _BV( PCIE1 );  /* in Pin Change Interrupt Control Register: indicate
                             * which interrupt(s) you want to activate (PCIE0: port B,
                             * PCIE1: port C, PCIE2: port D) */
    PCMSK1 |= BUTTON_MASK;   /* In the corresponding Pin Change Mask Register: indicate
                                 * which pin(s) of that port activate the ISR. */
    sei();
}
