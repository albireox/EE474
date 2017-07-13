// satelliteComms.c
//
// Conor Sayres and José Sánchez-Gallego
#include <stdio.h>
#include <time.h>
#include "bool.h"

// Global Vars (defined in globalVars.c)
extern unsigned short BATTERY_LVL;
extern unsigned short FUEL_LVL;
extern unsigned short PWR_CONSUMPTION;
extern unsigned short PWR_GENERATION;
extern Bool SOLAR_PANEL_STATE;
extern Bool FUEL_LOW;
extern Bool BATTERY_LOW;

// data structure
struct SatelliteCommsData
{
    unsigned short* batteryLvlPtr;
    unsigned short* pwrConsumptionPtr;
    unsigned short* pwrGenerationPtr;
    unsigned short* fuelLvlPtr;
    Bool* solarPanelStatePtr;
    Bool* fuelLowPtr;
    Bool* batteryLowPtr;
};

// ----------------- functions ---------------//


// initialize the SC data struct
struct SatelliteCommsData getSCData(){
    struct SatelliteCommsData scData;
    scData.batteryLvlPtr = &BATTERY_LVL;
    scData.pwrConsumptionPtr = &PWR_CONSUMPTION;
    scData.pwrGenerationPtr = &PWR_GENERATION;
    scData.fuelLvlPtr = &FUEL_LVL;
    scData.solarPanelStatePtr = &SOLAR_PANEL_STATE;
    scData.fuelLowPtr = &FUEL_LOW;
    scData.batteryLowPtr = &BATTERY_LOW;
    return scData;
}

// return time now in seconds
double timeNow()
{
    return (double)clock()/(double)CLOCKS_PER_SEC;
}

// update the logfile with the current state of the system
// emulating communication to earth
void updateLog(struct SatelliteCommsData* scData)
{
    FILE *logfile;
    logfile = fopen("/home/debian/satellite2earth.log", "a+");
    fprintf
    (
        logfile,
        "%f, %i, %i, %i, %hu, %hu, %hu, %hu\n",
        timeNow(), *scData->fuelLowPtr, *scData->batteryLowPtr,
        *scData->solarPanelStatePtr, *scData->batteryLvlPtr, *scData->fuelLvlPtr,
        *scData->pwrConsumptionPtr, *scData->pwrGenerationPtr
    );
    fclose(logfile);
}

// ------------- task --------------------//

void satelliteComms(void* data)
{
    //@todo: decide whether or not to run based on scheduler

    struct SatelliteCommsData* scData = (struct SatelliteCommsData*) data;
    updateLog(scData);
    printf("power consumption level is %hu\n", *scData->pwrConsumptionPtr);
    printf("power generation level is %hu\n", *scData->pwrGenerationPtr);
    printf("battery level is %hu\n", *scData->batteryLvlPtr);
    printf("solar panel state is %i\n", *scData->solarPanelStatePtr);
}

int main(void){
    int i;
    struct SatelliteCommsData scData;
    // initialize log file and print headers!
    FILE *logfile;
    logfile = fopen("/home/debian/satellite2earth.log", "w");
    fprintf(logfile, "#timestamp, Fuel Low, Battery Low, Solar Panel, Battery Level, Fuel Level, Power Consumption, Power Generation\n");
    fclose(logfile);
    scData = getSCData();
    for (i=0;i<20;i++)
    {
        satelliteComms(&scData);
    }
}