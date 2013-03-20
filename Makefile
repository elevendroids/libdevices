CC=gcc
CFLAGS=-O2 -Wall -Iinclude -fdata-sections -ffunction-sections
LDFLAGS=

PLATFORM=linux

ifeq ($(PLATFORM),msp430)
	CC=msp430-gcc -mmcu=msp430g2553
endif

SOURCES=$(wildcard bus/*.c) $(wildcard display/*.c) \
	$(wildcard gpio/*.c) $(wildcard sensor/*.c) \
	$(wildcard platform/$(PLATFORM)/*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))

LIBRARY=libdevices.a

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rc $(LIBRARY) $(OBJECTS)

clean:
	rm -rf $(OBJECTS) $(LIBRARY)


