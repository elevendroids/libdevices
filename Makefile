CFLAGS=-Os -Wall -Iinclude -fdata-sections -ffunction-sections -std=c99

# Default board
BOARD=raspberry

# Board-specific settings, assigning platform for a board

ifeq ($(BOARD),raspberry)
	PLATFORM=linux
endif

ifeq ($(BOARD),launchpad)
	PLATFORM=msp430
	MCU=msp430g2553
endif

# Platform-specific settings

ifeq ($(PLATFORM),linux)
	CC=gcc
endif

ifeq ($(PLATFORM),msp430)
	CC=msp430-gcc -mmcu=$(MCU)
endif

SOURCES=$(wildcard bus/*.c) $(wildcard display/*.c) \
	$(wildcard gpio/*.c) $(wildcard sensor/*.c) \
	$(wildcard radio/*.c) \
	$(wildcard platform/$(PLATFORM)/*.c)\
	board/$(BOARD).c
	
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))

LIBRARY=libdevices.a

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rc $(LIBRARY) $(OBJECTS)

clean:
	rm -rf $(OBJECTS) $(LIBRARY)


