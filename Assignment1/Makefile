
CC = gcc
CFLAGS = -Wall -Wextra -O2

DEPS =

ODIR = build

.PHONY: clean all build

build: $(ODIR)/project1a-2017-annotated \
	$(ODIR)/project1a-2017-external-file \
	$(ODIR)/project1a-2017-function-loops \
	$(ODIR)/project1a-2017-mod \
	$(ODIR)/project1a-2017-param-delay \
	$(ODIR)/project1a-2017-pointer-reference \
	$(ODIR)/project1a-2017-single-function \
	$(ODIR)/project1b-2017 \
	$(ODIR)/project1c-2017 \
	$(ODIR)/app1 \
	$(ODIR)/app2 \
	$(ODIR)/app3

mkdir:
	mkdir -p $(ODIR)

clean:
	rm -Rf $(ODIR)

$(ODIR)/project1a-2017-external-file: project1/project1a-2017-external-file.c project1/delayedWrite.c | mkdir
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/project1%: project1/project1%.c | mkdir
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/app%: applications/app%.c applications/delay.c | mkdir
	$(CC) -o $@ $^ $(CFLAGS)
