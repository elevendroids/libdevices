INCLUDES=-Iinclude
SOURCES=$(wildcard bus/*.c) $(wildcard display/*.c) \
	$(wildcard gpio/*.c) $(wildcard sensor/*.c) \
	$(wildcard platform/$(PLATFORM)/*.c) \
	$(wildcard sys/*.c) \
	board/$(BOARD).c
BOARD=launchpad
TARGET_LIB=libdevices.a

