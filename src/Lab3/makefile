#execute all steps to uploading hello.c into an arduino
all: hello.o usart.o  hello.elf hello.hex 

#removes all projects of a certain type
clean: 
	rm ./*.o ./*.elf ./*.hex

#making the object code for hello.c
hello.o: hello.c
	avr-gcc -c -mmcu=atmega328p -O2 -o hello.o hello.c

#making the object code for usart-1MHz.c
usart.o: usart-1MHz.c
	avr-gcc -c -mmcu=atmega328p -O2 -o usart.o usart-1MHz.c

#linking both hello.o and usart.o to make hello.elf
hello.elf: hello.o 
	avr-gcc -mmcu=atmega328p -O2 -o hello.elf hello.o usart.o

#creating machine code from hello.elf
hello.hex: hello.elf
	avr-objcopy -O ihex hello.elf hello.hex

