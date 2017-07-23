CC = gcc
#CFLAGS = -Wall -Wextra -O2
CFLAGS = -g

DEPS = -lrt -lcurses

.PHONY: clean all build

ODIR = build

build: $(ODIR)/satellite

mkdir:
	mkdir -p $(ODIR)

clean:
	rm -Rf $(ODIR)

$(ODIR)/satellite: consoleDisplay.c powerSubsystem.c satelliteComms.c thrusterSubsystem.c \
	               vehicleComms.c consoleKeypad.c solarPanel.c time.c leds.c gpio.c \
	               warningAlarm.c scheduler.c | mkdir
	$(CC) -o $@ $^ $(CFLAGS) $(DEPS)
