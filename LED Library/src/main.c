#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <util/delay.h>
#include <avr/io.h>
#include <ledlib.h>

// 1. Write a program to light up the 4 LEDs of the shield consecutively
void basicExercise1() {
  enableAllLeds();
  for(uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
    lightUpLed(i);
    _delay_ms(500);
  }
}

// 2. Write a program to make one of the LEDs light exactly 10 times
// 3. Turn exercise 2 into a function, where the parameters are the LED number and the number of flashes. (add the function to your library)
void basicExercise2(uint8_t led, uint8_t flashes) {
  enableLed(led);
  for(uint8_t i = 0; i < flashes; i++) {
    lightUpLed(led);
    _delay_ms(250);
    lightDownLed(led);
    _delay_ms(250);
  }
}

// 4. Write a program that makes an LED burn longer and longer (start with 10 ms and rise up to 1000 ms, incrementing with 50 ms in every iteration.
void basicExercise4() {
  enableLed(0);
  for(int i = 10; i < 1000; i+=50) {
    lightUpLed(0);
    _delay_ms(i);
    lightDownLed(0);
    _delay_ms(i);
  }
}

// 5. Write a program that uses an array of 10 digits between 0 and 4. Walk through the array and light up the corresponding led.
void basicExercise5(uint8_t *ledSequence, uint8_t seqSize) {
  enableAllLeds();
  for(int i = 0; i < seqSize; i++) {
    lightUpLed(ledSequence[i]);
    _delay_ms(200);
    lightDownLed(ledSequence[i]);
    _delay_ms(200);
  }
}

// 6. Write a program that creates an empty array and then fills it with the numbers from 10 to 1000 with a step value of 50 ms.
//  Use this array to light up a led, just like in exercise 4.
void basicExercise6(int start, int end, int step, uint8_t led) {
  int arraySize = (end - start)/step;
  int timeArray[arraySize];

  for(int i = 0; i < arraySize; i++) {
    timeArray[i] = start + step * i;
  }

  enableAllLeds(led);
  for(int i = 0; i < arraySize; i++) {
    lightUpLed(led);
    _delay_ms(timeArray[i]);
    lightDownLed(led);
    _delay_ms(timeArray[i]);
  }
}

// 7. Write a program that makes the leds light up based on a string. 
// The string contains a sequence of the letters a, b, c and d. An 'a' lights the first LED, 'b' the second and so on.
void basicExercise7(char *string, int len) {
  enableAllLeds();
  for(int i = 0; i < len; i++) {
    uint8_t led;
    if(string[i]=='a') {
      led = 0;
    }else if(string[i]=='b') {
      led = 1;
    }else if(string[i]=='c') {
      led = 2;
    }else if(string[i]=='d') {
      led = 3;
    }else {
      continue;
    }
    lightUpLed(led);
    _delay_ms(200);
    lightDownLed(led);
    _delay_ms(200);
  }
}

// 8. Write a program that lights up the leds based on a string. But this time the string of letters is random.
//  With the letter 'a' all 4 LEDs light up once, with a letter 'b' they light up twice, and so on.
void basicExercise8(char *string, int len) {
  enableAllLeds();
  for(int i = 0; i < len; i++) {
    uint8_t times;
    if(string[i]=='a') {
      times = 1;
    }else if(string[i]=='b') {
      times = 2;
    }else if(string[i]=='c') {
      times = 3;
    }else if(string[i]=='d') {
      times = 4;
    }else {
      continue;
    }
    for(int j = 0; j < times; j++) {
      lightUpAllLeds();
      _delay_ms(100);
      lightDownAllLeds();
      _delay_ms(100);
    }
    _delay_ms(500);
  }
}

int main()
{
  blankDisplay();
  enableLed(0);
  while (1)
  {
    fadeInLed(0, 1000);
    fadeOutLed(0, 1000);
  }
  

  // uint8_t sequence[] = {1, 1, 1, 0, 0, 3, 3, 3, 2, 2}; 
  // basicExercise5(sequence, sizeof(sequence)/sizeof(uint8_t));

  //basicExercise6(10, 1000, 50, 0);


  // char sequence[] = "abcd";
  // int length = sizeof(sequence)/sizeof(char);

  // basicExercise8(sequence, length);
  return 0;
}