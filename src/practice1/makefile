r#execute all steps to uploading timestamp.c into an arduino
all: timestamp.o usart.o setup.o timestamp.elf timestamp.hex timestamp_upload

#removes all projects of a certain type
clean: 
	rm ./*.o ./*.elf ./*.hex

#making the object code for timestamp.c
timestamp.o: timestamp.c
	avr-gcc -c -mmcu=atmega328p -O2 -o timestamp.o timestamp.c

#making the object code for usart-1MHz.c
usart.o: usart-1MHz.c
	avr-gcc -c -mmcu=atmega328p -O2 -o usart.o usart-1MHz.cd

#linking both timestamp.o and usart.o to make timestamp.elf
timestamp.elf: timestamp.o 
	avr-gcc -mmcu=atmega328p -O2 -o timestamp.elf timestamp.o usart.o 

#creating machine code from timestamp.elf
timestamp.hex: timestamp.elf
	avr-objcopy -O ihex timestamp.elf timestamp.hex

timestamp_upload:
	avrdude -c stk500v2 -p m328p -P /dev/ttyACM0 -U timestamp.hex
