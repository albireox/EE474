// satelliteComms.c
//
// Conor Sayres and José Sánchez-Gallego
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "bool.h"

// Global Vars (defined in globalVars.c)
extern unsigned short BATTERY_LVL;
extern unsigned short FUEL_LVL;
extern unsigned short PWR_CONSUMPTION;
extern unsigned short PWR_GENERATION;
extern Bool SOLAR_PANEL_STATE;
extern Bool FUEL_LOW;
extern Bool BATTERY_LOW;
extern unsigned int THRUST_CMD;

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
    unsigned int* thrustCmdPtr;
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
    scData.thrustCmdPtr = &THRUST_CMD;
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

//read in a thrust command, set it in the data structure
void updateThrustCommand(struct SatelliteCommsData* scData)
{
    //look for a hex command in /home/debian/earth2satellite
    // if present read it, and set it on the scData struct
    unsigned int thrustCmd;
    static FILE *thrustCmdFile;
    // if the file exists, read it and delete it
    if( access ("/home/debian/earth2satellite", F_OK) != -1)
    {
        thrustCmdFile = fopen("/home/debian/earth2satellite", "r");
        fscanf(thrustCmdFile, "%i", &thrustCmd);
        printf("thrustcmd %i\n", thrustCmd);
        fclose(thrustCmdFile);
        // remove the file now we have read it
        remove("/home/debian/earth2satellite");
        // set the command on the data struct
        *scData->thrustCmdPtr = thrustCmd;
    }


}

// ------------- task --------------------//

void satelliteComms(void* data)
{
    //@todo: decide whether or not to run based on scheduler

    struct SatelliteCommsData* scData = (struct SatelliteCommsData*) data;
    updateLog(scData);
    updateThrustCommand(scData);
    printf("power consumption level is %hu\n", *scData->pwrConsumptionPtr);
    printf("power generation level is %hu\n", *scData->pwrGenerationPtr);
    printf("battery level is %hu\n", *scData->batteryLvlPtr);
    printf("solar panel state is %i\n", *scData->solarPanelStatePtr);
    printf("thrustCmd is %i\n", *scData->thrustCmdPtr);
}

int main(void){
    int i;
    double t1, t2;
    struct SatelliteCommsData scData;
    // initialize log file and print headers!
    FILE *logfile;
    logfile = fopen("/home/debian/satellite2earth.log", "w");
    fprintf(logfile, "#timestamp, Fuel Low, Battery Low, Solar Panel, Battery Level, Fuel Level, Power Consumption, Power Generation\n");
    fclose(logfile);
    scData = getSCData();
    for (i=0;i<20;i++)
    {
        if(5==i){
            *(scData.fuelLowPtr) = TRUE;
        }
        satelliteComms(&scData);
        t1 = timeNow();
        t2 = timeNow();
        while(t2-t1<0.2)
        {
            // force a time delay
            t2=timeNow();
        }
    }
}