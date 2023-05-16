#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>

void pow2(volatile int* n) {
  *n = (*n)* (*n);
}

int main()
{
    initUSART();
    int var = 5;
    // calculate power 2 -> write as a function pow2
    pow2(&var);
    printf("Power 2 of 5 is %d", var);

    return 0;
}