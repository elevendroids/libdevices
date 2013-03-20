CC=gcc
CFLAGS=-O2 -Wall -I../../include -fdata-sections -ffunction-sections
LDFLAGS=-L../../ -ldevices -Wl,-Map,mapfile.map,--gc-sections,--print-gc-sections
#,--strip-all
SOURCES=hd44780_test.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=hd44780_test


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm -rf *.o $(EXECUTABLE)

