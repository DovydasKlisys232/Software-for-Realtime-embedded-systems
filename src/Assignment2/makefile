#execute all steps to uploading False_alarm.c into an arduino
all1: False_alarm.o usart.o False_alarm.elf False_alarm.hex False_alarm_upload

#removes all projects of a certain type
clean: 
	rm ./*.o ./*.elf ./*.hex

#making the object code for False_alarm.c
False_alarm.o: False_alarm.c
	avr-gcc -c -mmcu=atmega328p -O2 -o False_alarm.o False_alarm.c

#making the object code for usart-1MHz.c
usart.o: usart-1MHz.c
	avr-gcc -c -mmcu=atmega328p -O2 -o usart.o usart-1MHz.c

#linking both False_alarm.o and usart.o to make False_alarm.elf
False_alarm.elf: False_alarm.o 
	avr-gcc -mmcu=atmega328p -O2 -o False_alarm.elf False_alarm.o usart.o

#creating machine code from False_alarm.elf
False_alarm.hex: False_alarm.elf
	avr-objcopy -O ihex False_alarm.elf False_alarm.hex

False_alarm_upload:
	avrdude -c stk500v2 -p m328p -P /dev/ttyACM0 -U False_alarm.hex

#execute all steps to uploading False_alarm.c into an arduino
all2: fixed.o usart.o fixed.elf fixed.hex fixed_upload

#removes all projects of a certain type
clean: 
	rm ./*.o ./*.elf ./*.hex

#making the object code for fixed.c
fixed.o: fixed.c
	avr-gcc -c -mmcu=atmega328p -O2 -o fixed.o fixed.c

#making the object code for usart-1MHz.c
usart.o: usart-1MHz.c
	avr-gcc -c -mmcu=atmega328p -O2 -o usart.o usart-1MHz.c

#linking both fixed.o and usart.o to make fixed.elf
fixed.elf: fixed.o 
	avr-gcc -mmcu=atmega328p -O2 -o fixed.elf fixed.o usart.o

#creating machine code from fixed.elf
fixed.hex: fixed.elf
	avr-objcopy -O ihex fixed.elf fixed.hex

fixed_upload:
	avrdude -c stk500v2 -p m328p -P /dev/ttyACM0 -U fixed.hex
