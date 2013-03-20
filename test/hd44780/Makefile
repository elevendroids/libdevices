CC=gcc
OBJDUMP=objdump
PLATFORM=linux

ifeq ($(PLATFORM),msp430)
	CC=msp430-gcc -mmcu=msp430g2553
	OBJDUMP=msp430-objdump
endif

CFLAGS=-Os -Wall -I../../include -ffunction-sections -fdata-sections
LDFLAGS=-L../../ -ldevices -Wl,--gc-sections,-Map=mapfile.map
SOURCES=hd44780_test.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=hd44780_test


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm -rf *.o $(EXECUTABLE)

dump:
	$(OBJDUMP) -d $(EXECUTABLE) > $(EXECUTABLE).asm

