DEPEND = pthTextCode.o main.o public.o pthTextRW.o pthLocker.o
LIB = -lpthread
CFLAGS = -gdwarf-2 -DDEBUG
CC = /usr/bin/gcc
locker: $(DEPEND)
	$(CC) $(DEPEND) $(LIB) $(CFLAGS) -o locker
pthTextCode.o: pthTextCode.c
	$(CC) -c pthTextCode.c $(CFLAGS) -o pthTextCode.o
public.o: public.c
	$(CC) -c public.c $(CFLAGS) -o public.o
pthTextRW.o: pthTextRW.c
	$(CC) -c pthTextRW.c $(CFLAGS) -o pthTextRW.o
pthLocker.o: pthLocker.c
	$(CC) -c pthLocker.c $(CFLAGS) -o pthLocker.o
main.o: main.c
	$(CC) -c main.c $(CFLAGS) -o main.o

.PHONY:	clean cleanAll
clean:
	rm -f *.o
cleanAll:
	rm -f *.o locker
