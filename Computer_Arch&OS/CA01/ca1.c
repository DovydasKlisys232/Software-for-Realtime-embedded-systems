#include <stdio.h>

int main(void)
{
	union myUnion
	{
		float myFloat;
		unsigned int myUnsignedInt;
	};

	union myUnion z;

	printf("Please enter a float: ");
	scanf("%f", &z.myFloat);

	printf("The number you entered was: %f\n", z.myFloat);
	
	printf("The stored number in hex is: 0x%X\n", z.myUnsignedInt);

	return 0;
}

