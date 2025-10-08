#execute all steps to uploading timer.c into an arduino
all: timer.o usart.o timer.elf timer.hex timer_upload

#removes all projects of a certain type
clean: 
	rm ./*.o ./*.elf ./*.hex

#making the object code for timer.c
timer.o: timer.c
	avr-gcc -c -mmcu=atmega328p -O2 -o timer.o timer.c

#making the object code for usart-1MHz.c
usart.o: usart-1MHz.c
	avr-gcc -c -mmcu=atmega328p -O2 -o usart.o usart-1MHz.c

#linking both timer.o and usart.o to make timer.elf
timer.elf: timer.o 
	avr-gcc -mmcu=atmega328p -O2 -o timer.elf timer.o usart.o

#creating machine code from timer.elf
timer.hex: timer.elf
	avr-objcopy -O ihex timer.elf timer.hex

timer_upload:
	avrdude -c stk500v2 -p m328p -P /dev/ttyACM0 -U timer.hex
