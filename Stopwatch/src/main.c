#include <avr/interrupt.h>
#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h>
#include <display.h>
#include <usart.h>
#include <buttonlib.h>
#include <ledlib.h>

// Global variables
// Enter here your global variables
volatile uint16_t subtick_counter = 0;
volatile uint16_t stopwatch_seconds = 0;

void initTimer() {
  // Fast PWM (0 to OCRA)
  TCCR2A |= _BV(WGM21);
  // TCCR2B |= _BV(WGM22);
  // T(interrupt) = 250/62500 = 0,004 = 4 ms
  OCR2A = 250;
  TIMSK2 |= _BV(OCIE2A);   // enable OCRA timer interrupt
  sei();
}

void startTimer() {
  // 256 prescaling -> 16 MHz / 256 = 62500 Hz
  TCCR2B |= _BV(CS22) | _BV(CS21);
}

void stopTimer() {
  // reset prescaler to 0
  TCCR2B &= ~(_BV(CS22) | _BV(CS21));
}

ISR(BUTTONS_INTERRUPT) {
  if(bit_is_clear(BUTTONS_PIN, BUTTON1)) {
    _delay_us(1000);
    if(bit_is_clear(BUTTONS_PIN, BUTTON1)) {
      startTimer();
    }
  }

  if(bit_is_clear(BUTTONS_PIN, BUTTON2)) {
    _delay_us(1000);
    if(bit_is_clear(BUTTONS_PIN, BUTTON2)) {
      stopTimer();
    }
  }

  if(bit_is_clear(BUTTONS_PIN, BUTTON3)) {
    _delay_us(1000);
    if(bit_is_clear(BUTTONS_PIN, BUTTON3)) {
      stopwatch_seconds = 0;
    }
  }  
}

void tick() {
  lightUpLed(0);
  stopwatch_seconds++;
}

// This ISR runs every 4 ms, but we only want to call tick every second
// 1/0,004 = 250
#define SUBTICK_LIMIT 250
// We will wait untill the subtick_counter reached this value
ISR(TIMER2_COMPA_vect) {
  if(subtick_counter < SUBTICK_LIMIT) {
    subtick_counter++;
    lightDownLed(0);
  }else {
    subtick_counter = 0;
    tick();
  }
}

void writeTimeAndWait(uint8_t minutes, uint8_t seconds, int delay) {
  minutes = minutes % 100; // hard cap minutes at 99
  seconds = seconds % 60; // hard cap seconds at 59
  writeNumberAndWait(minutes * 100 + seconds, delay);
}

int main() {
  // initUSART();
  initDisplay();
  initTimer();

  enableButton(BUTTON1);
  enableButton(BUTTON2);
  enableButton(BUTTON3);
  enableButtonInterrupts();
  
  enableLed(0);

  // printf("Start the stopwatch by pressing button S1, stop by pressing button S2, and reset with S3\n");
  while (1) {
    writeTimeAndWait(stopwatch_seconds / 60, stopwatch_seconds % 60, 200);
  }
  return 0;
}