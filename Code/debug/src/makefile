all: blink.elf blink.hex upload_blink

CC = avr-gcc
CCHEX = avr-objcopy
UPLOAD = avrdude -c dragon_isp -p m328p -P usb -U
CCFLAGS = -Og -Wall -ggdb

clean:
	rm -f *.out *.exe *.elf *.hex main

hello.exe: hello.c
	gcc -g hello.c -o ../bin/hello.exe
run_hello:
	../bin/hello.exe
debug_hello:
	gdb -tui ../bin/hello.exe

blink.elf: blink.c
	$(CC) -D DEBUG -mmcu=atmega328p ${CCFLAGS} -o $@ $<

blink.hex: blink.elf
	$(CCHEX) -O ihex $< $@

upload_blink: blink.hex
	$(UPLOAD) $<

debug_blink:
	avr-gdb --tui blink.elf
