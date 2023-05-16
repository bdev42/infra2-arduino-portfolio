#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <adclib.h>
#include <display.h>


int main()
{
    initUSART();
    initADC();
    initDisplay();
    while ( 1 )
    {
        uint16_t value = readPinValue();
        writeNumberAndWait(value, 1000);
        printf( "Value: %d\n", value );
    }
    return 0;
}
