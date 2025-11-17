# Compiler and tools
CC = avr-gcc
CCHEX = avr-objcopy
UPLOAD = avrdude -c dragon_isp -p m328p -P usb -U
CCFLAGS = -Og -Wall -ggdb -mmcu=atmega328p

# Directories
SRC_DIR = src
BIN_DIR = bin

# Collect all C files from src
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# Output files
TARGET = Nuclear_reactor.elf
HEX = Nuclear_reactor.hex

# Default target
all: $(TARGET) $(HEX)

# make the bin directory
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile all C files into one ELF
$(TARGET): $(SRCS)
	$(CC) -D DEBUG $(CCFLAGS) -o $@ $(SRCS)

# Generate HEX from ELF
$(HEX): $(TARGET)
	$(CCHEX) -O ihex $< $@

# Upload to device
upload: $(HEX)
	$(UPLOAD) flash:w:$(HEX):i

# Debug target
debug:
	avr-gdb --tui $(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(HEX) $(SRC_DIR)/*.o
    
