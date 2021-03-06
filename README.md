# Four Digit Seven Segment Library (ESP32)
![4dig7seg](https://external-content.duckduckgo.com/iu/?u=http%3A%2F%2Fwww.learningaboutelectronics.com%2Fimages%2F4-digit-7-segment-LED-display-pinout.png&f=1&nofb=1)

![7seg](https://external-content.duckduckgo.com/iu/?u=http%3A%2F%2Fwww.circuitbasics.com%2Fwp-content%2Fuploads%2F2017%2F05%2FArduino-7-Segment-Display-Tutorial-Segment-Layout-Diagram.png&f=1&nofb=1)

A library for interfacing with a seven segment display using an ESP32 and the [Arduino ESP32 HAL](https://github.com/espressif/arduino-esp32).

## Supported Characters
* __Numbers:__ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
* __Letters:__ a, c, f, h
* __Punctuation:__ - (dash), . (period)

## General Usage
You can intialize the `FdigSseg` object with the digit pins, segment pins, and either `COMMON_ANODE` or `COMMON_CATHODE` macros.
For example:
```C++
const uint8_t digits[] = {1,2,3,4};
const uint8_t segments[] = {5,6,7,8,9,10,11,12};

FdigSseg segdisplay(digits, segments, COMMON_ANODE);
```

Digits can be addressed using `d(<digit number>)`.
Digits start at index 0 and go to 3.
For example, to set the first digit to show '1':
```C++
segdisplay.d(0);
segdisplay.s('1');
```

All digits can be turned off using `d_clear()` and turned on using `d_all()`.

Characters can be shown per segment.
See above for a list of supported characters.
For example, counting up from 1 to 4 on the first digit:
```C++
segdisplay.d(0);
for(int i=1; i<5; i++){
    segdisplay.s(i + '0'); // convert int to character
    delay(1000);
}
```

Individual segments can also be addressed with `segment_on(<segment number>)`. 
Segments are indexed from 0 to 7. 
* The 'A' segment corresponds with index 0.
* The 'B' segment corresponds with index 1.
* The 'C' segment corresponds with index 2.
* The 'D' segment corresponds with index 3.
* The 'E' segment corresponds with index 4.
* The 'F' segment corresponds with index 5.
* The 'G' segment corresponds with index 6.

For example, turning on the 'A' segment on the first digit, then turning it off after 1 sec:
```C++
segdisplay.d(0);
segdisplay.segment_on(0);
delay(1000);
segdisplay.segment_off(0);
```

The dot/period is also individually addressable. 
For example, to turn on the dot on the first digit, then turn it off after 1 sec:
```C++
segdisplay.d(0);
segdisplay.s_dot_on();
delay(1000);
segdisplay.s_dot_off();
```

Entire strings can be displayed at a time using `display_string()`.
This funciton must be used with the hardware timer.
The next section goes over how to configure the display for use with the hardware timer.

## Usage with Hardware Timer
Full example code can be found in the examples folder as [esp32_seg.ino](examples/esp32_seg/esp32_seg.ino).

TLDR:
1. Set up a timer.
2. Create a display task to run the display. Suspend the display task based on the timer interrupt.
3. Create a queue to send strings to the display task.

To display different numbers on each digit, the display switches at a high speed between digits to fool the human eye.
A timer is needed to set the switching (i.e. refresh rate) of the display.
The ESP32 has four hardware timers.
These timers can set up like so:
```C++
// size of the display queue
#define QSIZE 10
// there are 4 hardware timers numbered from 0 to 3
#define HARDWARETIMER 0 
// the number to divide the base clock frequency by.
// Most boards operate at 80MHz
#define TIMERDIVISION 80 
// Using a timer division of 80 gives 1micro-sec per tick of the timer
// Setting a frequency of 1600 gives us about a 60hz refresh rate on the display
#define TIMERFREQ 1700 
hw_timer_t* display_timer = NULL;

// timer interrupt callback to refresh the segment display 
void IRAM_ATTR onTimer(){
    xTaskResumeFromISR(display_handle); // handle that is assigned to the display task
}

void setup(){
    // init display refresh interrupt timer
    display_timer = timerBegin(HARDWARETIMER, TIMERDIVISION, true);
    timerAttachInterrupt(display_timer, &onTimer, true); // assign the ISR function here
    timerAlarmWrite(display_timer, TIMERFREQ, true);
    timerAlarmEnable(display_timer);
}
```

You must use a task hanlder to connect the timer interrupt to the display task.
```C++
#include "FdigSseg.h"
// pins for the 4 digit 7 segment display
const uint8_t digit_pins[] = {26,27,14,12}; // digits
const uint8_t seg_pins[] = {21, 19, 18, 5, 17, 16, 4, 15}; // segments
// four digit seven segment display object
FdigSseg segdisp (digit_pins, seg_pins, COMMON_ANODE);
// task handler to refresh the display based on the timer
TaskHandle_t display_handle;

// - snip - //

void setup(){
    Serial.begin(115200);
    segdisp.init();

    // - snip - //
    
    xTaskCreate(display_task, 
        "display task", 
        4000, 
        NULL, 
        tskIDLE_PRIORITY, // tskIDLE_PRIORITY must be enabled when using a task handle
        &display_handle // bind the display task handle to the display task
    ); 
}
```

A display task will handle showing numbers on the display.
The display task will unsuspend based on an interrupt generated by the timer.
```C++
// The max size of a string that can be sent to segment display is 9
// example: '0.0.0.0.'
#define STRINGSIZE 9 

// - snip - //

void display_task(void * paramter){
    char str[STRINGSIZE];
    while(1){
        vTaskSuspend(display_handle); // suspend the task while waiting for the next timer cycle
        if (xQueueReceive(display_queue, &str, 0) == pdTRUE){ // don't block
            Serial.print("Displaying:");
            Serial.println(str);
        }
        segdisp.display_string(str); // display the string
    }
    vTaskDelete(NULL);
}
```

Finally, you can use a queue to send strings into the display task.
```C++
// size of the display queue
#define QSIZE 10

// - snip - //

// Queue to send strings to the display task
QueueHandle_t display_queue;

void setup(){
    
    // - snip - //

    // initialize the queue
    display_queue = xQueueCreate(QSIZE, STRINGSIZE*sizeof(char));
    
    // - snip - //

}

void loop(){
    char str[STRINGSIZE] = "1234";
    xQueueSend(display_queue, &str, portMAX_DELAY);
    delay(1000); // this will not block, it is equivalent to vTaskDelay(1000 / portTICK_PERIOD_MS)
    
    sprintf(str, "2341");
    xQueueSend(display_queue, &str, portMAX_DELAY);
    delay(1000); 

    sprintf(str, "3412");
    xQueueSend(display_queue, &str, portMAX_DELAY);
    delay(1000); 

    sprintf(str, "4123");
    xQueueSend(display_queue, &str, portMAX_DELAY);
    delay(1000);
}
```

Full example code can be found in the examples folder as [esp32_seg.ino](examples/esp32_seg/esp32_seg.ino).

## Single Digit 7 Segment Display
While the library is made for a four digit seven segment display, a one digit seven segment display can also be used by only have one pin number in the digits array.
```C++
const uint8_t digit_pin[] = {26}; // single digit display
const uint8_t seg_pins[] = {21, 19, 18, 5, 17, 16, 4, 15}; // segments
FdigSseg segdisp (digit_pin, seg_pins, COMMON_ANODE);
```

## Multi Digit 7 Segment Display
This library can be used with multiple 7 segment displays as long as all digit pins are defined and all segment pins are connected together.
Using multiple digits with the 7 segment display may require a faster refresh rate on the hardware timer.
```C++
const uint8_t digit_pin[] = {1,2,3,6,7,26}; // multiple seven segment displays connected together
const uint8_t seg_pins[] = {21, 19, 18, 5, 17, 16, 4, 15}; // segments
FdigSseg segdisp (digit_pin, seg_pins, COMMON_ANODE);
```
