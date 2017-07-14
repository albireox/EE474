// consoleDisplay.c
//
// Conor Sayres and José Sánchez-Gallego

#include "bool.h"
#include "satellite.h"

#include <ncurses.h>


void refreshWindow(struct ConsoleStruct * data) {

        WINDOW *win = newwin(19, 55, 1, 1);
        refresh();

        box(win, '|', '-');
        mvwprintw(win, 1, 3, "Extraterrestrial Resources, Ltd. Warnings panel.");
        mvwhline(win, 2, 1, '=', 53);
        mvwprintw(win, 3, 20, "STATUS CONSOLE");
        mvwhline(win, 4, 1, '-', 53);

        mvwprintw(win, 6, 2, "Solar panel state: %s",
                  (*data->solarPanelState == FALSE) ? "retracted" : "DEPLOYED");
        mvwprintw(win, 7, 2, "Battery level: %d", *data->batteryLevel);
        mvwprintw(win, 8, 2, "Fuel level: %d", *data->fuelLevel);
        mvwprintw(win, 9, 2, "Power consumption: %d", *data->powerConsumption);

        mvwhline(win, 11, 1, '=', 53);
        mvwprintw(win, 12, 18, "ANNUNCIATION CONSOLE");
        mvwhline(win, 13, 1, '-', 53);

        mvwprintw(win, 15, 2, "Fuel low: %s",
                  (*data->fuelLow == FALSE) ? "false" : "TRUE");
        mvwprintw(win, 16, 2, "Battery low: %s",
                  (*data->batteryLow == FALSE) ? "false" : "TRUE");

        touchwin(win);
        wrefresh(win);

}


void consoleTask(void * data) {

    struct ConsoleStruct* consoleData = (struct ConsoleStruct *) data;

    if (doRun(consoleData->interval, consoleData->lastTimeRun) == FALSE)
        return;

    refreshWindow(data);

}
