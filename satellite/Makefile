CC = gcc
#CFLAGS = -Wall -Wextra -O2

DEPS = -lrt -lcurses

.PHONY: clean all build

ODIR = build

build: $(ODIR)/satellite

mkdir:
	mkdir -p $(ODIR)

clean:
	rm -Rf $(ODIR)

$(ODIR)/satellite: consoleDisplay.c powerSubsystem.c satelliteComms.c thrusterSubsystem.c time.c leds.c warningAlarm.c scheduler.c | mkdir
	$(CC) -o $@ $^ $(CFLAGS) $(DEPS)
