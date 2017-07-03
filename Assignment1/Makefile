
CC = gcc
CFLAGS = -Wall -Wextra -O2

DEPS =

ODIR = build

$(ODIR)/project1%: project1/project1%-2017.c | mkdir
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/app%: applications/app%.c applications/delay.c | mkdir
	$(CC) -o $@ $^ $(CFLAGS)


.PHONY: clean all

build: $(ODIR)/project1a $(ODIR)/project1b $(ODIR)/project1c \
	$(ODIR)/app1 $(ODIR)/app2 $(ODIR)/app3

mkdir:
	mkdir -p $(ODIR)

clean:
	rm -Rf $(ODIR)
