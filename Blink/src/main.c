#include <Arduino.h>

uint8_t LED_RANGE_START = 10;
uint8_t LED_RANGE_SIZE = 4;

int8_t ballPos = 0;
int8_t ballDir = 1;

void setup() {
  // put your setup code here, to run once:
  for (int8_t i = 0; i < LED_RANGE_SIZE; i++)
  {
    pinMode(LED_RANGE_START+i, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int8_t i = 0; i < LED_RANGE_SIZE; i++)
  {
    if(ballPos != i) {
      digitalWrite(LED_RANGE_START+i, HIGH);
    }else {
      digitalWrite(LED_RANGE_START+i, LOW);
    }
  }
  delay(42);
  if(ballPos == LED_RANGE_SIZE || ballPos < 0) {
    ballDir *= -1;
  }
  ballPos += ballDir;
}
