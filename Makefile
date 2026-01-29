CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lelf
BSD_LIBS = -lbsd

TARGETS = prog1 prog2 prog3 prog4 prog5 prog6

all: $(TARGETS)

prog1: prog1.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

prog2: prog2.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) $(BSD_LIBS)

prog3: prog3.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

prog4: prog4.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) $(BSD_LIBS)

prog5: prog5.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

prog6: prog6.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(TARGETS)