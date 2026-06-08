#ifndef BIGNUMBER_H
#define BIGNUMBER_H
#include <stddef.h>

/**
* @brief Structure representing a number with a mantissa and an exponent to handle very large numbers.
*/
typedef struct{
    float mantissa;
    int exponent;
} BigNumber;


/**
* @brief Adds two big numbers.
* @param a First big number.
* @param b Second big number.
* @return The result of the addition.
*/
BigNumber BigNumberAdd(BigNumber a, BigNumber b);

/**
* @brief Subtracts two big numbers.
* @param a First big number.
* @param b Second big number.
* @return The result of the subtraction.
*/
BigNumber BigNumberSubtract(BigNumber a, BigNumber b);

/**
* @brief Multiplies two big numbers.
* @param a First big number.
* @param b Second big number.
* @return The result of the multiplication.
*/
BigNumber BigNumberMultiply(BigNumber a, BigNumber b);

/**
* @brief Divides two big numbers.
* @param a First big number.
* @param b Second big number.
* @return The result of the division.
*/
BigNumber BigNumberDivide(BigNumber a, BigNumber b);

/**
* @brief Converts a float to a big number.
* @param value The float value to convert.
* @return The corresponding big number.
*/
BigNumber BigNumberFromFloat(float value);

/**
* @brief Converts a big number to a string safely.
* @param bn The big number to convert.
* @param buffer The string buffer where the result will be written.
* @param bufferSize The maximum size of the buffer.
*/
void BigNumberToString(BigNumber bn, char* buffer, size_t bufferSize);

/**
* @brief Compares two big numbers.
* @param a First big number.
* @param b Second big number.
* @return 1 if a > b, -1 if a < b, 0 if a == b.
*/
int BigNumberCompare(BigNumber a, BigNumber b);

#endif // BIGNUMBER_H