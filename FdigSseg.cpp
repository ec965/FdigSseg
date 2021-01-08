#include <Arduino.h>
#include "FdigSseg.h"

Seg::Seg(const uint8_t* d, const uint8_t* s, bool common){
    digit = d;
    segment = s;
    if (common) { // COMMON_CATHODE
        d_on=HIGH;
        d_off=LOW;
        s_on=LOW;
        s_off=HIGH;
    } else { // COMMON_ANODE
        d_on=LOW;
        d_off=HIGH;
        s_on=HIGH;
        s_off=LOW;
    }
}
        // return 0 if error in init, else return 1
bool Seg::init(){
    uint8_t i;
    for(i=0; i<4; i++){
        pinMode(digit[i], OUTPUT);
    }
    for(i=0; i<8; i++){
        pinMode(segment[i], OUTPUT);
    }
    return 1;
}

//-----DIGIT-----
void Seg::d_all(){
    uint8_t i;
    for(i=0; i<4; i++){
        digitalWrite(digit[i], d_on);
    }
}
void Seg::d_clear(){
    uint8_t i;
    for(i=0; i<4; i++){
        digitalWrite(digit[i], d_off);
    }
}
void Seg::d(uint8_t n){
    for(uint8_t i=0; i<4; i++){
        if (i == n){
            digitalWrite(digit[n], d_on);
        } else {
            digitalWrite(digit[i], d_off);
        }
    }
}

//------SEGMENT-----

void Seg::s_clear(){
    uint8_t i;
    for(i=0; i<8; i++){
        digitalWrite(segment[i], s_off);
    }
}

void Seg::s(char n){
    switch(n){
        case '0': 
            s_zero();
            break;
        case '1': 
            s_one();
            break;
        case '2': 
            s_two();
            break;
        case '3': 
            s_three();
            break;
        case '4': 
            s_four();
            break;
        case '5': 
            s_five();
            break;
        case '6': 
            s_six();
            break;
        case '7': 
            s_seven();
            break;
        case '8': 
            s_eight();
            break;
        case '9': 
            s_nine();
            break;
        case 'c':
            s_c();
            break;
        case 'f':
            s_f();
            break;
        case 'a':
            s_a();
            break;
        case 'h':
            s_h();
            break;
        case '-':
            s_dash();
            break;
        default:
            s_clear();
    }
}
void Seg::segment_on(uint8_t seg){
    digitalWrite(segment[seg], s_on);
}
void Seg::segment_off(uint8_t seg){
    digitalWrite(segment[seg], s_off);
}

void Seg::s_dash(){
    digitalWrite(segment[0], s_off);
    digitalWrite(segment[1], s_off);
    digitalWrite(segment[2], s_off);
    digitalWrite(segment[3], s_off);
    digitalWrite(segment[4], s_off);
    digitalWrite(segment[5], s_off);
    digitalWrite(segment[6], s_on);
}

void Seg::s_a(){
    digitalWrite(segment[0], s_on);
    digitalWrite(segment[1], s_on);
    digitalWrite(segment[2], s_on);
    digitalWrite(segment[3], s_off);
    digitalWrite(segment[4], s_on);
    digitalWrite(segment[5], s_on);
    digitalWrite(segment[6], s_on);
}

void Seg::s_h(){
    digitalWrite(segment[0], s_off);
    digitalWrite(segment[1], s_on);
    digitalWrite(segment[2], s_on);
    digitalWrite(segment[3], s_off);
    digitalWrite(segment[4], s_on);
    digitalWrite(segment[5], s_on);
    digitalWrite(segment[6], s_on);
}

void Seg::s_f(){
    digitalWrite(segment[0], s_on);
    digitalWrite(segment[1], s_off);
    digitalWrite(segment[2], s_off);
    digitalWrite(segment[3], s_off);
    digitalWrite(segment[4], s_on);
    digitalWrite(segment[5], s_on);
    digitalWrite(segment[6], s_on);
}
void Seg::s_c(){
    digitalWrite(segment[0], s_on);
    digitalWrite(segment[1], s_off);
    digitalWrite(segment[2], s_off);
    digitalWrite(segment[3], s_on);
    digitalWrite(segment[4], s_on);
    digitalWrite(segment[5], s_on);
    digitalWrite(segment[6], s_off);
}

void Seg::s_zero(){
    digitalWrite(segment[0], s_on);
    digitalWrite(segment[1], s_on);
    digitalWrite(segment[2], s_on);
    digitalWrite(segment[3], s_on);
    digitalWrite(segment[4], s_on);
    digitalWrite(segment[5], s_on);
    digitalWrite(segment[6], s_off);
}
void Seg::s_one(){
    digitalWrite(segment[0], s_off);
    digitalWrite(segment[1], s_on);
    digitalWrite(segment[2], s_on);
    digitalWrite(segment[3], s_off);
    digitalWrite(segment[4], s_off);
    digitalWrite(segment[5], s_off);
    digitalWrite(segment[6], s_off);
}
void Seg::s_two(){
    digitalWrite(segment[0], s_on);
    digitalWrite(segment[1], s_on);
    digitalWrite(segment[2], s_off);
    digitalWrite(segment[3], s_on);
    digitalWrite(segment[4], s_on);
    digitalWrite(segment[5], s_off);
    digitalWrite(segment[6], s_on);
}
void Seg::s_three(){
    digitalWrite(segment[0], s_on);
    digitalWrite(segment[1], s_on);
    digitalWrite(segment[2], s_on);
    digitalWrite(segment[3], s_on);
    digitalWrite(segment[4], s_off);
    digitalWrite(segment[5], s_off);
    digitalWrite(segment[6], s_on);
}
void Seg::s_four(){
    digitalWrite(segment[0], s_off);
    digitalWrite(segment[1], s_on);
    digitalWrite(segment[2], s_on);
    digitalWrite(segment[3], s_off);
    digitalWrite(segment[4], s_off);
    digitalWrite(segment[5], s_on);
    digitalWrite(segment[6], s_on);
}
void Seg::s_five(){
    digitalWrite(segment[0], s_on);
    digitalWrite(segment[1], s_off);
    digitalWrite(segment[2], s_on);
    digitalWrite(segment[3], s_on);
    digitalWrite(segment[4], s_off);
    digitalWrite(segment[5], s_on);
    digitalWrite(segment[6], s_on);
}
void Seg::s_six(){
    digitalWrite(segment[0], s_on);
    digitalWrite(segment[1], s_off);
    digitalWrite(segment[2], s_on);
    digitalWrite(segment[3], s_on);
    digitalWrite(segment[4], s_on);
    digitalWrite(segment[5], s_on);
    digitalWrite(segment[6], s_on);
}
void Seg::s_seven(){
    digitalWrite(segment[0], s_on);
    digitalWrite(segment[1], s_on);
    digitalWrite(segment[2], s_on);
    digitalWrite(segment[3], s_off);
    digitalWrite(segment[4], s_off);
    digitalWrite(segment[5], s_off);
    digitalWrite(segment[6], s_off);
}
void Seg::s_eight(){
    digitalWrite(segment[0], s_on);
    digitalWrite(segment[1], s_on);
    digitalWrite(segment[2], s_on);
    digitalWrite(segment[3], s_on);
    digitalWrite(segment[4], s_on);
    digitalWrite(segment[5], s_on);
    digitalWrite(segment[6], s_on);
}
void Seg::s_nine(){
    digitalWrite(segment[0], s_on);
    digitalWrite(segment[1], s_on);
    digitalWrite(segment[2], s_on);
    digitalWrite(segment[3], s_on);
    digitalWrite(segment[4], s_off);
    digitalWrite(segment[5], s_on);
    digitalWrite(segment[6], s_on);
}

void Seg::s_dot_on(){
    digitalWrite(segment[7], s_on);
}
void Seg::s_dot_off(){
    digitalWrite(segment[7], s_off);
}

//-----DISPLAY-----//
FdigSseg::FdigSseg(const uint8_t* d, const uint8_t* s, bool common)
    :Seg(d,s,common)
{
    digit_inc=0;
    dot_counter=0;
}

void FdigSseg::show_digit(uint8_t digit, char c, bool period){
    d(digit);
    s(c);
    if (period){
        s_dot_on();
    } else {
        s_dot_off();
    }
}

void FdigSseg::display_string(char* str){
    switch (digit_inc){
        case 0:
            if (str[1] == '.'){
                show_digit(0, str[0], true);
                dot_counter++;
            } else {
                show_digit(0, str[0], false);
            }
            digit_inc++;
            break;
        case 1:
            if (str[2+dot_counter] == '.'){
                show_digit(1, str[1+dot_counter], true);
                dot_counter++;
            } else {
                show_digit(1, str[1+dot_counter], false);
            }
            digit_inc++;
            break;
        case 2:
            if (str[3+dot_counter] == '.'){
                show_digit(2, str[2+dot_counter], true);
                dot_counter++;
            } else {
                show_digit(2, str[2+dot_counter], false);
            }
            digit_inc++;
            break;
        case 3:
            if (str[4+dot_counter] == '.'){
                show_digit(3, str[3+dot_counter], true);
                dot_counter++;
            } else {
                show_digit(3, str[3+dot_counter], false);
            }
            digit_inc=0;
            dot_counter=0;
            break;
    }
}