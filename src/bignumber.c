#include "bignumber.h"
#include <math.h>
#include <stdio.h>

// Private functions for internal use
static void Normalize(BigNumber* bn) {
    // Special case if 0
    if (bn->mantissa == 0.0f) {
        bn->exponent = 0;
        return;
    }

    // Sign handling to properly normalize the absolute value
    int sign = (bn->mantissa < 0.0f) ? -1 : 1;
    float abs_mantissa = fabsf(bn->mantissa);

    // If the mantissa is 10 or more, divide by 10 and increase the exponent
    while (abs_mantissa >= 10.0f) {
        abs_mantissa /= 10.0f;
        bn->exponent++;
    }
    // If the mantissa is between 0 and 1, multiply by 10 and decrease the exponent
    while (abs_mantissa > 0.0f && abs_mantissa < 1.0f) {
        abs_mantissa *= 10.0f;
        bn->exponent--;
    }

    // The original sign is restored
    bn->mantissa = abs_mantissa * sign;
}

BigNumber BigNumberAdd(BigNumber a, BigNumber b) {
    BigNumber result;

    // Optimisation
    if (a.exponent - b.exponent > 7) return a;
    if (b.exponent - a.exponent > 7) return b;

    // Align exponents
    if (a.exponent > b.exponent) {
        int diff = a.exponent - b.exponent;
        b.mantissa /= powf(10.0f, (float)diff);
        b.exponent = a.exponent;
    } else if (b.exponent > a.exponent) {
        int diff = b.exponent - a.exponent;
        a.mantissa /= powf(10.0f, (float)diff);
        a.exponent = b.exponent;
    }

    result.mantissa = a.mantissa + b.mantissa;
    result.exponent = a.exponent;

    Normalize(&result);
    return result;
}

BigNumber BigNumberSubtract(BigNumber a, BigNumber b) {
    BigNumber result;

    // Optimisation :  if a >> b
    if (a.exponent - b.exponent > 7) return a;
    
    // if b >> a
    if (b.exponent - a.exponent > 7) {
        b.mantissa = -b.mantissa;
        return b;
    }

    // Align exponents
    if (a.exponent > b.exponent) {
        int diff = a.exponent - b.exponent;
        b.mantissa /= powf(10.0f, (float)diff);
        b.exponent = a.exponent;
    } else if (b.exponent > a.exponent) {
        int diff = b.exponent - a.exponent;
        a.mantissa /= powf(10.0f, (float)diff);
        a.exponent = b.exponent;
    }

    result.mantissa = a.mantissa - b.mantissa;
    result.exponent = a.exponent;

    Normalize(&result);
    return result;
}

BigNumber BigNumberMultiply(BigNumber a, BigNumber b) {
    BigNumber result;
    
    result.mantissa = a.mantissa * b.mantissa;
    result.exponent = a.exponent + b.exponent;
    
    Normalize(&result);
    return result;
}

BigNumber BigNumberDivide(BigNumber a, BigNumber b) {
    BigNumber result;
    
    // Security against division by zero
    if (b.mantissa == 0.0f) {
        return a; 
    }

    result.mantissa = a.mantissa / b.mantissa;
    result.exponent = a.exponent - b.exponent;
    
    Normalize(&result);
    return result;
}

BigNumber BigNumberFromFloat(float value) {
    BigNumber bn;
    bn.mantissa = value;
    bn.exponent = 0;
    
    Normalize(&bn);
    return bn;
}

int BigNumberCompare(BigNumber a, BigNumber b) {
    // Compare exponents
    if (a.exponent > b.exponent) return 1;
    if (a.exponent < b.exponent) return -1;
    
    // If exponents are equal, compare mantissas
    if (a.mantissa > b.mantissa) return 1;
    if (a.mantissa < b.mantissa) return -1;
    
    // They are equal
    return 0;
}

void BigNumberToString(BigNumber bn, char* buffer, size_t bufferSize) {
    // Classic print for numbers that can be represented without scientific notation
    if (bn.exponent < 6) {
        float normalValue = bn.mantissa * powf(10.0f, (float)bn.exponent);
        snprintf(buffer, bufferSize, "%.0f", normalValue);
    } 
    // Scientific notation for larger numbers
    else {
        snprintf(buffer, bufferSize, "%.2fe%d", bn.mantissa, bn.exponent);
    }
}