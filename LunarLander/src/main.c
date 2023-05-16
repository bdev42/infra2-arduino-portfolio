#include <avr/io.h>
#include <avr/interrupt.h>
#include <display.h>
#include <ledlib.h>
#include <usart.h>
#include <buttonlib.h>
// #define __DELAY_BACKWARD_COMPATIBLE__  
// #include <util/delay.h>

typedef struct {
  float currentSpeed; // speed in m/s (meters per second)
  float distance; // distance to the lunar surface in meters - m
  int fuelReserve; // liter
  float burst;
} LANDERSTATE;

#define INITSTATE_SPEED 100.0
#define INITSTATE_DISTANCE 9999.0
#define INITSTATE_FUELRESERVE 1500

#define GRAVITY 1.622; // acceleration in m/s² 

// Write the function showParameters which shows the distance to the surface
// on the 4-digit LED display and which makes the LEDs light up and flash
// depending on the remaining fuel content in the 4 fuel tanks.
void showParameters(LANDERSTATE *state) {
  writeNumber(state->distance);
  
  lightDownAllLeds();
  uint8_t fullLeds;
  if(state->fuelReserve > 0 && state->fuelReserve < INITSTATE_FUELRESERVE/4) {
    fullLeds = 0;
  }else if(state->fuelReserve < INITSTATE_FUELRESERVE/4*2) {
    fullLeds = 1;
  }else if(state->fuelReserve < INITSTATE_FUELRESERVE/4*3) {
    fullLeds = 2;
  }else if(state->fuelReserve < INITSTATE_FUELRESERVE/4*4) {
    fullLeds = 3;
  }else {
    fullLeds = 4;
  }

  for(uint8_t i = 0; i < fullLeds; i++) {
    lightUpLed(i);
  }
}

void logParameters(LANDERSTATE *state) {
  printString("=========================\n");
  printf("speed = %d m/s\ndistance = %d m\nfuel = %d l\nburst = %d\n", (int)state->currentSpeed, (int)state->distance, state->fuelReserve, (int)state->burst);
}

void updateParams(LANDERSTATE *state) {
  (*state).currentSpeed += (1.622 - (state->burst)/5.0);
  (*state).distance -= state->currentSpeed;
  (*state).fuelReserve -= state->burst;
}

uint8_t isSafeLanding(LANDERSTATE *state) {
  return (state->distance <= 3 && state->currentSpeed <= 5);
}

LANDERSTATE landerState = {
  INITSTATE_SPEED, INITSTATE_DISTANCE, INITSTATE_FUELRESERVE, 0
};

void tick() {
  updateParams(&landerState);
  logParameters(&landerState);
}

volatile uint8_t subtick_counter = 0;
volatile uint8_t burst_subticks = 0;

void initTimer2() {
  // CTC (0 to OCRA)
  TCCR2A |= _BV(WGM21);
  // TCCR2B |= _BV(WGM22);
  // 256 prescaling -> 16 MHz / 256 = 62500 Hz
  TCCR2B |= _BV(CS22) | _BV(CS21);
  // T(interrupt) = 250/62500 = 0,004 = 4 ms
  OCR2A = 250;
  TIMSK2 |= _BV(OCIE2A);   // enable OCRA timer interrupt
  sei();
}

// This ISR runs every 4 ms, but we only want to call tick every second
// 1/0,004 = 250
#define SUBTICK_LIMIT 250
// We will wait untill the subtick_counter reached this value
ISR(TIMER2_COMPA_vect) {
  if(subtick_counter < SUBTICK_LIMIT) {
    subtick_counter++;
    if(buttonPushed(BUTTON2)) burst_subticks++;
  }else {
    subtick_counter = 0;
    landerState.burst = burst_subticks/250.0*50.0;
    if(landerState.fuelReserve <= 0) landerState.burst = 0;
    burst_subticks = 0;
    tick();
  }
}

int main() {
  initDisplay();
  enableAllLeds();
  enableButton(BUTTON2);
  initTimer2();
  initUSART();

  while(landerState.distance > 0) {
    if(isSafeLanding(&landerState)) break;
    showParameters(&landerState);
  }
  // reset prescaler to 0
  TCCR2B &= ~(_BV(CS22) | _BV(CS21));

  if(isSafeLanding(&landerState)) {
    writeStringAndWait("LAND", 500);
    writeStringAndWait("ING ", 500);
    writeStringAndWait("SUCC", 500);
    writeStringAndWait("ESS!", 500);
  }else {
    writeStringAndWait("BOOM", 2000);
  }
  blankDisplay();

  return 0;
}
