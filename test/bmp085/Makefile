CC=gcc
CFLAGS=-O2 -Wall -I../../include
LDFLAGS=-L../../ -ldevices
SOURCES=bmp085_test.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bmp085_test


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm -rf *.o $(EXECUTABLE)

