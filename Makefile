emulator: emulator.o hardware.o
	gcc -o emulator emulator.o hardware.o

assembler: assembler.o
	gcc -o assembler assembler.o

assembler.o: assembler.c
	gcc -c assembler.c

emulator.o: emulator.c
	gcc -c emulator.c

hardware.o: hardware.c
	gcc -c hardware.c

clean:
	rm -f *.o emulator assembler