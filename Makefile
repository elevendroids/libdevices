CC=gcc
CFLAGS=-O2 -Wall -Iinclude
LDFLAGS=

PLATFORM=linux

SOURCES=$(wildcard bus/*.c) $(wildcard display/*.c) \
	$(wildcard gpio/*.c) $(wildcard sensor/*.c) \
	$(wildcard platform/$(PLATFORM)/*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))

EXECUTABLE=test
LIBRARY=libdevices.a


all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rc $(LIBRARY) $(OBJECTS)

clean:
	rm -rf $(OBJECTS) $(LIBRARY)


