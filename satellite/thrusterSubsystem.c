// thrusterSubsystem.c
//
// Conor Sayres and José Sánchez-Gallego
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "satellite.h"
#include "gpio.h"

// @todo keep track of timing errors for thrust duration(s)?

// Defining a minimum thruster pulse duration:
// The thruster command specifies a duration and magnitude of a thrust.
// The thruster is commanded in a pulse-like fashion (full on or full off).
// Scaling of the thrust magnitude must then defined as the fraction of time the
// thruster is on over the total duration of the commanded thrust.
// The smallest allowed thrust duration is 1 second.
// The magnitude is specified by 4 bits, ranging from 0000-1111.
// From this the smallest percentage of max thrust allowed is 6.25%.
// This defines the minimum thrust duration as 0.0625 seconds,
// to ensure coorect pulse toggling this task should be rentered every
// 0.01 seconds
// #define MIN_PULSE_DURATION 0.0625;  // seconds

// gpio state flags for each thruster
// for on vs off, initialize all to off
static int gpioFlagDown = 0;
static int gpioFlagUp = 0;
static int gpioFlagLeft = 0;
static int gpioFlagRight = 0;


// structure for holding a decoded thrust command
struct ThrusterCommand
{
    double tStart;          // time command was started in seconds
    double magnitude;       // fraction of maximum thrust
    int duration;           // duration of thrust in seconds
    int leftOn;             // left thruster commanded, 1 or 0
    int rightOn;            // right thruster commanded, 1 or 0
    int upOn;               // up thruster commanded, 1 or 0
    int downOn;             // down thruster commanded, 1 or 0

};


// parse thrustCommandBits
// populate the ThrusterCommand
void decodeThrusterCommand(unsigned thrustCommandBits, struct ThrusterCommand* thrusterCommand)
{
    // @todo, if thrustCommandBits is 0 raise error?
    // set tStart to now!
    thrusterCommand->tStart = now();
    // test individually for bits 0-3
    // (https://class.ee.washington.edu/474/peckol/code/C/bits1.c)
    static unsigned bitSelect = 0x01;
    thrusterCommand->downOn = (thrustCommandBits & bitSelect) != 0;      //0th bit on?
    thrusterCommand->upOn = (thrustCommandBits & bitSelect<<1) != 0;     //1st bit on?
    thrusterCommand->rightOn = (thrustCommandBits & bitSelect<<2) != 0;  //2nd bit on?
    thrusterCommand->leftOn = (thrustCommandBits & bitSelect<<3) != 0;   //3rd bit on?

    // @todo, write a function for extracting integers from bit blocks?
    // decode magnitude, express it as a fraction of 1, bits 7-3
    // shift thrustCommandBits right 4 bits
    static unsigned magnitudeMask = 0xf; // = 1111
    static int magnitudeMax = 16; // 1111 = 16
    //integer value for bits 7-4, get there by shifting by 4
    int magnitudeExtracted = (int)((thrustCommandBits>>4)&magnitudeMask);
    // express it as a ratio of magnitudeMax and save it
    thrusterCommand->magnitude = (double)(magnitudeExtracted)/(double)(magnitudeMax);

    // decode duration, bits 7-3, express as integer seconds
    // shift thrustCommandBits right 8 bits to get the final 8 (of 16)
    static unsigned durationMask = 0xff; // = 1111 1111
    //integer value for bits 15-8
    thrusterCommand->duration = (int)((thrustCommandBits>>8)&durationMask);
}

// elapsed seconds for an a thruster command
double elapsedSeconds(struct ThrusterCommand* thrusterCommand)
{
    return now() - thrusterCommand->tStart;
}

// return 1 if the command is still active
// else return 0.  A command is active if
// tStart + duration > now()
int isActive(struct ThrusterCommand* thrusterCommand)
{
    return elapsedSeconds(thrusterCommand) < thrusterCommand->duration;
}

// set gpio
// note, gpio functions will only set a
// new value if it was different then
// the previous
void toggleGPIO()
{
    setThrustDown(gpioFlagDown);
    setThrustUp(gpioFlagUp);
    setThrustLeft(gpioFlagLeft);
    setThrustRight(gpioFlagRight);
}

//update the fuel level
//inputs:
//    impulseOn: 1 for thruster is on, 0 for thruster is off
//    tsData: pointer to thruster subsystem data struct, this function
//            modifies the fuel level (an attribute of tsData structure)
//discuss error in fuel level approximation?
void updateFuelLevel(int impulseOn, struct ThrusterSubsystemStruct* tsData)
{

    // rate of fuel consumption, empty after 6 months @ 5% fuel use.
    // 6 months = 15770000 seconds.
    // so rate at 100% fuel consumption ammounts to:
    // (100/15770000)*20 (percent/sec) = 0.00012682308180088776 (percent/sec)
    static double fuelRate = 0.00012682308180088776; // rate at 100% thrust magnitude
    static double tNow, elapsedSeconds, fuelUsed;
    // determine elapsed time between fuel level updates.
    // set only once (should be 100%), double for non-integer math
    static double fuelLevel = 100; // (double)*tsData->fuelLvlPtr;
    static int lastImpulseOn = -1; // -1 for uninitialized
    static double tLast = -1; // -1 for uninitialized
    tNow = now();
    if(lastImpulseOn < 0)
    {
        // uninitialized, first time we've been called
        // set things up for the next call
        lastImpulseOn = impulseOn;
        tLast = tNow;
    }
    else
    {
        // if impulse is on, and the last impulse was on
        // we've gone a full period of thruster use
        // determine the time elapsed and update the fuel level
        if(impulseOn && lastImpulseOn)
        {
            elapsedSeconds = tNow - tLast;
            // so fuel used since the last fuel level update is
            fuelUsed = elapsedSeconds*fuelRate;
            fuelLevel -= fuelUsed;
            // cast the new level to an unsigned short and update
            *tsData->fuelLvlPtr = (unsigned short)fuelLevel;
        }
        lastImpulseOn = impulseOn;
        tLast = tNow;
    }
}

// ------------------- task ----------------- //

void thrusterSubsystemTask(void* data)
{
    // cast input to correct type
    struct ThrusterSubsystemStruct* tsData = (struct ThrusterSubsystemStruct*) data;
    // created once, persists, update this struct as new commands come in
    static struct ThrusterCommand thrusterCommand;

    if (doRun(tsData->interval, tsData->lastTimeRun) == FALSE)
        return;

    // @todo, use an array of pointers for gpios?

    double tElapsed; // time elapsed for the active command
    double fractionalSec; //fraction of second elapsed
    int impulseOn; // 0 or 1.  If impluse on, set gpio on, else off
    // check if a new command is present.
    // if so overwrite the previous command
    // even if it has not finished!!!!
    if(*tsData->thrustCmdPtr){
        // printf("got new thruster command\n");
        decodeThrusterCommand(*tsData->thrustCmdPtr, &thrusterCommand);
        // Null the pointer (to be populated by sattelite comms)
        *tsData->thrustCmdPtr =  0;
    }
    // check whether the current command is still active
    if(!isActive(&thrusterCommand))
    {
        // there is no command active,
        // ensure all gpio to 0
        gpioFlagDown = 0;
        gpioFlagUp = 0;
        gpioFlagRight = 0;
        gpioFlagLeft = 0;
        impulseOn = 0;
    }
    else
    {
        // a thruster command is active, check timers and
        // update thruster signals
        // impluse period is 1 second decide how to toggle
        // gpio based on time that has elapsed
        tElapsed = elapsedSeconds(&thrusterCommand);
        // determine fractional second
        fractionalSec = tElapsed - (int)tElapsed;
        // determine whether or not an impluse is wanted
        impulseOn = fractionalSec < thrusterCommand.magnitude; // magnitude is fraction of second!
        // look at which thrusters should be active/inactive, update gpio
        // @todo: loop over these instead?
        if(thrusterCommand.downOn)
        {
            gpioFlagDown = impulseOn;
        }
        if(thrusterCommand.upOn)
        {
            gpioFlagUp = impulseOn;
        }
        if(thrusterCommand.rightOn)
        {
            gpioFlagRight = impulseOn;
        }
        if(thrusterCommand.leftOn)
        {
            gpioFlagLeft = impulseOn;
        }
    }
    toggleGPIO(); // will only toggle if bit is flipped!
    updateFuelLevel(impulseOn, tsData);
    tsData->lastTimeRun = now();
}

