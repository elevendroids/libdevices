INCLUDES=-Iinclude
SOURCES=$(wildcard src/bus/*.c) $(wildcard src/display/*.c) \
	$(wildcard src/gpio/*.c) $(wildcard src/sensor/*.c) \
	$(wildcard src/platform/$(PLATFORM)/*.c) \
	src/board/$(BOARD).c
BOARD=mspnode
TARGET_LIB=libdevices.a
