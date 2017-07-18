// satelliteComms.c
//
// Conor Sayres and José Sánchez-Gallego
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "bool.h"
#include "satellite.h"

// update the logfile with the current state of the system
// emulating communication to earth
void updateLog(struct SatelliteCommsStruct* scData)
{
    FILE *logfile;
    logfile = fopen("/home/debian/satellite2earth.log", "a+");
    fprintf
    (
        logfile,
        "%f, %i, %i, %i, %hu, %hu, %hu, %hu\n",
        now(), *scData->fuelLowPtr, *scData->batteryLowPtr,
        *scData->solarPanelStatePtr, *scData->batteryLvlPtr, *scData->fuelLvlPtr,
        *scData->pwrConsumptionPtr, *scData->pwrGenerationPtr
    );
    fclose(logfile);
}

//read in a thrust command, set it in the data structure
void updateThrustCommand(struct SatelliteCommsStruct* scData)
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
        fclose(thrustCmdFile);
        // remove the file now we have read it
        remove("/home/debian/earth2satellite");
        // set the command on the data struct
        *scData->thrustCmdPtr = thrustCmd;
    }


}

// ------------- task --------------------//

void satelliteCommsTask(void* data)
{

    struct SatelliteCommsStruct* scData = (struct SatelliteCommsStruct*) data;

    if (doRun(scData->interval, scData->lastTimeRun) == FALSE)
        return;

    updateLog(scData);
    updateThrustCommand(scData);
    scData->lastTimeRun = now();
}

