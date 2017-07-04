//*****************************************************************************
// project1a-2017-annotated.c
//
// Conor Sayres and José Sánchez-Gallego
//
// Version of project1a-2017.c with each line annotated, as specified under
// Building, Editing, and Running a Program #3, in Assignment 1.  Unused/extraneous variables,
// definitions and assignments from the original program have been removed.
//
//*****************************************************************************

#include <stdio.h> // Provide access to standard C library i/o routines (eg printf, fflush...)

// Define TRUE as 1, used in infinite while loop, aids readability
#define TRUE 1

// prototype for the delay function, so the compiler doesn't complain, body defined below.
void delay(unsigned long aValue);

//*****************************************************************************
//
// The main function is called by the operating system when this program is run.
// This program runs forever until it is explicitly killed by the user.  It prints
// the numbers 0 - 9 in order on a single line, until all numbers are visible.
// Then removes the numbers in the same order in which they were printed until no
// numbers are visible, then the routine repeats.  A small delay is forced between
// prints so the user can see the ouptut.
//
//*****************************************************************************
int main(void)
{

    //  initialize i as 0, used as a loop variable
    volatile int i = 0;

    // Show the user the initial value of i (which is 0)
    printf("The value of i is: %d\n", i);
	fflush(stdout);  // fflush required to force output to user without a newline character.

    //  begin infinite loop
    while(TRUE)
    {
      //  working C style string
      // this char array is re-populated
      // throughout the program with either numbers
      // or spaces and shown to the user
      char myData[3];

      // loop from 9 to 0
      // this loop builds the output line
      // 0 1 2 3 ... with a delay between each
      // printed integer
      for (i = 9; i >=0; i--)
      {
        myData[0] = i + '0';        //  convert the int i to ascii
        myData[1] = '\0';           //  terminate the string
        printf("%s ", myData);      //  add the current value of i to the stdout buffer
		fflush(stdout);             //  flush the buffer so the value is displayed to the user

        delay(1000);                //  delay so we can read the display
      }

      //  print a carridge return, bringing us back to the beginning of the output line
      printf("%c", 0x0d);
	  fflush(stdout);
      //  populate myData with a space and terminate it
      myData[0] = ' ';
      myData[1] = '\0';

      //  begin the loop to clear the line
      // this overwrites each number that was printed
      // in previous loop above with a space character
      for (i = 0; i < 10; i++)
      {
        printf("%s ", myData);  // add the space to the stdout buffer
		fflush(stdout);         // show the user

        delay(1000);            // delay so we can read the display
      }

      //  print a carridge return, bringing us back to the beginning of the output line
      printf("%c", 0x0d);           //  print a carridge return
	  fflush(stdout);
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
