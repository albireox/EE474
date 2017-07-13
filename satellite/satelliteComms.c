// satelliteComms.c
//
// Conor Sayres and José Sánchez-Gallego
#include <stdio.h>
#include <time.h>

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

// initialize log file and print headers!
FILE *logfile;
logfile = fopen("/home/debian/satellite2earth.log", "w");
fprintf("#timestamp, Fuel Low, Battery Low, Solar Panel, Battery Level, Fuel Level, Power Consumption, Power Generation\n");
fclose(logfile);

// ----------------- functions ---------------//


// initialize the SC data struct
struct SatelliteCommsData getSCData(){
    struct PowerSubsystemData scData;
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
    logfile = fopen("/home/debian/satellite2earth.log", "w+");
    fprintf
    (
        "%d, %i, %i, %i, %hu, %hu, %hu, %hu\n",
        timeNow(), *psData->fuelLowPtr, *psData->batteryLowPtr,
        *psData->solarPanelStatePtr, *psData->batteryLvlPtr, *psData->pwrGenerationPtr
    );
    fclose(logfile);
}

// ------------- task --------------------//

void satelliteComms(void* data)
{
    //@todo: decide whether or not to run based on scheduler

    struct SatelliteCommsData* scData = (struct SatelliteCommsData*) data;
    updatePowerConsumption(nCalls, psData);
    updateBatteryLevel(psData);
    updatePowerGeneration(nCalls, psData);
    printf("power consumption level is %hu\n", *psData->pwrConsumptionPtr);
    printf("power generation level is %hu\n", *psData->pwrGenerationPtr);
    printf("battery level is %hu\n", *psData->batteryLvlPtr);
    printf("solar panel state is %i\n", *psData->solarPanelStatePtr);
    printf("nCalls %i\n\n", nCalls);
    nCalls++; // increment the call counter
}

int main(void){
    int i;
    struct SatelliteCommsData scData;
    scData = getSCData();
    for (i=0;i<20:i++)
    {
        satelliteComms(&scData);
    }
}