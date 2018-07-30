# Adapted from THE UNIX PROGRAMMING LANGUAGE by Brian Kernighan & Rob Pike.
# This version of the makefile simply changes the name of the output program
# to "moc"

YFLAGS = -d
OBJS = moc.o init.o math.o symbol.o

moc:	$(OBJS)
	gcc $(OBJS) -lm -o moc

moc.o:	moc.h

pr:
		@pr moc.y moc.h init.c math.c symbol.c makefile

clean:
		rm -f $(OBJS) y.tab.[ch]
