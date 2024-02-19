#ifndef MYSTRINGUTIL_H
#define MYSTRINGUTIL_H

#include <stdint.h>
#include <stdbool.h>

// selective special character set
const char special[] = { '\"', '\'', '~',  '`', '@', '#', '$', '%', '^', '&', '*',  '(', ')',  
    '-', '_', '+', '=', '<', '>', ',', '.',  '?', '/', '\\', '|', '[', ']', '{', '}'};

bool stringCompare( char [], char []);
void stringCopy( char [], char []);

bool isStrongPasswd( char []);
bool isValidEmail( char email_to_Valid[]);
bool isValidPh( char phone_to_valid[13]);

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

bool isStrongPasswd( char passwd[])
{
    bool small_flg = false, cap_flg = false, num_flg = false, 
            spec_flg = false, size_flg = false;
    for( int i = 0; passwd[i] != '\0'; i++)
    {
        if( ( passwd[i] >= 0x61 ) && ( passwd[i] <= 0x7A))  // a-z
            small_flg = true;
        else if( ( passwd[i] >= 0x41 ) && ( passwd[i] <= 0x5A))  // A-Z
            cap_flg = true;
        else if( ( passwd[i] >= 0x30 ) && ( passwd[i] <= 0x39))  // 0-9
            num_flg = true;  
        else
        {
            for( int j = 0; j < sizeof(special); j++)
            {
                if( passwd[i] == special[j])  // check if the special char is from the predefined array
                    spec_flg = true; 
            }   
            if( spec_flg == false)
                // the password contains special chars which are not allowed or unknown char
                return false;  
        }
        if( i >= 6)
            size_flg = true;
    }
    if( ( small_flg == true) && ( cap_flg == true) && ( num_flg == true)\
            && (spec_flg == true) && ( size_flg == true))
        return true;
    else 
        return false;
}

bool isValidEmail( char email_to_valid[])
{
    bool dot_flag = false;
    char part1[20], part2[20];
    int i = 0;

    // 1st char can't be a number
    if( ( email_to_valid[0] >= 0x30) && ( email_to_valid[0] <= 0x39))
        return false;

    while( email_to_valid[i] != '@')
    {
        if( ( ( email_to_valid[i] >= 0x30 ) && ( email_to_valid[i] <= 0x39))  // 0-9
            || (( email_to_valid[i] >= 0x61 ) && ( email_to_valid[i] <= 0x7A))  // a-z
            || ( email_to_valid[i] == '.'))  // . is allowed
            part1[i] = email_to_valid[i];
        else if( email_to_valid[i] == '\0')  // part2 missing;
            return false;
        i++;
    }
    i++; // agian to skip the '@' character ONCE
    int j = 0;  // part2 needs a new index starting from 0
    while( email_to_valid[i] != '\0')
    {
        if( ( ( email_to_valid[i] >= 0x30 ) && ( email_to_valid[i] <= 0x39))  // 0-9
            || (( email_to_valid[i] >= 0x61 ) && ( email_to_valid[i] <= 0x7A)))  // a-z
            part2[j] = email_to_valid[i];
        else if( email_to_valid[i] == '.')
        {
            part2[j] = email_to_valid[i];
            dot_flag = true;
        }
        else
            return false;
        i++;
        j++;  // to append next index
    }
    /*
        dot flag is the actual result because
        if the program counter reaches this line, it means
        all the conditions has been satisfied and 
        the correct email is given
            
            but

        Can not simply return TRUE because
        whether part2 has the dot or not is yet unknown
    */
    return dot_flag;
}

bool isValidPh( char phone_to_valid[13])
{
    if( phone_to_valid[0] == '0' && phone_to_valid[1] == '9')
        return true;
    else
        return false;
}

#endif

