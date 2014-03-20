# Required variables:
#
# BOARD - name of target board (raspberry, beaglebone, launchpad, etc.)
# INCLUDES - include directory in format "-Idir_name"
# TARGET - name of target executable/firmware
# and/or
# TARGET_LIB - name of target static library
#
include project.mk

ifeq ($(BOARD),)
$(error No board name provided - BOARD is not set)
endif

ifeq ($(TARGET),)
ifeq ($(TARGET_LIB),)
$(error No target executable/library specified)
endif
endif

PREFIX=
AR=$(PREFIX)ar
CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
LD=$(PREFIX)ld
OBJDUMP=$(PREFIX)objdump
SIZE=$(PREFIX)size
STRIP=$(PREFIX)strip
DOXYGEN=doxygen

INCLUDES=-I$(LIBROOT)/include -I$(LIBROOT)/target/include/$(PLATFORM) -I$(LIBROOT)/target/include/$(PLATFORM)/$(BOARD)

C_WARNINGS=-Wall -Wstrict-prototypes
CXX_WARNINGS=-Wall

CFLAGS=--std=c99 $(C_WARNINGS) -pedantic -ffunction-sections -fdata-sections -DPLATFORM_$(PLATFORM) $(INCLUDES) -Wl,-gc-sections
CXXFLAGS=--std=c++11 $(CXX_WARNINGS) -pedantic -ffunction-sections -fdata-sections -DPLATFORM_$(PLATFORM) $(INCLUDES)
LDFLAGS=-Wl,-Map=$(TARGET).map

ifneq ($(RELEASE),)
	CFLAGS+=-Os
	CXXFLAGS+=-Os
endif

ifeq ($(TARGET_LIB),)
	LIBS += -L$(LIBROOT) -ldevices
	LDFLAGS +=-Wl,-gc-sections
endif

# Board sections
# Set appropriate platform, cpu/mcu etc.

ifeq ($(BOARD), raspberry)
	PLATFORM=linux
endif

ifeq ($(BOARD), beaglebone)
	PLATFORM=linux
endif
 
ifeq ($(BOARD), mspnode)
	PLATFORM=msp430
	MCU=msp430g2553
	CFLAGS += -DF_CPU=16000000UL
endif

ifeq ($(BOARD), tinypad)
	PLATFORM=msp430
	MCU=msp430g2230
endif

#OUTDIR=.

# Platform sections
# Set toolchain options, flags etc.
PLATFORM_DIR=$(LIBROOT)/target/src/$(PLATFORM)
BOARD_DIR=$(PLATFORM_DIR)/$(BOARD)
include $(PLATFORM_DIR)/platform.mk
include $(BOARD_DIR)/board.mk

INCLUDES += -I$(LIBROOT)/target/include/$(PLATFORM) -I$(LIBROOT)/target/include/$(PLATFORM)/$(BOARD)

ifeq ($(PLATFORM),linux)
	LIBS += -lstdc++
endif

OUTDIR=out/$(PLATFORM)/$(BOARD)

OBJECTS=$(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SOURCES)))

all: destdir $(SOURCES) $(TARGET) $(TARGET_LIB)

$(TARGET): $(OBJECTS)
	echo "(LD): $@"
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@
	echo
	echo Firmware size:
	$(SIZE) $(TARGET)

$(TARGET_LIB): $(OBJECTS)
	echo "(AR): $@"
	$(AR) rc $(TARGET_LIB) $(OBJECTS)
	echo
	echo Library size:
	$(SIZE) $(TARGET_LIB)

%.o: %.c
	echo "(CC): $<"
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.cpp
	echo "(CXX): $<"
	$(CXX) -c $(CXXFLAGS) -o $@ $<

test:
	echo "CFLAGS: $(CFLAGS)"
	echo "LDFLAGS: $(LDFLAGS)"

.SILENT:
.PHONY:	clean test

destdir:
	mkdir -p $(OUTDIR)

clean:
	echo "Cleaning up..."
	rm -rf $(OBJECTS) $(TARGET) $(TARGET_LIB) *.dump *.map

dump:
	echo "(OBJDUMP): $(TARGET)$(TARGET_LIB)"
	$(OBJDUMP) -g -d $(TARGET)$(TARGET_LIB) > $(TARGET)$(TARGET_LIB).dump

strip:
	echo "(STRIP): $(TARGET)$(TARGET_LIB)"
	$(STRIP) $(TARGET)$(TARGET_LIB)

docs:
	echo "Generating documentation..."
	$(DOXYGEN)

