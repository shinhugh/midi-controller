# Makefile

# --------------------------------------------------

# Paths
PATH_ROOT := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
PATH_BUILD := $(PATH_ROOT)/build
PATH_SRC := $(PATH_ROOT)/src

# Targets
OBJ := $(PATH_BUILD)/*.o
EXE := $(PATH_BUILD)/main
HEX := $(PATH_BUILD)/*.hex

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
	@avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o $(PATH_BUILD)/main.o \
$(PATH_SRC)/main.c
	@avr-gcc -mmcu=atmega328p -o $(PATH_BUILD)/main $(PATH_BUILD)/main.o
	@avr-objcopy -O ihex -R .eeprom $(PATH_BUILD)/main $(PATH_BUILD)/main.hex

# --------------------------------------------------

# Flash compiled machine code to target hardware
flash:

	@echo "Flashing."
	@avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U \
flash:w:$(PATH_BUILD)/main.hex

# --------------------------------------------------

# Remove compiled build
clean:

	@echo "Removing build."
	@rm -rf $(OBJ) $(EXE) $(HEX)
