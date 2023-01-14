CC=g++
CFLAGS=-Wall -g

main: clean cs3421_emul

cs3421_emul: emulator.o cpu.o clock.o memory.o
	$(CC) $(CFLAGS) -o cs3421_emul emulator.o cpu.o clock.o memory.o

emulator.o: emulator.cpp clock_client.h memory.h cpu.h clock.h
	$(CC) $(CFLAGS) -c emulator.cpp

clock_client.o: clock_client.h

memory.o: memory.h

cpu.o: cpu.h clock_client.h memory.h

clock.o: clock.h cpu.h

clean:
	find . -name '*.o' -delete


