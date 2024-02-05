#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_USER

typedef struct{
    int id;
    char name[20];
    char email[41];
    char password[30];
    int ASMpts;
    /* 
        temporarily storing the last transaction
        ( which will be made a separate structure in the future) 
    */
    char transacMsg[40];  
}User;

// typedef struct{
//     int from;
//     int to;
//     int amount;
//     char eventTime[30];
// }Transac;

void createAdminAcc();
void signInPage();
void registration();
void deleteUser( int id);  // id needed to know if the user is deleting its own or not
void searchUser();
int findMaxId( User * user, int totalUser);

// Menu
void startMenu();
void userMenu( int id);
void adminMenu( int id);

void doExit( int x);
void showProfile( int id);
// void logout( int id);

void readUserFromFile();
void writeUserToFile();
// void appendTransacToFile();

bool stringCompare( char [], char []);
void stringCopy( char [], char []);

bool isStrongPasswd( char []);
bool isValidEmail( char []);

void ptsProcess( int giveId);
void ptsGive( int giveId, int receiveId, int pts);

// selective special character set
char special[] = { '\"', '\'', '~', '`', '@', '#', '$', '%', '^', '&', 
                    '*', '(', ')', '-', '_', '+', '=', '<', '>', ',', 
                    '.', '?', '/', '\\', '|', '[', ']', '{', '}'};
const char * filePath = "../data/db.bin";  // relative file path to binary file
FILE * fptr;
User * user;
int totalUser = 0;

// Transac * transac = NULL;  // initialise indefinite transaction events 
// int onlineUserIDs[MAX_USER];  // to store the id of online user (needs concurrency control)

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
    char _mailBuffer[50];  // to store mail temporarily during input process
    char _passwdBuffer[30];  // to store password temporarily during input process
    int id = 0;  // to store the id of the user who is logging in
    bool mailExist = false;
    printf("Enter email : ");
    scanf(" %[^'\n']", &_mailBuffer);

    for( int i = 0; i < totalUser; i++)
    {
        if( stringCompare( _mailBuffer, user[i].email) == true)
        {    
            mailExist = true;
            id = i + 1;
            break;
        }
    }
    if( mailExist == true)
    {
        printf("Enter password : ");
        scanf(" %[^'\n']", &_passwdBuffer);
        if( stringCompare( _passwdBuffer, user[id-1].password) == 1)
        {
            if( id >= 1 && id <= 3)
                adminMenu(id);
            userMenu(id);  
        }
        printf("Sorry, the password you entered is invalid!\n");
        startMenu();
    }
    printf("Sorry, the email you entered is not registered!\n");
    startMenu();
}

void registration()
{
    char _nameBuffer[20];  // to store mail temporarily during input process
    char _mailBuffer[50];  // to store mail temporarily during input process
    char _passwuseruffer[30];  // to store password temporarily during input process
    int _asmPtsBuffer = 0;  // to store the ASM point on account creation

    printf("Enter user name : ");
    scanf(" %[^'\n']", &_nameBuffer);

    email_ask:
    printf("Enter a valid email : ");
    scanf(" %[^'\n']", &_mailBuffer);
    if(!isValidEmail(_mailBuffer))  // check if valid email is entered
    {
        printf("Error! Invalid email entered.\n");
        goto email_ask;
    }
    for( int j = 0; j < totalUser; j++)  // 0 to 2 is intentionally included
    {
        if( stringCompare( _mailBuffer, user[j].email) == true)
        {
            printf("Sorry, this email has been connected to another account!\n");
            goto email_ask;
        }
    }

    passwd_ask:
    printf("Enter password : ");
    scanf(" %[^'\n']", &_passwuseruffer);
    if( !isStrongPasswd( _passwuseruffer))
    {
        printf("Error! The password is not strong enough!\n");
        goto passwd_ask;
    }

    pts_ask:
    printf("Enter ASM point [0-500] : ");
    scanf(" %d", &_asmPtsBuffer);
    if( _asmPtsBuffer > 500 || _asmPtsBuffer < 0 )
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
    stringCopy( user[totalUser-1].name, _nameBuffer);
    stringCopy( user[totalUser-1].email, _mailBuffer);
    stringCopy( user[totalUser-1].password, _passwuseruffer);
    user[totalUser-1].ASMpts = _asmPtsBuffer;
    // replace "\0" with "-\0" to show more readable format 
    stringCopy( user[totalUser].transacMsg, "-");  

    fptr = fopen( filePath, "ab");
    if( fptr == NULL) 
    {
        perror("Error opening file to append!");
        doExit(1);
    }
    size_t elementWritten = fwrite( &user[totalUser-1], sizeof(User), 1, fptr);
    if( elementWritten != 1)
    {
        perror("Error writing created user to the file!");
        doExit(1);
    }
    fclose(fptr);

    printf("Your account has been successfully created!\n");
}

int findMaxId( User* user, int totalUser) 
{
    int maxId = -1;
    // 2 must be the maxId when there is no user created
    for( int i = 2; i < totalUser; i++) 
        if( user[i].id > maxId) 
            maxId = user[i].id;
    return maxId;
}

void readUserFromFile()
{
    fptr = fopen( filePath, "rb");
    if( fptr == NULL)
    {
        perror("Error opening the user database!");
        doExit(1);
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
    doExit(1);
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
        doExit(1);
    }
    perror("Error opening the user database for saving data!");
    doExit(1);
}

void deleteUser( int userId)
{   
    int deleteId;
    bool isThereUser = false;

    askDeleteId:
    printf("Enter the id of the user to delete : ");
    scanf( "%d", &deleteId);

    if( deleteId >= 1 && deleteId <= 3 )
    {
        printf( "Sorry, the id is not avaialable to delete.\n");
        goto askDeleteId;
    }

    // counting from 3 to skip admin acc and accelerate searching
    for( int i = 3; i != totalUser; i++)   
    {
        if( user[i].id == deleteId)
        {
            isThereUser = true;
            break;
        }
    }
    if( isThereUser == false)
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
    // logout(deleteId);  
    // overwrite the element to remove, with the last element
    user[deleteId] = user[totalUser-1];
    
    // resizing the user 
    totalUser--;
    User * newUser = (User*)malloc(sizeof(User) * totalUser);
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
    startMenu();
}

void searchUser()
{
    int id;
    bool found = false;
    askIdToSearch:
    printf("Enter the id number you want to search : ");
    scanf( "%d", &id);
    if( id < 1 || id > totalUser)
    {
        printf("Invalid user ID!");
        goto askIdToSearch;
    }
    for( int i = 0; user[i].email[0] != '\0'; i++)
    {
        if( user[i].id == id)
        {
            found = true;
            break;
        }
    }
    if( found == true)
    {
        showProfile(id);
    }
    else
    {
        printf("No user with such ID is found");
    }
    return ;
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

void startMenu()
{
    int _command = 0; // to get the sign in/up decision
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
            doExit(1);
            break;
    };
}

void userMenu( int id)
{
    int _command = 0; // to get the app in menu decision
    printf("\n1. Profile\n");
    printf("2. Give ASM Points\n");
    printf("3. Log Out\n");
    printf("4. Log Out & Exit\n");
    scanf("%d", &_command);

    switch(_command)
    {
        case 1:
            showProfile( id);
            userMenu( id);
            break;
        case 2:
            ptsProcess( id);
            userMenu( id);
            break;
        case 3:
            // logout(id);
            startMenu();
            break;
        case 4:
            // logout(id);
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
    printf("\n1. Profile\n");
    printf("2. Delete User\n");
    printf("3. Search User\n");
    printf("4. Log Out\n");
    printf("5. Log Out & Exit\n");
    scanf("%d", &_command);

    switch(_command)
    {
        case 1:
            showProfile( id);
            adminMenu( id);
            break;
        case 2:
            deleteUser(id);
            adminMenu(id);
            break;
        case 3:
            searchUser(id);
            adminMenu(id);
            break;
        case 4:
            // logout(id);
            startMenu();
            break;
        case 5:
            // logout(id);
            doExit(0);
            break;
        default:
            break;
    };
    adminMenu(id);
}

void showProfile( int id)
{
    printf("User ID         : %d\n", user[id-1].id);
    printf("User Name       : %s\n", user[id-1].name);
    printf("User Email      : %s\n", user[id-1].email);
    printf("ASM Balance     : %d\n", user[id-1].ASMpts);
    printf("Transac Message : %s\n", user[id-1].transacMsg);
}

// void logout( int id)
// {
//     int i = 0;
//     while( onlineUserIDs[i] != id )
//     {
//         i++;
//     }
//     while( onlineUserIDs[i] != '\0')  // Shift the array later than the logout id
//     {
//         onlineUserIDs[i] = onlineUserIDs[i+1];
//         i++;
//     }  // '\0' is also appended this way to end the array index 
// }

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

void ptsProcess( int giveId)
{
    int receiveId, pts = 0;
    askreceiveId:
    printf("Enter the user id you want to give : ");
    scanf(" %d", &receiveId);
    if( receiveId > totalUser || receiveId < 4)  // normal ID must be 4-100
    {
        printf("Error! Invalid user id.\n");
        goto askreceiveId;
    }
    for( int i = 3; user[i].email[0] != '\0'; i++)  // more accurate to find with string initialized '\0' 
    {
        if( user[i].id == receiveId)
            goto askPts;
    }
    // when user is not found ( may be skipped by branching askPts above)
    printf("Error! The user does not exist.\n");
    goto askreceiveId;

    askPts:
    printf("Enter ASM amount to give : ");
    scanf(" %d", &pts);
    if( pts > user[giveId-1].ASMpts)
    {
        printf("Error! You cannot give more than you have\n");
        goto askPts;
    }

    ptsGive( giveId, receiveId, pts);
    
    // transac = (Transac*)malloc(sizeof(Transac));
    // if( transac != NULL)
    // {
    //     // else?
    //     transac -> from = giveId;
    //     transac -> to = receiveId;
    //     transac -> amount = pts;
    // }  
    // else 
    // {
    //     printf("Error! Memory allocation for transaction failed.\n");
    //     doExit(1);
    // }

    // apply the changes (file needs to be exclusively controlled)
    writeUserToFile();

    printf("You have given %d ASM pts to user %d successfully!\n", pts, receiveId);
}

void ptsGive( int giveId, int receiveId, int pts)  // both id and pts must hvae been validated
{
    int i = 3;
    char msgBuffer[40] = "\0";
    // start with deduction pts from giverpoints
    while( user[i].id != giveId)  // as long as the id doesn't match.
    {
        i++;
    }
    user[i].ASMpts -= pts;
    snprintf( msgBuffer, sizeof(msgBuffer), "%d points given to user-%d", pts, receiveId);
    stringCopy( user[i].transacMsg, msgBuffer);

    i = 3;  // start agian for receiver
    while( user[i].id != receiveId)  // as long as the id doesn't match.
    {
        i++;
    }
    user[i].ASMpts += pts;
    snprintf( msgBuffer, sizeof(msgBuffer), "%d points received from user-%d", pts, giveId);
    stringCopy( user[i].transacMsg, msgBuffer);
}

void createAdminAcc()
{
    totalUser = 3;  
    user = (User*) realloc( user, sizeof(User) * 3);
    /*
        This way looks more neat for less data.
        Creating arrays may impact the loading time
    */
    user[0].id = 1;
    user[1].id = 2;
    user[2].id = 3;
    stringCopy( user[0].name, "adm1");
    stringCopy( user[1].name, "adm2");
    stringCopy( user[2].name, "adm3");
    stringCopy( user[0].email, "adm1@gmail.com");
    stringCopy( user[1].email, "adm2@gmail.com");
    stringCopy( user[2].email, "adm3@gmail.com");
    stringCopy( user[0].password, "Adm#1");
    stringCopy( user[1].password, "Adm#2");
    stringCopy( user[2].password, "Adm#3");
    user[0].ASMpts = 0;
    user[1].ASMpts = 0;
    user[2].ASMpts = 0;
    stringCopy( user[0].transacMsg, "-");
    stringCopy( user[1].transacMsg, "-");
    stringCopy( user[2].transacMsg, "-");
}

void doExit( int x)  
{
    // program may crash upon failing the following functions again and again
    writeUserToFile();
    free(user);
    printf("Exiting the program.\n");
    exit(x);
}

