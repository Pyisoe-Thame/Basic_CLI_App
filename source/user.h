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
    bool isSuspended;
    int ASMpts;
    char transacMsg[30];  // this size can be variable so no constant for this
    Transac transac[MAX_TRANSACTION]; 
}User;

User * getUser( int id);
void showUserInfo( int id);
void changeName( int id);
void changeEmail( int id);
void changePhone( int id);
void changePasswd( int id);
bool isUserExisting( int id);
bool isEmailTaken( char email_to_valid[]);
bool isPhoneTaken( char phone_to_valid[]);
int findMaxId();

User * user;
int totalUser = 0;

User * getUser( int id)
{
    for( int i = 0; i < totalUser; i++)  // include the 0 id (owner acc)
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

void changeName( int id)
{
    char name[NAME_SIZE];
    // I don't know when will the input end so initialise it with all 0 using memset
    memset( name, 0, sizeof(name));
    putchar('\n');  // to make more readable CLI
    printf("Enter new name :");
    scanf(" %29[^\n]", &name);  // accept up to only 29 because 30 in the struct minus \0
    for( int i = 4; i < totalUser; i++)  // exclude the default accounts
    {
        if( user[i].id == id)
        {
            stringCopy( user[i].name, name);
            printf("Name successfully changed!\n");
            break; 
        }
    }
}

void changeEmail( int id)
{
    char emailBuffer[EMAIL_SIZE];
    // I don't know when will the input end so initialise it with all 0 using memset
    memset( emailBuffer, 0x00, sizeof(emailBuffer));
    putchar('\n');  // to make more readable CLI

    email_ask:  // local branch
    printf("Enter new email :");
    scanf(" %40[^\n]", &emailBuffer);  // accept up to only 40 because 41 in the struct minus \0
      // to bypass '\n' char
    if(!isValidEmail( emailBuffer))
    {
        printf("Error! Invalid email entered.\n");
        goto email_ask;
    }
    else if( isEmailTaken( emailBuffer))  
    {
        printf("Error! The email is connected to another account.\n");
        goto email_ask;
    }

    for( int i = 4; i < totalUser; i++)  // exclude the default accounts
    {
        if( user[i].id == id)
        {
            stringCopy( user[i].email, emailBuffer);
            printf("Email successfully changed!\n");
            break; 
        }
    }
}

void changePhone( int id)
{
    char phBuffer[15];
    // initialise for storing data safely
    memset( phBuffer, 0x00, sizeof(phBuffer));
    putchar('\n');  // to make more readable CLI

    phone_ask:  // local branch
    printf("Enter new phone number (xx-xxx xxx xxx):");
    scanf(" %14[^\n]", &phBuffer);  // accept up to only 40 because 41 in the struct minus \0
    if(!isValidPh( phBuffer))
    {
        printf("Error! Invalid phone number entered.\n");
        goto phone_ask;
    }
    else if( isPhoneTaken( phBuffer))  
    {
        printf("Error! The phone number is connected to another account.\n");
        goto phone_ask;
    }

    for( int i = 4; i < totalUser; i++)  // exclude the default account
    {
        if( user[i].id == id)
        {
            stringCopy( user[i].phone, phBuffer);
            printf("Phone number successfully changed!\n");
            break; 
        }
    }
    return ;
}

void changePasswd( int id)
{
    char passwdBuffer[PASSWORD_SIZE];
    memset( passwdBuffer, 0x00, sizeof(passwdBuffer));
    putchar('\n');

    old_passwd_ask:
    printf("Enter the old password : ");
    scanf(" %29[^\n]", &passwdBuffer);
      // to bypass '\n' char
    if( stringCompare( passwdBuffer, getUser( id) -> password) == false)
    {    
        printf("Old password invalid!\n");
        goto old_passwd_ask;
    }

    passwd_ask:
    printf("\nEnter new password : ");
    scanf(" %29[^\n]", &passwdBuffer);
    if( !isStrongPasswd( passwdBuffer))
    {
        printf("Error! The password is not strong enough!\n");
        goto passwd_ask;
    }

    for( int i = 4; i < totalUser; i++)  // exclude the default accounts
    {
        if( user[i].id == id)
        {
            stringCopy( user[i].password, passwdBuffer);
            printf("Password successfully changed!\n");
            break; 
        }
    }
}

bool isUserExisting( int id)
{
    for( int i = 1; i < totalUser; i++)  // exclude searching for owner account
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
    for( int i = 4; i < totalUser; i++)  // exclude the default accounts
        if( user[i].id > maxId) 
            maxId = user[i].id;
    return maxId;
}

#endif

