#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "myString.h"
#include "user.h"

void signInPage();
void registration();
void searchUser();
void deleteUser( int id);  // id needed to know if the user is deleting its own or not

// Menu
void startMenu();
void userMenu( int id);
void adminMenu( int id);

void doExit();
void showTransacHistory( int id);
void changeMenu( int id);
void changeName( int id);
void changeEmail( int id);
void changePhone( int id);
void changePasswd( int id);
void login( int id);
void logout( int id);

void createAdminAcc();
void readUserFromFile();
void writeUserToFile();

bool isStrongPasswd( char []);
bool isValidEmail( char email_to_Valid[]);
bool isValidPh( char phone_to_valid[13]);

void ptsProcess( int giveId);
void ptsGive( int giveId, int receiveId, int pts);

// selective special character set
char special[] = { '\"', '\'', '~',  '`', '@', '#', '$', '%', '^', '&', 
                    '*',  '(', ')',  '-', '_', '+', '=', '<', '>', ',', 
                    '.',  '?', '/', '\\', '|', '[', ']', '{', '}'};
const char * filePath = "../data/db.bin";  // relative file path to binary file
FILE * fptr;

// Transac * transac = NULL;  // initialise indefinite transaction events 
int * onlineUserID;  // to store the id of online user (needs concurrency control)
int totalOnlineUser = 0;

int main()
{
    readUserFromFile();

    if( totalUser == 0)  // data file is empty
        createAdminAcc();

    while(true)
    {
        startMenu();
    }
}

void signInPage()
{
    char mailBuffer[EMAIL_SIZE];  // to store mail temporarily during input process
    char passwdBuffer[PASSWORD_SIZE];  // to store password temporarily during input process
    memset( mailBuffer, 0, sizeof(mailBuffer));
    memset( passwdBuffer, 0, sizeof(passwdBuffer));
    
    printf("\nEnter email : ");
    scanf(" %40[^\n]", &mailBuffer);
    

    for( int i = 0; i < totalUser; i++)  // loop will run to the fullest unless mail found
    {
        if( stringCompare( mailBuffer, user[i].email) == true)
        {    
            int id = user[i].id;  // i-1 no longer works; if a user deleted, id will shuffle
            printf("Enter password : ");
            scanf(" %29[^\n]", &passwdBuffer);
            
            if( stringCompare( passwdBuffer, user[i].password) == true)
            {
                login(id); 
                if( id >= 1 && id <= 3)
                    adminMenu(id);
                userMenu(id);  
            }
            printf("Sorry, the password you entered is invalid!\n");
            return ;
        }
    }
    printf("Sorry, the email you entered is not registered!\n");
    return ;
}

void registration()
{
    char nameBuffer[NAME_SIZE];  // to store mail temporarily during input process
    char mailBuffer[EMAIL_SIZE];  // to store mail temporarily during input process
    char passwdBuffer[PASSWORD_SIZE];  // to store password temporarily during input process

    // initialise with \0 to make them end with \0 because %[^\n] doesn't
    memset( nameBuffer, 0, sizeof(nameBuffer));
    memset( mailBuffer, 0, sizeof(mailBuffer));
    memset( passwdBuffer, 0, sizeof(passwdBuffer));

    int asmPtsBuffer = 0;  // to store the ASM point on account creation

    printf("\nEnter user name : ");
    scanf(" %29[^\n]", &nameBuffer);
      // get a char but do not store it anywhere to clear input buffer

    email_ask:
    printf("Enter a valid email : ");
    scanf(" %40[^\n]", &mailBuffer);
      // get a char but do not store it anywhere to clear input buffer
    if( !isValidEmail(mailBuffer)) 
    {
        printf("Error! Invalid email entered.\n");
        goto email_ask;
    }
    else if( isEmailTaken(mailBuffer))  
    {
        printf("Error! The email is connected to another account.\n");
        goto email_ask;
    }

    passwd_ask:
    printf("Enter password : ");
    scanf(" %29[^\n]", &passwdBuffer);
    
    if( !isStrongPasswd( passwdBuffer))
    {
        printf("Error! The password is not strong enough!\n");
        goto passwd_ask;
    }

    pts_ask:
    printf("Enter ASM point [0-500] : ");
    scanf(" %d", &asmPtsBuffer);
    if( asmPtsBuffer > 500 || asmPtsBuffer < 0 )
    {
        printf("Error! Invalid amount of ASM point. \n");
        goto pts_ask;
    }

    totalUser++;
    user = (User *)realloc( user, sizeof(User) * totalUser);  // memory re-allocation
/*
    new id is created according to the max id, because
    deleted user are overwritten with the last user
    2nd argument to findMaxId must be set "totalUser - 1" to exclude the currently 
    registering user.
*/ 
    user[totalUser-1].id = findMaxId( user, totalUser-1) + 1; 
    stringCopy( user[totalUser-1].name, nameBuffer);
    stringCopy( user[totalUser-1].email, mailBuffer);
    stringCopy( user[totalUser-1].password, passwdBuffer);
    user[totalUser-1].ASMpts = asmPtsBuffer;
// replace "\0" with "-\0" to show more readable format 
    stringCopy( user[totalUser-1].transacMsg, "-");  
// initialise the memory with zero so I can check with certain conditions
    memset( user[totalUser-1].transac, 0x00, sizeof(Transac) * MAX_TRANSACTION);

    fptr = fopen( filePath, "ab");
    if( fptr == NULL) 
    {
        perror("Error opening file to append!");
        exit(EXIT_FAILURE);
    }
    size_t elementWritten = fwrite( &user[totalUser-1], sizeof(User), 1, fptr);
    if( elementWritten != 1)
    {
        perror("Error appending user to the file!");
        exit(EXIT_FAILURE);
    }
    fclose(fptr);

    printf("Your account has been successfully created!\n");
}

void createAdminAcc()
{
    char name[5];  // sizes specifically customised for admins
    char email[15];  //    ||
    char password[6];  //    ||
    int ASMpts; 

    totalUser = 3;  
    user = (User*) realloc( user, sizeof(User) * 3);

    for( int i = 0; i < 3; i++)
    {
        sprintf( name, "adm%d", i+1);
        sprintf( email, "adm%d@gmail.com", i+1);
        sprintf( password, "Adm#%d", i+1);

        user[i].id = i + 1;  // id starts from 1
        stringCopy( user[i].name, name);
        stringCopy( user[i].email, email);
        stringCopy( user[i].phone, "-");
        stringCopy( user[i].password, password);
        user[i].ASMpts = 0;
        stringCopy( user[i].transacMsg, "-");
    }
    writeUserToFile();
}

void readUserFromFile()
{
    fptr = fopen( filePath, "rb");
    if( fptr == NULL)
    {
        perror("Error opening the user database!");
        exit(EXIT_FAILURE);
    }
    
    // file size calculating section
    fseek( fptr, 0, SEEK_END);  // move the coordinator to 0 byte from EoF
    /* 
        used long bcz size_t cannot handle -ive vlaue 
        which might become problematic; ftell returns -1 if error happens
    */
    long fileSize = ftell(fptr);  // tell how is the current coordinator position  
    /* 
        set the coordinator/cursor back to 0 byte from BoF (Beginning of File)
        for the next reading in case, so not too much in need to do
    */
    fseek( fptr, 0, SEEK_SET);
    totalUser = fileSize/sizeof(User);  // get the number of saved users  
    user = (User*)malloc(fileSize);  // fileSize already is sizeof(User)*readCount
    size_t readCount = fread( user, sizeof(User), totalUser, fptr);  // accumulate the user number
    if( readCount == readCount)
    {
        printf("User data reading successful.\n");
        fclose(fptr);
        return ;
    }  // else
    perror("Error! Something bad happened during reading user data.\n");
    exit(EXIT_FAILURE);
}

void writeUserToFile()
{
    fptr = fopen( filePath, "wb");  // overwrite the data every time the data is updated.
    if( fptr != NULL)  // best case senerio check first
    {
        /* 
            user here is a pointer, working the same as- 
            the address to the first element of array or &user[0] 
        */
        size_t writtenUserAmount = fwrite( user, sizeof(User), totalUser, fptr);  
        if( writtenUserAmount == totalUser)  // best case senerio check first
        {
            fclose(fptr);
            printf("User data saved successfully.\n");
            return ;
        }
        perror("Error saving user data to file.");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    perror("Error opening the user database for saving data!");
    exit(EXIT_FAILURE);
}

void searchUser()
{
    int id;
    printf("\nEnter the id number you want to search : ");
    scanf( "%d", &id);
    if( !isUserExisting(id))
    {
        printf("The user doesn't exist!\n");
        return ;
    }
    showUserInfo( id);
    return ;
}

void deleteUser( int id)
{   
    int deleteId;

    askDeleteId:
    printf("\nEnter the id of the user to delete : ");
    scanf( "%d", &deleteId);

    if( deleteId >= 1 && deleteId <= 3 )
    {
        printf( "Sorry, the id is not avaialable to delete.\n");
        goto askDeleteId;
    }

    if( isUserExisting( deleteId) == false)
    {
        printf("Error! There is no user with such id.");
        return ;
    }

/*
    remove the deleteId from onlineUserID 
    but if the deleteId is online now, he can still be using it for the time being
    and can mess up like doing transactions 
    and I don't know how to send him back to start menu yet
*/
    logout(deleteId);  
    // overwrite the element to remove, with the last element
    *getUser(deleteId) = user[totalUser-1];
    
    // resizing the user 
    totalUser--;
    User * newUser = (User*)malloc(sizeof(User) * totalUser);
    if( newUser == NULL)
    {
        perror("Error creating new user array.\n");
        exit(EXIT_FAILURE);
    }
    for( int i = 0; i < totalUser; i++)  // copy till before the last index
        newUser[i] = user[i];
    free(user);  // free the old user
/*
    user is global and newUser is local, so user will carry the data agian
    by assigning the pointer, they both now point the same data
    free(newUser) would free(user) again so be cautious
    the newUser will get off after the function since it is local
*/
    user = newUser; 

    writeUserToFile();
    printf("Account deleted successfully!\n");
    return ;
}

void startMenu()
{
    int _command = 0; // to get the sign in/up decision
    putchar('\n');
    printf("1. Sign in \n");
    printf("2. Sign up \n");
    printf("3. Exit \n");
    scanf("%d", &_command);

    switch(_command)
    {
        case 1:
            signInPage();
            break;
        case 2:
            registration();
            startMenu();
            break;
        case 3:
            doExit(0);
            break;
        default:
            perror("Error! Unspported input detected!\n");
            perror("Ending the program...\n");
            exit(EXIT_FAILURE);
            break;
    };
    startMenu();
}

void userMenu( int id)
{
    int _command = 0; // to get the app in menu decision
    putchar('\n');
    printf("1. Profile\n");
    printf("2. Change Menu\n");
    printf("3. Give ASM Points\n");
    printf("4. Show Transaction History\n");
    printf("5. Log Out\n");
    printf("6. Log Out & Exit\n");
    scanf("%d", &_command);

    switch(_command)
    {
        case 1:
            showUserInfo( id);
            userMenu( id);
            break;
        case 2:
            changeMenu( id);
            userMenu( id);
            break;
        case 3:
            ptsProcess( id);
            userMenu( id);
            break;
        case 4:
            showTransacHistory(id);
            userMenu(id);
            break;
        case 5:
            logout(id);
            startMenu();
            break;
        case 6:
            logout(id);
            doExit(0);
            break;
        default:
            break;
    };
    userMenu( id);
}

void adminMenu( int id)
{
    int _command = 0; // to get the app in menu decision
    putchar('\n');
    printf("1. Profile\n");
    printf("2. Delete User\n");
    printf("3. Search User\n");
    printf("4. Log Out\n");
    printf("5. Log Out & Exit\n");
    scanf("%d", &_command);

    switch(_command)
    {
        case 1:
            showUserInfo( id);
            adminMenu( id);
            break;
        case 2:
            deleteUser( id);
            adminMenu(id);
            break;
        case 3:
            searchUser(id);
            adminMenu(id);
            break;
        case 4:
            logout(id);
            startMenu();
            break;
        case 5:
            logout(id);
            doExit(0);
            break;
        default:
            break;
    };
    adminMenu(id);
}

void changeMenu( int id)
{
    int changeChoice = 0;
    putchar('\n');
    printf("1. Change name\n");
    printf("2. Change email\n");
    printf("3. Change phone number\n");
    printf("4. Change password\n");
    scanf("%d", &changeChoice);

    switch(changeChoice)
    {
        case 1:
            changeName(id);
            break;
        case 2:
            changeEmail(id);
            break;
        case 3:
            changePhone(id);
            break;
        case 4:
            changePasswd(id);
            break;
        default:
            printf("Other options are not supported!\n");
            break;
    }
}

void changeName( int id)
{
    char name[NAME_SIZE];
    // I don't know when will the input end so initialise it with all 0 using memset
    memset( name, 0, sizeof(name));
    putchar('\n');  // to make more readable CLI
    printf("Enter new name :");
    scanf(" %29[^\n]", &name);  // accept up to only 29 because 30 in the struct minus \0
      // to bypass '\n' char
    for( int i = 3; i < totalUser; i++)
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

    for( int i = 3; i < totalUser; i++)
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
      // to bypass '\n' char
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

    for( int i = 3; i < totalUser; i++)
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

    for( int i = 3; i < totalUser; i++)
    {
        if( user[i].id == id)
        {
            stringCopy( user[i].password, passwdBuffer);
            printf("Password successfully changed!\n");
            break; 
        }
    }
}

void showTransacHistory( int id)
{
    User * tempUser =  getUser(id);
/*
    next line does intentionally assign an array to a pointer
    additional pointer( memory address) is declared to simply the descendant lines of codes
    otherwise, referencing on printing will be too long 
*/
    Transac * tempTransac = tempUser -> transac;
    char date_string[17], time_string[9];  // resize it according to the format
    int i = 0;
    while( tempTransac[i].from != 0 && i < MAX_TRANSACTION)
    {
        strftime( date_string, sizeof(date_string), "%a, %b/%d/%Y", &(tempTransac[i].time_info));  // format date
        strftime( time_string, sizeof(time_string), "%H:%M:%S", &(tempTransac[i].time_info));  // format time
        printf("\nfrom: %d  to: %d  amount: %d  date: %s  time: %s", tempTransac[i].from, tempTransac[i].to, 
        tempTransac[i].amount, date_string, time_string);
        i++;
    }
    putchar('\n');
    return ;
}

void login( int id)
{
    totalOnlineUser++;
    onlineUserID = (int*)realloc( onlineUserID, sizeof(int) * totalOnlineUser);
    onlineUserID[totalOnlineUser-1] = id;
    return ;
}

void logout( int logoutId)
{
    for( int i = 0; i < totalOnlineUser; i++)  // loop will end even if no logoutId found
    {
        if( onlineUserID[i] == logoutId)
        {
            onlineUserID[i] = onlineUserID[totalOnlineUser-1];  // replace with last one
            totalOnlineUser--;
            int * tempOnlineUserID = (int*) malloc( sizeof(int) * totalOnlineUser);
/*
    be aware the total online user is now ONE LESS than before
    check if ZERO and unless proceed the next assignment looping 
    if ZERO malloc above will return ZERO pointer or NULL depending on the compiler
    be cautious when working with memory allocations
*/
            if( totalOnlineUser != 0)
                for( int j = 0; j < totalOnlineUser; j++)  
                    tempOnlineUserID[j] = onlineUserID[j];
            free(onlineUserID);  // free the old memory from global pointer
            onlineUserID = tempOnlineUserID;  // assign the new memory to global pointer
            break;  // immediate drop out to reduce runtime overhead
            //  the outer loop with new limit number( total user online) will be neglected
        }
    }
    return ;
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

void ptsProcess( int giveId)
{
    int receiveId, pts = 0;
    char passwdBuffer[PASSWORD_SIZE] = "\0";
    putchar('\n');

    askreceiveId:
    printf("Enter the user id you want to give : ");
    scanf(" %d", &receiveId);
    if( receiveId >=1 && receiveId <= 3)  // cannot give to admin 
    {
        printf("Error! Invalid user id.\n");
        goto askreceiveId;
    }
    else if( !isUserExisting(receiveId))
    {
        printf("Error! The user does not exist.\n");
        goto askreceiveId;
    }

    askPts:
    printf("Enter ASM amount to give : ");
    scanf(" %d", &pts);
    if( pts > getUser(giveId) -> ASMpts)
    {
        printf("Error! You cannot give more than you have\n");
        goto askPts;
    }
    else if( pts < 1)
    {
        printf("Invalid amount of points.\n");
        goto askPts;
    }

    printf("Enter the password to confirm : ");
    scanf(" %[^\n]", &passwdBuffer);
      // to bypass '\n' char
    if( !stringCompare( passwdBuffer, getUser(giveId) -> password))
    {
        printf("Password does not match...\n");
        printf("Please proceed from the start again...\n");
        return ;
    }

    ptsGive( giveId, receiveId, pts);
    
    writeUserToFile();

    printf("You have given %d ASM pts to user %d successfully!\n", pts, receiveId);
}

void ptsGive( int giveId, int receiveId, int pts)  // both id and pts must hvae been validated
{
    User * giver = getUser(giveId);
    User * receiver = getUser(receiveId);

    char msgBuffer[34];  // can fit up to 3-digit point and 3-digit user id to show
    memset( msgBuffer, 0, sizeof(msgBuffer));  // bitwise initialise to 0
    
    giver -> ASMpts -= pts;  // deduct pts from giver
    snprintf( msgBuffer, sizeof(msgBuffer), "%d points given to user-%d", pts, receiveId);
    stringCopy( giver -> transacMsg, msgBuffer);  // add transaction message
    addTransac( giver -> transac, giveId, receiveId, pts);  // add transaction record

    receiver -> ASMpts += pts;  // add pts to receiver
// I won't reset the buffer here.
// I trust snprintf will safely handle this (end with null char).
    snprintf( msgBuffer, sizeof(msgBuffer), "%d points received from user-%d", pts, giveId);
    stringCopy( receiver -> transacMsg, msgBuffer);  // add transaction message
// no need to add a new transaction on receiver side if giver is receiver
    if( giveId != receiveId) 
        addTransac( receiver -> transac, giveId, receiveId, pts);  // add transaction record
    return ;
}

void doExit()  
{
    // program may crash upon failing the following functions again and again
    putchar('\n');
    writeUserToFile();
    free(user);
    printf("Exiting the program.\n\n");
    exit(EXIT_SUCCESS);
}

