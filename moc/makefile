YFLAGS = -d
OBJS = hoc.o init.o math.o symbol.o

moc:	$(OBJS)
	gcc $(OBJS) -lm -o moc

hoc.o:	hoc.h

pr:
		@pr hoc.y hoc.h init.c math.c symbol.c makefile

clean:
		rm -f $(OBJS) y.tab.[ch]
