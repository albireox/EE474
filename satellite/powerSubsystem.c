// powerSubsystem.c
//
// Conor Sayres and José Sánchez-Gallego


#include <stdio.h>
#include "bool.h"
#include "satellite.h"

// Increment powerconsumption by 2 on even calls, decrement by 1 on odd
// calls if
void updatePowerConsumption(int nCalls, struct PowerSubsystemStruct* psData)
{
    // effective flag for determining if power consumption is rising or falling
    // begin with an increasing direction
    static int stepDirection = 1;
    int incrVal; // value to increment power consumption by (may be negative!)
    unsigned short currentPowerConsumption = *psData->pwrConsumptionPtr;
    unsigned short newPowerConsumption;
    if (0 == nCalls%2)
    {
        // even function call, use step size 2
        // in same direction as stepDirection
        incrVal = 2 * stepDirection;
    }
    else{
        // odd function call, use step size 1
        // in opposite direction as stepDirection
        incrVal = -1 * stepDirection;
    }
    // modify the shared variable
    newPowerConsumption = currentPowerConsumption + incrVal;
    *psData->pwrConsumptionPtr = newPowerConsumption;
    // if stepDirection is positive and pwr consumption is
    // greater than 10, or if stepDirection is negative and
    // pwr consumption is less than 5, reverst the stepDirection
    if ((1==stepDirection && newPowerConsumption > 10) || (-1==stepDirection && newPowerConsumption < 5))
    {
        stepDirection *= -1;
    }

}

// update the global power generation level
void updatePowerGeneration(int nCalls, struct PowerSubsystemStruct* psData)
{
    unsigned short currentPowerGeneration = *psData->pwrGenerationPtr;
    if(*psData->solarPanelStatePtr)
    {
        // solar panel deployed
        if(*psData->batteryLvlPtr>95)
        {
            // battery is greater than 95%, retract solar panel
            // issue command?
            *psData->solarPanelStatePtr = FALSE;
            // reset power generation to zero
            *psData->pwrGenerationPtr = 0;
        }
        else
        {
            if(*psData->batteryLvlPtr<50)
            {
                // if battery is less than 50 increment by 2 for even function calls, 1 otherwise
                if(nCalls%2==0)
                {
                    *psData->pwrGenerationPtr = currentPowerGeneration + 2;
                }
                else
                {
                    *psData->pwrGenerationPtr = currentPowerGeneration + 1;
                }
            }
            else if (nCalls%2==0)
            {
                // if battery is > 50, only increment by 2 on even function calls
                *psData->pwrGenerationPtr = currentPowerGeneration + 2;
            }
        }

    }
    else if (*psData->batteryLvlPtr<=10)
    {
        // solar panel not deployed and battery level is at 10% or lower
        // deploy the solar panel
        *psData->solarPanelStatePtr = TRUE;
    }
}

// update the global battery level
void updateBatteryLevelOrig(struct PowerSubsystemStruct* psData)
{
    int currentBatteryLvl = *psData->batteryLvlPtr;
    int newBatteryLvl; // use signed ints to catch negative values
    if(*psData->solarPanelStatePtr)
    {
        // solar panel deployed
        newBatteryLvl = currentBatteryLvl + *psData->pwrGenerationPtr - *psData->pwrConsumptionPtr;
    }
    else
    {
        // solar panel not deployed
       newBatteryLvl = currentBatteryLvl -3 * (*psData->pwrConsumptionPtr);
    }
    // confine battery to 0-100%!!!
    if (newBatteryLvl < 0)
    {
        newBatteryLvl = 0;
    }
    if (newBatteryLvl > 100)
    {
        newBatteryLvl = 100;
    }
    *psData->batteryLvlPtr = (unsigned short) newBatteryLvl; // recast to correct type
}

void updateBatteryLevel(struct PowerSubsystemStruct* psData)
{
    *psData->batteryLvlPtr = getBatteryLevel();
}

// powerSubsystem task
// argument: data to be cast to PowerSubsystemStruct type
// for access to globals

void powerSubsystemTask(void* data)
{
    //@todo: decide whether or not to run based on scheduler
    // set up a counter to track calls to this funtion
    // value is 0 at first call.
    static int nCalls = 0;
    struct PowerSubsystemStruct* psData = (struct PowerSubsystemStruct*) data;

    if (doRun(psData->interval, psData->lastTimeRun) == FALSE)
        return;

    updatePowerConsumption(nCalls, psData);
    updateBatteryLevel(psData);
    updatePowerGeneration(nCalls, psData);
    nCalls++; // increment the call counter
    psData->lastTimeRun = now();
}




