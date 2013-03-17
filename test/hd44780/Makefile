CC=gcc
CFLAGS=-O2 -Wall -I../../include
LDFLAGS=-L../../ -ldevices
SOURCES=hd44780_test.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=hd44780_test


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm -rf *.o $(EXECUTABLE)

