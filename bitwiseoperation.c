#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

// Function to print a 32-bit number in binary format
void print_binary(uint32_t num) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i % 8 == 0 && i != 0) printf(" "); // Add space between bytes
    }
    printf("\n");
}

// Function to process each byte of the number according to the task
uint32_t process_number(uint32_t num) {
    uint32_t result = 0;

    for (int i = 0; i < 4; i++) { // Process 4 bytes of the number
        uint8_t byte = (num >> (i * 8)) & 0xFF; // Extract the i-th byte
        uint8_t high_nibble = (byte & 0xF0) >> 4; // Extract the high nibble
        uint8_t low_nibble = byte & 0x0F;        // Extract the low nibble

        if (i % 2 == 0) { // Even byte
            uint8_t xor_result = high_nibble ^ low_nibble;
            low_nibble = ~xor_result & 0x0F; // Bitwise NOT and mask to 4 bits
        } else { // Odd byte
            high_nibble = high_nibble ^ low_nibble; // XOR result
        }

        byte = (high_nibble << 4) | low_nibble; // Reassemble the byte
        result |= (uint32_t)byte << (i * 8);    // Place the byte back in result
    }

    return result;
}

int main(int argc, char *argv[]) {
    uint32_t number;

    if (argc == 2) {
        // Try to parse the input number
        if (sscanf(argv[1], "%u", &number) != 1) {
            fprintf(stderr, "Ошибка: '%s' не является числом.\n", argv[1]);
            return 1;
        }
    } else {
        // Generate a random number if no argument is provided
        srand(time(NULL));
        number = rand();
    }

    printf("Исходное число: \n");
    print_binary(number);

    uint32_t processed = process_number(number);

    printf("Преобразованное число: \n");
    print_binary(processed);

    return 0;
}
