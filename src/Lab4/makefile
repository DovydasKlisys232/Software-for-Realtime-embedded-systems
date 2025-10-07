#execute all steps to uploading count.c into an arduino
all: count.o usart.o count.elf count.hex count_upload

#removes all projects of a certain type
clean: 
	rm ./*.o ./*.elf ./*.hex

#making the object code for count.c
count.o: count.c
	avr-gcc -c -mmcu=atmega328p -O2 -o count.o count.c

#making the object code for usart-1MHz.c
usart.o: usart-1MHz.c
	avr-gcc -c -mmcu=atmega328p -O2 -o usart.o usart-1MHz.c

#linking both count.o and usart.o to make count.elf
count.elf: count.o 
	avr-gcc -mmcu=atmega328p -O2 -o count.elf count.o usart.o

#creating machine code from count.elf
count.hex: count.elf
	avr-objcopy -O ihex count.elf count.hex

count_upload:
	avrdude -c stk500v2 -p m328p -P /dev/ttyACM0 -U count.hex
