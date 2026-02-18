/*
    name: Dovydas Klisys
    ID: B00165094
    date: 2024-09-30
    Purpose: Code which will take user input and convert to floating-point and print out binary and hex representation
*/

#include <stdio.h>
#include <math.h>

int main()
{
    // Create a union to store the float and its binary representation
    // use instead of casting , converts the same word to both a float and an unsigned int
    union myUnion
	{
		float myFloat;
		unsigned int myUnsignedInt;
	};

    // get user input
    fprintf(stdout,"Please enter a float: ");
    union myUnion z;
    fscanf(stdin,"%f", &z.myFloat);

    // Print the value of the number scanned in using printf and %f.
    fprintf(stdout,"The number you entered was: %f\n", z.myFloat);

    //Print out the "raw" hex/binary pattern stored in the IEEE 754 float.
    fprintf(stdout,"The stored number in hex is: 0x%X\n", z.myUnsignedInt);

    // print out sign bit of stored number
    // first mask the sign bit using bitwise AND with 0x80000000, then shift it to the right by 31 bits to get the value of the sign bit (0 or 1)
    union myUnion z2;
    z2.myUnsignedInt = (z.myUnsignedInt & 0x80000000) >> 31;
    fprintf(stdout,"The sign bit is: %u\n", z2.myUnsignedInt);

    // Print out the biased exponent of the number.
    union myUnion z3;
    z3.myUnsignedInt = (z.myUnsignedInt & 0x7F800000) >> 23; // mask the exponent bits (bits 23-30) then shift by 23 bits (number of mantissa bits).
    fprintf(stdout,"The biased exponent is: %u\n", z3.myUnsignedInt);

    // Print out the unbiased exponent of the number.
    // take the biased exponent and subtract the bias (127) to get the unbiased exponent
    union myUnion z4;
    z4.myUnsignedInt = z3.myUnsignedInt - 127; 
    fprintf(stdout,"The unbiased exponent is: %u\n", z4.myUnsignedInt);

    // Print out the normalised mantissa of the number without the implied one at the beginning.
    // mask the mantissa bits (bits 0-22) to get standard mantissa, the fractional part without leading 1.
    union myUnion z5;
    z5.myUnsignedInt = z.myUnsignedInt & 0x007FFFFF;
    fprintf(stdout,"The normalised mantissa is: %u\n", z5.myUnsignedInt);

    // Print out the normalised mantissa of the number with the implied one at the beginning.
    union myUnion z6;
    z6.myFloat = z.myFloat / pow(2.0, z4.myFloat); // divide the float by 2 raised to the power of the unbiased exponent to get the normalised mantissa with the implied leading 1.
    fprintf(stdout,"The normalised mantissa with the implied one is: %f\n", z6.myFloat);

    return 0;
}