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
#define MIN_PULSE_DURATION 0.0625;  // seconds

// Global Vars (defined in globalVars.c)
extern unsigned short FUEL_LVL;
extern unsigned int THRUST_CMD;

// return time now in seconds
double timeNow()
{
    return (double)clock()/(double)CLOCKS_PER_SEC;
}

// data structure
struct ThrusterSubsystemData
{
    unsigned short* fuelLvlPtr;
    unsigned int* thrustCmdPtr;
};

// initialize the TS data struct
struct ThrusterSubsystemData getTSData(){
    struct ThrusterSubsystemData tsData;
    tsData.fuelLvlPtr = &FUEL_LVL;
    tsData.thrustCmdPtr = &THRUST_CMD;
    return tsData;
};

// structure for holding a decoded thrust command
struct ThrusterCommand
{
    double tStart;    // time command was started in seconds
    double magnitude; // fraction of maximum thrust
    int duration;     // duration of thrust in seconds
    int leftOn;      // left thruster commanded, 1 or 0
    int rightOn;     // right thruster commanded, 1 or 0
    int upOn;        // up thruster commanded, 1 or 0
    int downOn;      // down thruster commanded, 1 or 0

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
    //integer value for bits 7-4
    int magnitudeExtracted = (int)((thrustCommandBits>>4)&magnitudeMask);
    // express it as a ratio of magnitudeMax and save it
    thrusterCommand->magnitude = (double)(magnitudeExtracted)/(double)(magnitudeMax);

    // decode duration, express it as a fraction of 1, bits 7-3
    // shift thrustCommandBits right 8 bits
    static unsigned durationMask = 0xff; // = 1111 1111
    //integer value for bits 15-8
    thrusterCommand->duration = (int)((thrustCommandBits>>8)&durationMask);
}

// return 1 if the command is still active
// else return 0.  A command is active if
// tStart + duration < timeNow()
int isActive(struct ThrusterCommand* thrusterCommand)
{
    return (thrusterCommand->tStart + thrusterCommand->duration) < timeNow();
}


void thrusterSubsystem(void* data)
{
    //@todo: decide whether or not to run based on scheduler
    static struct ThrusterCommand thrusterCommand; // created once, persists
    struct ThrusterSubsystemData* tsData = (struct ThrusterSubsystemData*) data;
    // check if a new command is present.
    // if so overwrite the previous command
    // even if it has not finished!!!!
    if(*tsData->thrustCmdPtr){
        printf("got thruster command\n");
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
    else
    {
        printf("no new thruster command\n");
    }
    // check whether the current command is still active
    // (has it run to its final duration?)
    printf("command active: %i\n", isActive(&thrusterCommand));

}




int main(void)
{
    unsigned tCmd;
    int intTCmd;
    tCmd = 0x111; // 1sec 0.625 mag, down
    intTCmd = (int)tCmd;
    THRUST_CMD = intTCmd;
    struct ThrusterSubsystemData tsData = getTSData();
    thrusterSubsystem(&tsData);

    thrusterSubsystem(&tsData);

    tCmd = 0x113; // 1sec 0.625 mag, down, up
    intTCmd = (int)tCmd;
    THRUST_CMD = intTCmd;
    thrusterSubsystem(&tsData);
}

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