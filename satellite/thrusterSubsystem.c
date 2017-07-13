// thrusterSubsystem.c
//
// Conor Sayres and José Sánchez-Gallego
#include <stdio.h>
#include <time.h>
#include <unistd.h>

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


// gpio ports:
// down = 66, up = 67, right = 68, left = 69
#define GPIO_DOWN 66
#define GPIO_UP 67
#define GPIO_RIGHT 68
#define GPIO_LEFT 69
// gpio initial states:

// gpio state flags for each thruster
// for on vs off, initialize all to off
static int gpioFlagDown = 0;
static int gpioFlagUp = 0;
static int gpioFlagLeft = 0;
static int gpioFlagRight = 0;

// Global Vars (defined in globalVars.c)
extern unsigned short FUEL_LVL;
extern unsigned int THRUST_CMD;

// -------- structures ---------------- //

// data structure
struct ThrusterSubsystemData
{
    unsigned short* fuelLvlPtr;
    unsigned int* thrustCmdPtr;
};


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

// -------- helper functions ---------------- //

// return time now in seconds
double timeNow()
{
    return (double)clock()/(double)CLOCKS_PER_SEC;
}

// initialize the TS data struct
struct ThrusterSubsystemData getTSData(){
    struct ThrusterSubsystemData tsData;
    tsData.fuelLvlPtr = &FUEL_LVL;
    tsData.thrustCmdPtr = &THRUST_CMD;
    return tsData;
};

// decode a thrust command
// inputs: thrustCommandBits
//              bits 3-0 correspond to which thruster(s)
//              are commanded, left, right, up, down respectively
//              bits 7-3 correspond to magnitude (as a percentage)
//              bits 15-8 correspond to a duration in seconds (0-255)
//              a thrust command == 0 is assumed to be a null command
//          thrusterCommand: pointer to a ThrusterCommand instance (to be populated)
// returns: void
// modifies: thrusterCommand struct

void decodeThrusterCommand(unsigned thrustCommandBits, struct ThrusterCommand* thrusterCommand)
{
    // @todo, if thrustCommandBits is 0 raise error?
    // set tStart to now!
    thrusterCommand->tStart = timeNow();
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
    return timeNow() - thrusterCommand->tStart;
}

// return 1 if the command is still active
// else return 0.  A command is active if
// tStart + duration > timeNow()
int isActive(struct ThrusterCommand* thrusterCommand)
{
    return elapsedSeconds(thrusterCommand) < thrusterCommand->duration;
}

// set gpio
// but only set it if it is
// different than the last time it was set!
// eg, don't keep writing a high value if it's
// already high
void toggleGPIO()
{
    static FILE *fp;
    // local vars for dectecting if the value
    // has changed.  initialize to -1
    // so that on first call they get set
    // to initial values
    static int lastGpioFlagDown = -1;
    static int lastGpioFlagUp = -1;
    static int lastGpioFlagLeft = -1;
    static int lastGpioFlagRight = -1;
    // down gpio
    if (lastGpioFlagDown != gpioFlagDown)
    {
        printf("setting down flag to %i\n", gpioFlagDown);
        lastGpioFlagDown = gpioFlagDown;
        fp = fopen("/sys/class/gpio/gpio66/value", "w");
        if (1 == gpioFlagDown)
        {
            fprintf(fp, "1");
        }
        else
        {
            fprintf(fp, "0");
        }
        fclose(fp);
    }
    // up gpio
    if (lastGpioFlagUp != gpioFlagUp)
    {
        lastGpioFlagUp = gpioFlagUp;
        fp = fopen("/sys/class/gpio/gpio67/value", "w");
        if (1 == gpioFlagUp)
        {
            fprintf(fp, "1");
        }
        else
        {
            fprintf(fp, "0");
        }
        fclose(fp);
    }
    // right gpio
    if (lastGpioFlagRight != gpioFlagRight)
    {
        lastGpioFlagRight = gpioFlagRight;
        fp = fopen("/sys/class/gpio/gpio68/value", "w");
        if (1 == gpioFlagRight)
        {
            fprintf(fp, "1");
        }
        else
        {
            fprintf(fp, "0");
        }
        fclose(fp);
    }
    // left gpio
    if (lastGpioFlagLeft != gpioFlagLeft)
    {
        lastGpioFlagLeft = gpioFlagLeft;
        fp = fopen("/sys/class/gpio/gpio69/value", "w");
        if (1 == gpioFlagLeft)
        {
            fprintf(fp, "1");
        }
        else
        {
            fprintf(fp, "0");
        }
        fclose(fp);
    }
    return;
}

//update the fuel level
//inputs:
//    impulseOn: 1 for thruster is on, 0 for thruster is off
//    tsData: pointer to thruster subsystem data struct, this function
//            modifies the fuel level (an attribute of tsData structure)
//discuss error in fuel level approximation?
void updateFuelLevel(int impulseOn, struct ThrusterSubsystemData* tsData)
{

    // rate of fuel consumption, empty after 6 months @ 5% fuel use.
    // 6 months = 15770000 seconds.
    // so rate at 100% fuel consumption ammounts to:
    // (100/15770000)*20 (percent/sec) = 0.00012682308180088776 (percent/sec)
    static double fuelRate = 0.00012682308180088776; // rate at 100% thrust magnitude
    static double tNow, elapsedSeconds, fuelUsed;
    // determine elapsed time between fuel level updates.
    // set only once (should be 100%), double for non-integer math
    static double fuelLevel = (double)(*tsData->fuelLevelPtr);
    static int lastImpulseOn = -1; // -1 for uninitialized
    static double tLast = -1; // -1 for uninitialized
    tNow = timeNow();
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
            *tsData->fuelLevelPtr = (unsigned short)fuelLevel;
            printf("fuel double: %f\n", fuelLevel);
            printf("fuel short: %hu\n", *tsData->fuelLevelPtr);
        }
        // uninitialized, first time we've been called
        // set things up for the next call
        lastImpulseOn = impulseOn;
        tLast = tNow;
    }
}

// ------------------- task ----------------- //

void thrusterSubsystem(void* data)
{
    //@todo: decide whether or not to run based on scheduler

    // cast input to correct type
    struct ThrusterSubsystemData* tsData = (struct ThrusterSubsystemData*) data;

    // created once, persists, update this struct as new commands come in
    static struct ThrusterCommand thrusterCommand;

    // @todo, use an array of pointers for gpios?

    double tElapsed; // time elapsed for the active command
    double fractionalSec; //fraction of second elapsed
    int impulseOn; // 0 or 1.  If impluse on, set gpio on, else off
    // check if a new command is present.
    // if so overwrite the previous command
    // even if it has not finished!!!!
    if(*tsData->thrustCmdPtr){
        printf("got new thruster command\n");
        decodeThrusterCommand(*tsData->thrustCmdPtr, &thrusterCommand);
        // Null the pointer (to be populated by sattelite comms)
        *tsData->thrustCmdPtr =  0;
        printf("decoded thruster command:\n");
        printf("-----------------------\n");
        printf("thrust down %i\n", thrusterCommand.downOn);
        printf("thrust up %i\n", thrusterCommand.upOn);
        printf("thrust left %i\n", thrusterCommand.leftOn);
        printf("thrust right %i\n", thrusterCommand.rightOn);
        printf("thrust mag %f\n", thrusterCommand.magnitude);
        printf("thrust duration %i\n", thrusterCommand.duration);
        printf("-----------------------\n");
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
        // printf("current cmd is active!\n");
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
    // printf("time, gpios: %f %i %i %i %i\n", tElapsed, gpioFlagDown, gpioFlagUp, gpioFlagLeft, gpioFlagRight);
}

// int main(void)
// {
//     FILE *fp;
//     fp = fopen("/sys/class/gpio/gpio64/value", "w");
//     fprintf(fp, "1");
//     fclose(fp);

//     fp = fopen("/sys/class/gpio/gpio64/value", "r");

// }

int main(void)
{
    toggleGPIO(); // initialize all gpio
    unsigned tCmd;
    int intTCmd;
    // tCmd = 0x111; // 1sec 0.0625 mag, down
    tCmd = 0x383; //0.5 mag, down up, 3sec
    intTCmd = (int)tCmd;
    THRUST_CMD = intTCmd;
    struct ThrusterSubsystemData tsData = getTSData();
    thrusterSubsystem(&tsData);
    double t1, t2;
    t1 = timeNow();
    t2 = timeNow();
    thrusterSubsystem(&tsData);
    while(t2-t1<3.01)
    {
        t2=timeNow();
        thrusterSubsystem(&tsData);
    }

    // tCmd = 0x113; // 1sec 0.625 mag, down, up
    // intTCmd = (int)tCmd;
    // THRUST_CMD = intTCmd;
    // thrusterSubsystem(&tsData);
    // t1 = timeNow();
    // t2 = timeNow();
    // thrusterSubsystem(&tsData);
    // while(t2-t1<1.01)
    // {
    //     t2=timeNow();
    //     thrusterSubsystem(&tsData);
    // }
}


// int main(void){
//     double g = 1.95;
//     int v = (int)g;
//     double frac = g - v;
//     printf("g: %f\n",g);
//     printf("v: %i\n",v);
//     printf("frac: %f\n", frac);
// }

// int main(void){
//     unsigned thrustMag = 0x61;
//     unsigned maxThrust = 0xf;
//     int thrustExtracted = (int)((thrustMag>>4)&maxThrust);

//     printf("hello %i\n", thrustExtracted);
//     printf("fraction of max: %f\n", (double)thrustExtracted/16.0);

// }

// int main(void){
//     unsigned bitSelect = 0x1;
//     unsigned testBit1;
//     int i;
//     testBit1 = 0x1; // bit 0 set
//     for(i=0; i<5; i++)
//     {
//         if(testBit1 & (bitSelect << i)){
//             printf("bit %i is set\n", i);
//         }
//     }
//     printf("----------\n");
//     testBit1 = 0x2; // bit 1 set
//     for(i=0; i<5; i++)
//     {
//         if(testBit1 & (bitSelect << i)){
//             printf("bit %i is set\n", i);
//         }
//     }
//     printf("----------\n");
//     testBit1 = 0x3; // bit 0 and 1 set
//     for(i=0; i<5; i++)
//     {
//         if(testBit1 & (bitSelect << i)){
//             printf("bit %i is set\n", i);
//         }
//     }
//     printf("----------\n");
//     testBit1 = 0x4; // bit 2 set
//     for(i=0; i<5; i++)
//     {
//         if(testBit1 & (bitSelect << i)){
//             printf("bit %i is set\n", i);
//         }
//     }
//     printf("----------\n");
//     testBit1 = 0x5; // bit 2 and 0 set
//     for(i=0; i<5; i++)
//     {
//         if(testBit1 & (bitSelect << i)){
//             printf("bit %i is set\n", i);
//         }
//     }
//     printf("----------\n");
//     testBit1 = 0x6; // bit 2,1 set
//     for(i=0; i<5; i++)
//     {
//         if(testBit1 & (bitSelect << i)){
//             printf("bit %i is set\n", i);
//         }
//     }
//     printf("----------\n");
//     testBit1 = 0x7; // bit 2,1,0 set
//     for(i=0; i<5; i++)
//     {
//         if(testBit1 & (bitSelect << i)){
//             printf("bit %i is set\n", i);
//         }
//     }
//     printf("----------\n");
//     testBit1 = 0x8; // bit 3 set
//     for(i=0; i<5; i++)
//     {
//         if(testBit1 & (bitSelect << i)){
//             printf("bit %i is set\n", i);
//         }
//     }
//     printf("----------\n");

// }

// int main(void){
//     double t1, t2;
//     printf("clocks per sec %d\n", CLOCKS_PER_SEC);
//     t1 = (double)clock()/CLOCKS_PER_SEC;
//     t2 = (double)clock()/CLOCKS_PER_SEC;
//     while(t2-t1<1)
//     {
//         t2 = (double)clock()/CLOCKS_PER_SEC;
//     }
//     printf("t1 %f\n", t1);
//     printf("t2 %f\n", t2);
//     printf("elapsed %f\n", t2-t1);
// }