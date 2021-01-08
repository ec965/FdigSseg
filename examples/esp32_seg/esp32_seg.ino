#include <Arduino.h>
#include "FdigSseg.h"
// The max size of a string that can be sent to segment display is 9
// example: '0.0.0.0.'
#define STRINGSIZE 9 
// size of the display queue
#define QSIZE 10

#define HARDWARETIMER 0 // there are 4 hardware timers numbered from 0 to 3
// the number to divide the base clock frequency by.
// Most boards operate at 80MHz
#define TIMERDIVISION 80 
// Using a timer division of 80 gives 1micro-sec per tick of the timer
// Setting a frequency of 1600 gives us about a 60hz refresh rate on the display
#define TIMERFREQ 1700 // about 60hz

// pins for the 4 digit 7 segment display
const uint8_t digit_pins[] = {26,27,14,12}; // digits
const uint8_t seg_pins[] = {21, 19, 18, 5, 17, 16, 4, 15}; // segments

FdigSseg segdisp (digit_pins, seg_pins, COMMON_ANODE);
// Queue to send strings to the display task
QueueHandle_t display_queue;
// task handler to refresh the display based on the timer
TaskHandle_t display_handle;
// display timer
hw_timer_t* display_timer = NULL;

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

// timer interrupt callback to refresh the segment display 
void IRAM_ATTR onTimer(){
    xTaskResumeFromISR(display_handle);
}

void setup(){
    Serial.begin(115200);

    segdisp.init();

    // initialize the queue
    display_queue = xQueueCreate(QSIZE, STRINGSIZE*sizeof(char));

    // init display refresh interrupt timer
    display_timer = timerBegin(HARDWARETIMER, TIMERDIVISION, true);
    timerAttachInterrupt(display_timer, &onTimer, true);
    timerAlarmWrite(display_timer, TIMERFREQ, true);
    timerAlarmEnable(display_timer);
    
    xTaskCreate(display_task, "display task", 4000, NULL, tskIDLE_PRIORITY, &display_handle);
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
