#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <ledlib.h>
#include <buttonlib.h>
#include <display.h>

#define BUTTON 1
#define LED 1

#define DEBUG

int main()
{   
    blankDisplay();
    enableLed(LED);
    enableButton(BUTTON);
    
    #ifdef DEBUG
    initUSART();
    printf("Program reset. \n");
    #endif

    uint8_t lastState;
    while (1)
    {
        if(isEdgeEvent(&lastState)) {
            if(buttonPushed(BUTTON)) {
                #ifdef DEBUG
                printf("Button %d pressed.\n", BUTTON);
                #endif
                lightUpLed(LED);
            }
            if(buttonReleased(BUTTON)) {
                #ifdef DEBUG
                printf("Button %d released.\n", BUTTON);
                #endif
                lightDownLed(LED);
            }
        }
    }
    

}