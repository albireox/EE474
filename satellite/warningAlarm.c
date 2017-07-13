// warningAlarm.c
//
// Conor Sayres and José Sánchez-Gallego

#include <stdio.h>
#include <time.h>

#include "bool.h"
#include "satellite.h"


void warningAlarmTask(void * data) {

    struct WarningStruct* warningAlarmData = (struct WarningStruct *) data;

    long long lastTimeRun;

    if (warningAlarmData->lastTimeRun != 0)
        lastTimeRun = (long long) warningAlarmData->lastTimeRun;
    else
        lastTimeRun = 0;

    printf("Last time run: %lld\n"
           "Battery level: %hu\n"
           "Fuel level: %hu\n"
           "Battery low: %hu\n"
           "Fuel low: %hu\n\n", lastTimeRun,
                              (unsigned short) *warningAlarmData->batteryLevel,
                              (unsigned short) *warningAlarmData->fuelLevel,
                              (unsigned short) *warningAlarmData->batteryLow,
                              (unsigned short) *warningAlarmData->fuelLow);

    warningAlarmData->lastTimeRun = time(NULL);

}


void warnings_main(){

    time_t now = time(NULL);

    unsigned short BATTERY_LVL = 100;
    unsigned short FUEL_LVL = 100;
    Bool FUEL_LOW = FALSE;
    Bool BATTERY_LOW = FALSE;

    struct WarningStruct testData = {now, &FUEL_LOW, &BATTERY_LOW, &FUEL_LVL, &BATTERY_LVL};

    warningAlarmTask(&testData);

}
