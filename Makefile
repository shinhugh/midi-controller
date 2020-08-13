# Makefile

# --------------------------------------------------

# Paths
PATH_ROOT := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
PATH_BUILD := $(PATH_ROOT)/build
PATH_SRC := $(PATH_ROOT)/src

# Targets
OBJ := $(PATH_BUILD)/*.o
EXE := $(PATH_BUILD)/program
HEX := $(PATH_BUILD)/*.hex

# Commands and options
CC := avr-gcc
CFLAGS := -c -std=c11 -mmcu=atmega328p -Os -Wall -I $(PATH_SRC)
LFLAGS := -mmcu=atmega328p
OC := avr-objcopy
OCFLAGS := -O ihex -R .eeprom
UP := avrdude
UPFLAGS := -p m328p -c usbtiny -F -V

# --------------------------------------------------

# Unconditional targets
.PHONY: clean default

# --------------------------------------------------

# Default target
default:
	@echo "Options:"
	@echo "- make compile"
	@echo "- make flash"
	@echo "- make clean"

# --------------------------------------------------

# Compile into machine code for target hardware
compile:

	@echo "Compiling."
	@$(CC) $(CFLAGS) -o $(PATH_BUILD)/main.o $(PATH_SRC)/main.c
	@$(CC) $(CFLAGS) -o $(PATH_BUILD)/twi.o $(PATH_SRC)/twi.c
	@$(CC) $(CFLAGS) -o $(PATH_BUILD)/io_expand.o $(PATH_SRC)/io_expand.c
	@$(CC) $(CFLAGS) -o $(PATH_BUILD)/serial_midi.o $(PATH_SRC)/serial_midi.c
	@$(CC) $(CFLAGS) -o $(PATH_BUILD)/serial_print.o $(PATH_SRC)/serial_print.c
	@$(CC) $(CFLAGS) -o $(PATH_BUILD)/text_lcd.o $(PATH_SRC)/text_lcd.c
	@$(CC) $(LFLAGS) -o $(PATH_BUILD)/program $(PATH_BUILD)/main.o \
$(PATH_BUILD)/twi.o $(PATH_BUILD)/io_expand.o $(PATH_BUILD)/serial_midi.o \
$(PATH_BUILD)/serial_print.o $(PATH_BUILD)/text_lcd.o
	@$(OC) $(OCFLAGS) $(PATH_BUILD)/program $(PATH_BUILD)/program.hex

# --------------------------------------------------

# Flash compiled machine code to target hardware
flash:

	@echo "Flashing."
	@sudo $(UP) $(UPFLAGS) -U flash:w:$(PATH_BUILD)/program.hex

# --------------------------------------------------

# Remove compiled build
clean:

	@echo "Removing build."
	@rm -rf $(OBJ) $(EXE) $(HEX)
