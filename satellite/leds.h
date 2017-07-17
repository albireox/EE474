// leds.h
//
// Conor Sayres and José Sánchez-Gallego

#ifndef LEDS_H
#define LEDS_H

#include "bool.h"


// Prototypes for functions defined in leds.c

void setLED(unsigned short ledID, Bool on, unsigned int delay_on,
            unsigned int delay_off);

void turnOnLED(unsigned short ledID);

void turnOffLED(unsigned short ledID);

void flashLED(unsigned short ledID, unsigned int delay_on,
              unsigned int delay_off);
void turnAllOff();

#endif
