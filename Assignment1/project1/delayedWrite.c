//*****************************************************************************
// delayedWrite.c
//
// Conor Sayres and José Sánchez-Gallego
//
// Functions delay, and delayedWrite in an external file. To be compiled with
// project1a-2017-external-file.c
//
//*****************************************************************************

#include <stdio.h> // Provide access to standard C library i/o routines (eg printf, fflush...)

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
// Write something to stdout with a delay
// inputs: delay - an unsigned long, passed by pointer reference
//         data - character(s) to be printed
//*****************************************************************************
void delayedWrite(unsigned long *delayValue, char *data)
{
        printf("%s ", data);  // print the data to the user
        fflush(stdout);
        delay(*delayValue);   // delay so we can read the display
}
