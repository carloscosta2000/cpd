#include <stdio.h>
#define LONG_SIZE (sizeof(long) * 8) // Number of bits in a long

int main() {
    int array_size = 10;
    long bit_array = 0;

    printf("array: %ld\n", bit_array);
    // Set the 2nd and 7th bits
    bit_array = bit_array | (1L << 2);
    bit_array |= (1L << 7);
    bit_array |= (1L << 10);
    printf("array: %ld\n", bit_array);

    // Check if the 2nd and 7th bits are set
    if (bit_array & (1L << 2))
        printf("2nd bit is set\n");
    if (bit_array & (1L << 7))
        printf("7th bit is set\n");

    // Print the set bits in the bit array
    printf("Set bits: ");
    for (int i = 0; i < LONG_SIZE; i++) 
        if (bit_array & (1L << i)) 
            printf("%d ", i);
        
    printf("\n");
    // Clear the 2nd bit
    bit_array &= ~(1L << 2);
    // Clear the 7th bit
    bit_array &= ~(1L << 7);
    //leaving 10th bit uncleared
    // Check if the 2nd bit is clear
    if ((bit_array & (1L << 2)) == 0) 
        printf("2nd bit is clear\n");
    // Check if the 7th bit is clear
    if ((bit_array & (1L << 7)) == 0)
        printf("7th bit is clear\n");

    // Print the set bits in the bit array
    printf("Set bits: ");
    printf("LONG SIZE -> %ld\n", LONG_SIZE);
    for (int i = 0; i < LONG_SIZE; i++) 
        if (bit_array & (1L << i)) 
            printf("%d ", i);
    printf("\n");
    return 0;
}