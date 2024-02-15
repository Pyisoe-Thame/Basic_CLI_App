#ifndef USER_H
#define USER_H

#define NAME_SIZE 30
#define EMAIL_SIZE 41
#define PASSWORD_SIZE 30

typedef struct{
    int id;
    char name[NAME_SIZE];
    char email[EMAIL_SIZE];
    char password[PASSWORD_SIZE];
    int ASMpts;
    char transacMsg[30];  // this size can be variable so no constant for this
    Transac transac[MAX_TRANSACTION]; 
}User;

User * getUser( int id);
bool isUserExisting( int id);
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

bool isUserExisting( int id)
{
    for( int i = 0; i < totalUser; i++)
    {
        if( user[i].id == id)
            return true;  // immediate return
    }
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

