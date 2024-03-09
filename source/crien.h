#ifndef ENCRY_H
#define ENCRY_H

#include <string.h>
#include <stdlib.h>

char sakuraPush( char maChar, int time);
void plusCri( char * txt, char * cryKey);
void minusCri( char * txt, char * cryKey);

char sakuraPush( char maChar, int time) 
{
/* 
    Normalize shift to be within the range [0, 8)
    Because shifting minus or excessively in C 
    may result in undefined behaviour
*/
    time %= 8;  // can use CHAR_BIT which in turn is 8 again
/*
    operation should be unsigned as char is normally signed and 
    signed shifting can result in undefined behaviour on some char values
*/

    // Perform circular left shift for non-negative time
    if( time >= 0) 
    {
        char shiftedPartL = maChar << time;
        // printf("first shift: %x\n", shiftedPartL);
        char shiftedPartR = maChar >> (8 - time);
        // printf("second shift: %x\n", shiftedPartR);
        maChar = shiftedPartL | shiftedPartR;
    }

    // Perform circular right shift for negative time
    else 
    {
        // Convert negative time to positive equivalent
        time = abs(time);
        char shiftedPartR = maChar >> time;
        // printf("first shift: %x\n", shiftedPartR);
        char shiftedPartL = maChar << (8 - time);
        // printf("second shift: %x\n", shiftedPartL);
        maChar = shiftedPartR | shiftedPartL;
    }
    return maChar;  // unsigned only needed for the pushing
}

void plusCri( char * txt, char * cryKey)
{
    int lead = 0, middle = 0, base = 0; 
/*
    modChar is used to temporarily store the char before
    finally store back into the original String
    This has to be done because rapidly changing the value inside the String
    results in resetting the String back to its original value in C
*/

    for( int i = 0; i < strlen(txt); i++)  // txt[i] != '\0' would be nice too
    {
        for( int j = 0; j < strlen(cryKey); j +=2)
        {
            // ASCII chars have lead, middle, and the base
            lead = (int) cryKey[j] / 100; 
            middle = ( (int) cryKey[j] /10 ) %10; 
            base = (int) cryKey[j] % 10;
            txt[i] = sakuraPush( txt[i], lead - middle + base);
        }
        for( int j = 1; j < strlen(cryKey); j += 2)
        {
            // ASCII chars have lead, middle, and the base
            lead = (int) cryKey[j] / 100; 
            middle = ( (int) cryKey[j] /10 ) %10; 
            base = (int) cryKey[j] % 10;
            txt[i] = sakuraPush( txt[i], - lead + middle - base);
        }
    }
}

void minusCri( char * txt, char * cryKey)
{
    int lead = 0, middle = 0, base = 0; 
/*
    modChar is used to temporarily store the char before
    finally store back into the original String
    This has to be done because rapidly changing the value inside the String
    results in resetting the String back to its original value in C
*/
    for( int i = 0; i < strlen(txt); i++)  // txt[i] != '\0' would be nice too
    {
        for( int j = 0; j < strlen(cryKey); j += 2)
        {
            // ASCII chars have lead, middle, and the base
            lead = (int) cryKey[j] / 100; 
            middle = ( (int) cryKey[j] /10 ) %10;  
            base = (int) cryKey[j] % 10;
            txt[i] = sakuraPush( txt[i], -( lead - middle + base));
        }
        for( int j = 1; j < strlen(cryKey); j += 2)
        {
            // ASCII chars have lead, middle, and the base
            lead = (int) cryKey[j] / 100; 
            middle = ( (int) cryKey[j] /10 ) %10; 
            base = (int) cryKey[j] % 10;
            txt[i] = sakuraPush( txt[i], -( -lead + middle - base));
        }
    }
}

#endif