#define NUMBER_OF_LEDS 4 //Define is a "preprocessor directive". It ensures that every NUMBER_OF_LEDS will be replaced by 4 in the following code
#define ALL_LEDS_MASK 0b00111100

// --- 1.11 LED Library ---

void enableLed (uint8_t lednumber);

void lightUpLed (uint8_t lednumber);

void lightDownLed (uint8_t lednumber);

void enableMultipleLeds(uint8_t leds);

void enableAllLeds();

//void lightUpOneLed(int); already implemented by lightUpLed();

void lightUpMultipleLeds(uint8_t leds);

void lightUpAllLeds(); 

//void lightDownOneLed(int); already implemented by lightDownLed();

void lightDownMultipleLeds(uint8_t leds);

void lightDownAllLeds(); 

void lightToggleOneLed(uint8_t lednumber);

void lightToggleMultipleLed(uint8_t leds);

void lightToggleAllLeds();

// --- 1.12 Dimmed LEDs ---

void dimLed(uint8_t lednumber, uint8_t percentange, int duration);  

void fadeInLed(uint8_t lednumber, int duration);  

void fadeOutLed(uint8_t lednumber, int duration);

