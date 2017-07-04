//*****************************************************************************
// project1a-2017-external-file.c
//
// Conor Sayres and José Sánchez-Gallego
//
// Modification of project1a-2017-single-function.c in which the previous functions
// delayedWrite and delay are broken out into a separate file: delayedWrite.c. As outlined in
// lab 1, heading: Building, Editing, and Running a Program point #11.  The user may specify
// the delay time(s) as a command line argument.
//
//
//*****************************************************************************

#include <stdio.h> // Provide access to standard C library i/o routines (eg printf, fflush...)
#include <stdlib.h> // Provide access to strtoul
// Define TRUE as 1, used in infinite while loop, aids readability
#define TRUE 1

// prototypes for the delay functions called in main.
void delay(unsigned long aValue);
void dataLoop(unsigned long *delay1, int showNumber);
extern void delayedWrite(unsigned long *delayValue, char *data);

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
    // don't allow more than 2 arguments to be passed to this program
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
        dataLoop(&delayValuePrint, 1); // write numbers
        dataLoop(&delayValueClear, 0); // clear numbers
    }

}
//*****************************************************************************
// Prints numbers or spaces to stdout with a delay between each print
// inputs: delay1 - an unsigned long, passed by pointer reference, a scaling
//                     factor for length of delay between printed numbers
//         showNumber - boolean flag.  If 1 show the number, else clear the number.
// calls: delayedWrite
//*****************************************************************************
void dataLoop(unsigned long *delayValue, int showNumber)
{
      int i = 0; // initialize loop vairable
      //  working C style string
      // this char array is re-populated
      // throughout the program with numbers
      // if showNumber == 1
      // initialize it as an empty space
      char myData[3] = " \0";
      // loop from 9 to 0
      for (i = 9; i >=0; i--)
      {
        if (1 == showNumber)
        {
            // populate myData with the integer
            myData[0] = i + '0';        //  convert the int i to ascii
            myData[1] = '\0';           //  terminate the string
        }

        delayedWrite(delayValue, myData); // write to the user
      }

      //  print a carridge return, bringing us back to the beginning of the output line
      printf("%c", 0x0d);
      fflush(stdout);
}
