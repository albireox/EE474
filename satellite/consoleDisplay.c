// consoleDisplay.c
//
// Conor Sayres and José Sánchez-Gallego

#include <ncurses.h>


void printWindow(unsigned int leds){

        WINDOW *win = newwin(10, 55, 1, 1);
        refresh();

        box(win, '|', '-');
        mvwprintw(win, 1, 3, "Extraterrestrial Resources, Ltd. Warnings panel.");
        mvwhline(win, 2, 1, '=', 53);
        mvwprintw(win, 3, 20, "WARNING CONSOLE");
        mvwhline(win, 4, 1, '-', 53);

        int ii;
        static int jj = 0;

        for (ii=1; ii <= 4; ii++) {
            mvwprintw(win, ii + 4, 2, "LED %d: %d", ii, jj);
        }

        touchwin(win);
        wrefresh(win);

}
