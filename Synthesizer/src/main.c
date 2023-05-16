#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/interrupt.h>

#include <fizzbuzzlib.h>
#include <display.h>
#include <adclib.h>
#include <ledlib.h>
#include <buttonlib.h>

#define SYNTH_MIN C2
#define SYNTH_MAX B6

#define SYNTH_BUTTON_TOGGLEPLAY BUTTON2

uint8_t playing = 0;

ISR(BUTTONS_INTERRUPT) {
  if(bit_is_clear(PINC, SYNTH_BUTTON_TOGGLEPLAY)) {
    _delay_us(1000);
    if(bit_is_clear(PINC, SYNTH_BUTTON_TOGGLEPLAY)) {
      playing ^= 1; // Toggle playing
    }
  }
}

int main() 
{
  initDisplay();
  initADC();

  enableButtonInterrupts();
  enableBuzzer();
  enableLed(0);

  while (1) 
  {
    // 0 => SYNTH_MIN, 1024 => SYNTH_MAX
    float freq = ((SYNTH_MAX-SYNTH_MIN) * readPinValue())/1024.0 + SYNTH_MIN;

    writeNumber((int) freq);

    if(playing) {
      lightUpLed(0);
      playTone(freq, 100);
    }
    lightDownLed(0);
  }
  
  return 0;
}
