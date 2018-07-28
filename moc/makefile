# Adapted from THE UNIX PROGRAMMING LANGUAGE by Brian Kernighan & Rob Pike.
# This version of the makefile simply changes the name of the output program
# to "moc"

YFLAGS = -d
OBJS = hoc.o init.o math.o symbol.o

moc:	$(OBJS)
	gcc $(OBJS) -lm -o moc

hoc.o:	hoc.h

pr:
		@pr hoc.y hoc.h init.c math.c symbol.c makefile

clean:
		rm -f $(OBJS) y.tab.[ch]
