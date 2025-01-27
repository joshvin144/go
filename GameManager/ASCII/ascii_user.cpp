#include <iostream>
#include <stdint.h>
#include <vector>
#include "ascii.h"


// Find the first comma in the set
int find_comma(std::vector<int>* p_ascii_char_set)
{
    int comma_idx = -1;
    for(int i = 0; i <= (p_ascii_char_set->size() - 1) and (-1 == comma_idx); i++)
    {
        if(ASCII_ENCODING_COMMA == p_ascii_char_set->at(i))
        {
            comma_idx = i;
        }
    }
    return comma_idx;
}


/**
 * Converts ASCII character set ['0' - '9'] to a decimal value
 * For example, converts ['1', '1'] to 11.
*/
int ascii_char_set_to_decimal(std::vector<int>* p_ascii_char_set)
{
    int decimal_value = 0;
    for(int i = 0; i <= (p_ascii_char_set->size() - 1); i++)
    {
        int ascii_char = p_ascii_char_set->at(i);
        int temp = (ascii_char - ASCII_ENCODING_0);
        // Multiply by the number of tens
        // The 0th index is not a tens place, so decrement to 1
        for(int j = (p_ascii_char_set->size() - 1 - i); j >= 1; j--)
        {
            temp *= 10;
        }
        decimal_value += temp;
    }
    return decimal_value;
}