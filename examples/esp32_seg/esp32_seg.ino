#include <Arduino.h>
#include "FdigSseg.h"

#define HARDWARETIMER 0 // there are 4 hardware timers numbered from 0 to 3
// the number to divide the base clock frequency by.
// Most boards operate at 80MHz
#define TIMERDIVISION 80 
// Using a timer division of 80 gives 1micro-sec per tick of the timer
// Setting a frequency of 1600 gives us about a 60hz refresh rate on the display
#define TIMERFREQ 1700 // about 60hz

// pins for the 4 digit 7 segment display
const uint8_t digit_pins[] = {14, 4, 16, 5}; // digits
const uint8_t seg_pins[] = {12, 17, 19, 22, 23, 15, 18, 21}; // segments

FdigSseg segdisp (digit_pins, seg_pins, COMMON_ANODE);


void setup(){
    Serial.begin(115200);

    segdisp.init(HARDWARETIMER, TIMERDIVISION, TIMERFREQ);
}

void loop(){
    char str[SEG_MAX_STRING_SIZE] = "1234";
    segdisp.display(str);
    delay(1000); // this will not block, it is equivalent to vTaskDelay(1000 / portTICK_PERIOD_MS)
    
    sprintf(str, "2341");
    segdisp.display(str);
    delay(1000); 

    sprintf(str, "3412");
    segdisp.display(str);
    delay(1000); 

    sprintf(str, "4123");
    segdisp.display(str);
    delay(1000);
}
