# Adapted from THE UNIX PROGRAMMING LANGUAGE by Brian Kernighan & Rob Pike.
# This version of the makefile simply changes the name of the output program
# to "mhc"

YFLAGS = -d
OBJS = mhc.o init.o math.o symbol.o

mhc:	$(OBJS)
	gcc $(OBJS) -lm -o mhc

mhc.o:	mhc.h

pr:
		@pr mhc.y mhc.h init.c math.c symbol.c makefile

clean:
		rm -f $(OBJS) y.tab.[ch]
