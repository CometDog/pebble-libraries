#include "pbl-stdlib.h"
#include "stdbool.h"

/**
 * Function to convert a given integer value to an ASCII representation
 * @param value Integer value to convert
 * @param buffer Char buffer to update with ASCII representation
 * @param base Numeric base to use to convert the given value
 */
char *pbl_itoa(int value, char *buffer, int base)
{
    // Return an empty string if outside of support base options
    if (base < 2 || base > 36)
    {
        *buffer = '\0';
        return buffer;
    }

    // Represent negative numbers as positive for handling
    bool is_negative = false;
    if (value < 0 && base == 10)
    {
        is_negative = true;
        value = -value;
    }

    // Write to buffer
    int write_position = 0;
    if (value == 0)
    {
        buffer[write_position++] = '0';
    }
    else
    {
        while (value != 0)
        {
            int remainder = value % base;
            buffer[write_position++] = (remainder < 10) ? remainder + '0' : remainder - 10 + 'a';
            value /= base;
        }

        // Insert a hyphen when negative
        if (is_negative)
        {
            buffer[write_position++] = '-';
        }
    }

    // C ASCII representations are null terminated
    buffer[write_position] = '\0';

    // Reverse the string
    int start = 0;
    int end = write_position - 1;
    while (start < end)
    {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }

    return buffer;
}