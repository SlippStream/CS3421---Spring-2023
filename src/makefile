CC=g++
CFLAGS=-Wall -g -fno-common
SOURCES := $(shell find -name '*.cpp')
HEADERS := $(shell find -name '*.h')
OUTPUT := cs3421_emul

all: clean $(OUTPUT)

$(OUTPUT): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SOURCES)

clean:
	$(RM) $(OUTPUT)