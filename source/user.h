#ifndef USER_H
#define USER_H

#include "transac.h"

#define NAME_SIZE 30
#define EMAIL_SIZE 41
#define PASSWORD_SIZE 30

typedef struct{
    int id;
    char name[NAME_SIZE];
    char email[EMAIL_SIZE];
    char phone[15];
    char password[PASSWORD_SIZE];
    int ASMpts;
    char transacMsg[30];  // this size can be variable so no constant for this
    Transac transac[MAX_TRANSACTION]; 
}User;

User * getUser( int id);
void showUserInfo( int id);
bool isUserExisting( int id);
bool isEmailTaken( char email_to_valid[]);
bool isPhoneTaken( char phone_to_valid[]);
int findMaxId();

User * user;
int totalUser = 0;

User * getUser( int id)
{
    for( int i = 0; i < totalUser; i++)
    {
        if( user[i].id == id)
            return &user[i];
    }
    return NULL;
}

void showUserInfo( int id)
{
    User * tempUser = getUser(id);
    putchar('\n');
    printf("User ID         : %d\n", tempUser -> id);
    printf("User Name       : %s\n", tempUser -> name);
    printf("User Email      : %s\n", tempUser -> email);
    printf("User Phone      : %s\n", tempUser -> phone);
    printf("ASM Balance     : %d\n", tempUser -> ASMpts);
    printf("Transac Message : %s\n", tempUser -> transacMsg);
}

bool isUserExisting( int id)
{
    for( int i = 0; i < totalUser; i++)
    {
        if( user[i].id == id)
            return true;  // immediate return
    }
    return false;
}

bool isEmailTaken( char email_to_valid[])
{
    for( int i = 0; i < totalUser; i++)  // 0 to 2 is intentionally included
        if( stringCompare( email_to_valid, user[i].email) == true)
            return true; 
    return false;
}

bool isPhoneTaken( char phone_to_valid[])
{
    for( int i = 0; i < totalUser; i++)  // 0 to 2 is intentionally included
        if( stringCompare( phone_to_valid, user[i].phone) == true)
            return true; 
    return false;
}

int findMaxId() 
{
    int maxId = -1;
    // 2 must be the maxId when there is no user created
    for( int i = 2; i < totalUser; i++) 
        if( user[i].id > maxId) 
            maxId = user[i].id;
    return maxId;
}

#endif

