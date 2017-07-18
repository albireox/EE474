// consoleDisplay.c
//
// Conor Sayres and José Sánchez-Gallego

#include "bool.h"
#include "satellite.h"

#include <ncurses.h>


void refreshWindow(struct ConsoleStruct * data) {
    // Uses ncurses to print a box with the status and annunciation consoles.

    WINDOW *win = newwin(19, 55, 1, 1);  // Creates the window
    refresh();

    box(win, '|', '-');  // Adds a box

    // Prints the header of the status console
    mvwprintw(win, 1, 3, "Extraterrestrial Resources, Ltd. Warnings panel.");
    mvwhline(win, 2, 1, '=', 53);
    mvwprintw(win, 3, 20, "STATUS CONSOLE");
    mvwhline(win, 4, 1, '-', 53);

    // Status console values
    mvwprintw(win, 6, 2, "Solar panel state: %s",
              (*data->solarPanelState == FALSE) ? "retracted" : "DEPLOYED");
    mvwprintw(win, 7, 2, "Battery level: %d", *data->batteryLevel);
    mvwprintw(win, 8, 2, "Fuel level: %d", *data->fuelLevel);
    mvwprintw(win, 9, 2, "Power consumption: %d", *data->powerConsumption);

    // Header of the annunciation console
    mvwhline(win, 11, 1, '=', 53);
    mvwprintw(win, 12, 18, "ANNUNCIATION CONSOLE");
    mvwhline(win, 13, 1, '-', 53);

    // Annunciation console values
    mvwprintw(win, 15, 2, "Fuel low: %s",
              (*data->fuelLow == FALSE) ? "false" : "TRUE");
    mvwprintw(win, 16, 2, "Battery low: %s",
              (*data->batteryLow == FALSE) ? "false" : "TRUE");

    // Flushes the screen.
    touchwin(win);
    wrefresh(win);

}


void consoleTask(void * data) {
    // Console task. Updates a terminal screen with the global variable values.

    struct ConsoleStruct* consoleData = (struct ConsoleStruct *) data;

    // Checks whether it is time to run again.
    if (doRun(consoleData->interval, consoleData->lastTimeRun) == FALSE)
        return;

    // If it is time to run, refreshes the screen.
    refreshWindow(data);

}
