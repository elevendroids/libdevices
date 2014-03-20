SOURCES=$(wildcard src/*.c) \
	$(wildcard src/bus/*.c) $(wildcard src/display/*.c) \
	$(wildcard src/gpio/*.c) $(wildcard src/sensor/*.c) \
	$(wildcard $(PLATFORM_DIR)/*.c) $(wildcard $(BOARD_DIR)/*.c)
LIBROOT=.
PLATFORM=msp430
BOARD=launchpad
TARGET_LIB=libdevices.a

