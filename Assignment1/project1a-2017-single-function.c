//*****************************************************************************
// project1a-2017-single-function.c
//
// Conor Sayres and José Sánchez-Gallego
//
// Modification of project1a-2017-pointer-reference.c in which the previous functions
// f1Data and f2Clear are replaced with a single function dataLoop.
// dataLoop calls the delayedWrite function, which accepts a character to display
// and a delay value, as outlined in lab 1, heading:
// Building, Editing, and Running a Program point #10.  The user may specify
// the delay time(s) as a command line argument.
//
//*****************************************************************************

#include <stdio.h> // Provide access to standard C library i/o routines (eg printf, fflush...)
#include <stdlib.h> // Provide access to strtoul
// Define TRUE as 1, used in infinite while loop, aids readability
#define TRUE 1

// prototypes for the delay functions called in main.
void delay(unsigned long aValue);
void dataLoop(unsigned long *delay1, int doClear);
void delayedWrite(unsigned long *delayValue, char *data);

//*****************************************************************************
//
// The main function is called by the operating system when this program is run.
// This program runs forever until it is explicitly killed by the user.  It prints
// the numbers 0 - 9 in order on a single line, until all numbers are visible.
// Then removes the numbers in the same order in which they were printed until no
// numbers are visible, then the routine repeats.  A small delay is forced between
// prints so the user can see the ouptut.
//
// The program will permit a 2, 1, or 0 command line arguments specifying delay
// time(s), otherwise the default value of 1000 will be used for all delays.
// User should specify only unsigned integers at the command line!
//
// inputs: argc - int number of command line args supplied to the main function
//         argv - pointer array to args
//*****************************************************************************
int main(int argc, char *argv[])
{

    // initialize both delay values to 1000
    // this will be the default if not overridden by the user
    unsigned long delayValuePrint = 1000;
    unsigned long delayValueClear = 1000;
    // don't allow more than a single argument to be passed to this program
    if (argc > 3){
        printf("Too many arguments!  This program accepts 2, 1 or 0 arguments of type unsigned int. \n");
        printf("Argument 1 specifies delay in printing numbers. \n");
        printf("Argument 2 specifies delay in clearing numbers. \n");
        return 0;
    }
    else if (3 == argc){
        // convert the user supplied string to an unsigned long
        delayValuePrint = strtoul(argv[1], NULL, 10);
        delayValueClear = strtoul(argv[2], NULL, 10);
        printf("You supplied a print delay value of: %lu\n", delayValuePrint);
        printf("You supplied a clear delay value of: %lu\n", delayValueClear);
    }
    else if (2 == argc){
        // convert the user supplied string to an unsigned long
        delayValuePrint = strtoul(argv[1], NULL, 10);
        printf("You supplied a print delay value of: %lu\n", delayValuePrint);
        printf("Using default clear delay value of: %lu\n", delayValueClear);
    }
    else {
        printf("Using default print delay value of: %lu\n", delayValuePrint);
        printf("Using default clear delay value of: %lu\n", delayValueClear);
    }

    //  begin infinite loop
    while(TRUE)
    {
        dataLoop(&delayValuePrint, 0); // write numbers
        dataLoop(&delayValueClear, 1); // clear numbers
    }

}
//*****************************************************************************
// Definition of the delay function, a software delay.
// Implemented by a long nested loop that doesn't do
// any real work.
// inputs: an unsigned long
//      a scaling factor for length of delay
// returns: void
//*****************************************************************************
void delay(unsigned long aValue)
{
    // outer loop variable initialization
    // overwritten by the aValue input to the function
    volatile unsigned long i = 0;

    // inner loop variable initialization
    volatile unsigned int j = 0;

    for (i = aValue; i > 0; i--)
    {
        for (j = 0; j < 100000; j++);
    }

    return;
}
//*****************************************************************************
// Prints numbers or spaces to stdout with a delay between each print
// inputs: delay1 - an unsigned long, passed by pointer reference
//      a scaling factor for length of delay between printed numbers
//         doClear - boolean flag.  If 1 clear the number, else print the integer.
// calls: delayedWrite
//*****************************************************************************
void dataLoop(unsigned long *delayValue, int doClear)
{
      //  working C style string
      // this char array is re-populated
      // throughout the program with numbers
      char myData[3];
      // loop from 9 to 0
      for (int i = 9; i >=0; i--)
      {
        if (1 == doClear){
            myData[0] = ' ';
            myData[1] = '\0';
        }
        else
        {
            myData[0] = i + '0';        //  convert the int i to ascii
            myData[1] = '\0';           //  terminate the string
        }

        delayedWrite(delayValue, myData); // write to the user
      }

      //  print a carridge return, bringing us back to the beginning of the output line
      printf("%c", 0x0d);
      fflush(stdout);
}

//*****************************************************************************
// Write something to stdout with a delay
// inputs: delay - an unsigned long, passed by pointer reference
//         data - character(s) to be printed
//*****************************************************************************
void delayedWrite(unsigned long *delayValue, char *data)
{
        printf("%s ", data);  // add the space to the stdout buffer
        fflush(stdout);         // show the user
        delay(*delayValue);            // delay so we can read the display
}
