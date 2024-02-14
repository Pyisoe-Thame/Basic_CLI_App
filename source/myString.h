#ifndef MYSTRING_H
#define MYSTRING_H

#include <stdint.h>
#include <stdbool.h>

bool stringCompare( char [], char []);
void stringCopy( char [], char []);

bool stringCompare( char buffer[], char key[])
{
    int i = 0;
    do
    {
        if( buffer[i] != key[i])
            return false;
        i++;
    } while( ( buffer[i] != '\0' ) && ( key[i] != '\0' ));

    if( ( buffer[i] == '\0') && ( key[i] == '\0' ))  // Only if both arrays end at the same index
        return true;
    else
        return false;
}

void stringCopy( char dest[], char buffer[])  // copy from buffer array to destination array
{
    int i = 0;  // to index the arrays
    while( buffer[i] != '\0')  // as long as the content is not a null character
    {
        dest[i] = buffer[i];
        i++;
    };  
    dest[i] = '\0';  // add null character manually to set the end of destination array
}

#endif

