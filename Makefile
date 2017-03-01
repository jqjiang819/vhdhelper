install: vhdhelper.o
	cp vhdhelper.o /usr/bin/vhdhelper

vhdhelper.o: vhdhelper.c vhdhelper.h
	gcc vhdhelper.c -o vhdhelper.o

clean:
	rm vhdhelper.o
