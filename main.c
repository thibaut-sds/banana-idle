#include <stdio.h>
#include "bignumber.h"

int main(void) {
    char buffer[64];

    // Test with 500 bananas
    BigNumber bananas = BigNumberFromFloat(500.0f);
    
    // He wins 1.2e6 bananas
    BigNumber gain = BigNumberFromFloat(1.2f);
    gain.exponent = 6; 

    // Addition
    bananas = BigNumberAdd(bananas, gain);

    // Display
    BigNumberToString(bananas, buffer, sizeof(buffer));
    printf("Total Bananas: %s\n", buffer);

    return 0;
}