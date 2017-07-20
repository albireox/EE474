// leds.h
//
// Conor Sayres and José Sánchez-Gallego

#ifndef GPIO_H
#define GPIO_H

#include "bool.h"


// Prototypes for functions defined in gpio.c

void setThrustDown(int value);
void setThrustUp(int value);
void setThrustLeft(int value);
void setThrustRight(int value);
int getBatteryLevel();

#endif
