vhdhelper: vhdhelper.c vhdhelper.h vhduuid.c vhduuid.h
	gcc .\vhdhelper.c .\vhduuid.c -lole32 -o vhdhelper.exe

clean:
	rm .\vhdhelper.exe
