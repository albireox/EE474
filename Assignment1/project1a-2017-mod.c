//*****************************************************************************
//
// Josés modified script program.
//
// Built from hello.c - Simple hello world example.
//
// Copyright (c) 2006-2011 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 7243 of the EK-LM3S8962 Firmware Package.
//
//*****************************************************************************

#include <stdio.h>

#define DELAY_FACTOR 100000
#define TRUE 1
#define FALSE 0

// Delays for writing and clearing the line.
unsigned long write_delay = 1000;
unsigned long clear_delay = 100;

// Function prototypes
void delay(unsigned long aValue);
void fData(char* myPtr, unsigned long *delay);


//*****************************************************************************
//
// Print "9 8 7 6 5 4 3 2 1 0"
//
//*****************************************************************************

int main(void)
{

    // Define some local variables
    char space = ' ';

    // Print the digits 9 8 7 6 5 4 3 2 1 0
    // Then clear them.
    while(TRUE)
    {

        // Sends the NULL pointer to indicate that we want to print the
        // digit in the loop.
        fData(NULL, &write_delay);

        // Sends the space character.
        fData(&space, &clear_delay);

    }

}


//  software delay
void delay(unsigned long aValue)
{
    volatile unsigned long i = 0;  // Uses volatiles to prevent the compiler
    volatile unsigned int j = 0;   // to ignore this loop.

    for (i = aValue; i > 0; i--)
    {
        for (j = 0; j < DELAY_FACTOR; j++);
    }

    return;
}


void fData(char* myChar, unsigned long *myDelay)
{

    char myData[2];

    for (int i = 9; i >= 0; i--)
    {
        if (!myChar) {            // If myChar is the NULL pointer, writes i
            myData[0] = i + '0';
        } else {                  // Otherwise prints myChar.
            myData[0] = *myChar;
        }

        myData[1] = '\0';       // Adds a string terminator
        printf("%s ", myData);  // Prints the string
        fflush(stdout);         // Flushes the standard ouput now

        delay(*myDelay);        // Waits until the next iteration of the loop

    }

    printf("%c", 0x0d);           //  print a carridge return
    fflush(stdout);

}
