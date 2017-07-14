// warningAlarm.c
//
// Conor Sayres and José Sánchez-Gallego

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "bool.h"
#include "satellite.h"
#include "leds.h"


const unsigned char led1_1 = 0x01;  // LED1 flash 1 second interval, battery <= 10%
const unsigned char led1_2 = 0x02;  // LED1 flash 2 second interval, battery <= 50%
const unsigned char led2_1 = 0x04;  // LED2 flash 1 second interval, fuel <= 10%
const unsigned char led2_2 = 0x08;  // LED2 flash 2 second interval, fuel <= 50%
const unsigned char led3 = 0x10;    // LED3 solid, battery and fuel > 50%


void applyNewLEDs(int leds){

    if (leds & led1_1)
        flashLED(0, 1000, 1000);

    if (leds & led1_2)
        flashLED(0, 2000, 2000);

    if (leds & (led1_1 & led1_2))
        turnOffLED(0);

    if (leds & led2_1)
        flashLED(1, 1000, 1000);

    if (leds & led2_2)
        flashLED(1, 2000, 2000);

    if (leds & (led2_1 & led2_2))
        turnOffLED(1);

    if (leds & led3)
        turnOnLED(2);
    else
        turnOffLED(2);

}


void warningAlarmTask(void * data) {

    struct WarningStruct* warningAlarmData = (struct WarningStruct *) data;

    if (doRun(warningAlarmData->interval, warningAlarmData->lastTimeRun) == FALSE)
        return;

    static int leds = 0;
    int new_leds = 0;

    unsigned short * batteryLevel = warningAlarmData->batteryLevel;
    unsigned short * fuelLevel = warningAlarmData->fuelLevel;

    if (*fuelLevel > 50 & *batteryLevel > 50) {

        new_leds &= ~(led1_1 | led1_2 | led2_1 | led2_2);
        new_leds |= led3;
        *warningAlarmData->batteryLow = FALSE;
        *warningAlarmData->fuelLow = FALSE;

    } else {

        if (*batteryLevel > 50) {
            new_leds &= ~(led1_1 | led1_2);
            *warningAlarmData->batteryLow = FALSE;
        } else if (*batteryLevel <= 50 & *batteryLevel > 10) {
            new_leds |= led1_2;
            new_leds &= ~led1_1;
            *warningAlarmData->batteryLow = FALSE;
        } else {
            new_leds &= ~led1_2;
            new_leds |= led1_1;
            *warningAlarmData->batteryLow = TRUE;
        }

        if (*fuelLevel > 50) {
            new_leds &= ~(led2_1 | led2_2);
            *warningAlarmData->fuelLow = FALSE;
        } else if (*fuelLevel <= 50 & *fuelLevel > 10) {
            new_leds |= led2_2;
            new_leds &= ~led2_1;
            *warningAlarmData->fuelLow = FALSE;
        } else {
            new_leds &= ~led2_2;
            new_leds |= led2_1;
            *warningAlarmData->fuelLow = TRUE;
        }

    }

    if (leds != new_leds){
        applyNewLEDs(new_leds);
        leds = new_leds;

    }

    warningAlarmData->lastTimeRun = now();

    return;

}


int main() {

    unsigned short BATTERY_LVL = 100;
    unsigned short FUEL_LVL = 100;
    Bool FUEL_LOW = FALSE;
    Bool BATTERY_LOW = FALSE;

    struct WarningStruct testData = {now() - 5., 0, &FUEL_LOW, &BATTERY_LOW, &FUEL_LVL, &BATTERY_LVL};

    turnAllOff();

    while (TRUE) {

        if (*testData.fuelLevel > 0)
            *testData.fuelLevel -= 2;

        if (*testData.batteryLevel > 0)
            *testData.batteryLevel -= 5;

        printf("\nFuel level: %d\n", *testData.fuelLevel);
        printf("Battery level: %d\n", *testData.batteryLevel);

        warningAlarmTask(&testData);
        usleep(1000000);

    }

}
