#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <display.h>

#define CHECKSTRINGS


#ifndef CHECKSTRINGS

int main()
{
    initUSART();
    initDisplay();
    while ( 1 )
    {
        for ( int i = 0; i < 4; i++ )
        {
            writeNumberToSegment( i, 8 );
            _delay_ms( 1000 );
        }
        /* Use the writeNumber function if
         * you want to display a 4 digit number. */
        writeNumber( 2001 );
        _delay_ms( 1000 );
        /* Problem: this function shows the digits 1 by 1 and then
         * adds a delay. So, only the last digit is displayed. 
         * Solution: to display the complete number, repeat this in a loop. */
        for ( int i = 0; i < 10000; i ++ )
        {
            writeNumber( 2004 );
        }
        /* Or you could use a function from the library.
         * Then you can also specify the display duration. */
        writeNumberAndWait( 2023, 10000 );
    }
    return 0;
}
#endif

#ifdef CHECKSTRINGS

int main() {
  initDisplay();
  initUSART();

  while (1){
    for (uint8_t i = 0; i < 10; i++){
      writeNumberAndWait((i * 1000 + ((i + 1)%10) * 100 + ((i + 2)%10) * 10 + (i+3)%10), 1000);
      writeCharAndWait(1, 'x', 1000);
    }
    writeStringAndWait("ABCD", 1000);
    writeStringAndWait("EFGH", 1000);
    writeStringAndWait("IJKL", 1000);
    writeStringAndWait("MNOP", 1000);
    writeStringAndWait("QRST", 1000);
    writeStringAndWait("UVWX", 1000);
    writeStringAndWait("YZ", 1000);
  }

}

#endif
