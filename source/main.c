#include "main.h"

int main()
{
    fptr = fopen( filePath, "rb");
    if( fptr == NULL)
    {
        perror("Error opening the user database!");
        return 1;
    }
    size_t readCount = fread( user, sizeof(User), Max_User, fptr);  // accumulate the user number
    fclose(fptr);

    while(1)
    {
        start_menu();
    }
}

void sign_in_page()
{
    char _mailBuffer[50];  // to store mail temporarily during input process
    char _passwdBuffer[30];  // to store password temporarily during input process
    int id = 0;  // to store the id of the user who is logging in
    bool mailExist = false;
    printf("Enter email : ");
    scanf(" %[^'\n']", &_mailBuffer);

    for( int i = 0; user[i].id != 0; i++)
    {
        if( compare_string( _mailBuffer, user[i].email) == true)
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
        if( compare_string( _passwdBuffer, user[id-1].password) == 1)
        {
            int j = 0;
            while( onlineUserIDs[j] != '\0')
                j++;
            onlineUserIDs[j] = id;  // assign the user id to the online user list
            app_in_menu(id);
        }
        else
        {
            printf("Sorry, the password you entered is invalid!\n");
            start_menu();
        }
    }
    else
    {
        printf("Sorry, the email you entered is not registered!\n");
        start_menu();
    }

}

void registration()
{
    char _nameBuffer[20];  // to store mail temporarily during input process
    char _mailBuffer[50];  // to store mail temporarily during input process
    char _passwuseruffer[30];  // to store password temporarily during input process
    int _asmPtsBuffer = 0;  // to store the ASM point on account creation
    int i = 0;

    while(user[i].id != '\0')
    {
        i++;
    };
    
    user[i].id = i+1; // assign the id ( id starts at 1 and array start at 0)

    printf("Enter user name : ");
    scanf(" %[^'\n']", &_nameBuffer);
    string_copy( user[i].name, _nameBuffer);

    email_ask:
    printf("Enter a valid email : ");
    scanf(" %[^'\n']", &_mailBuffer);
    if(!isValidEmail(_mailBuffer))  // check if valid email is entered
    {
        printf("Error! Invalid email entered.\n");
        goto email_ask;
    }
    for( int j = 0; j < Max_User; j++)
    {
        if( compare_string( _mailBuffer, user[j].email) == true)
        {
            printf("Sorry, this email has been connected to another account!\n");
            goto email_ask;
        }
        else if( user[j].id == '\0')  // end the loop early if there are no longer user
            break;
    }
    string_copy( user[i].email, _mailBuffer);

    passwd_ask:
    printf("Enter password : ");
    scanf(" %[^'\n']", &_passwuseruffer);
    if( !isStrongPasswd( _passwuseruffer))
    {
        printf("Error! The password is not strong enough!\n");
        goto passwd_ask;
    }
    string_copy( user[i].password, _passwuseruffer);

    pts_ask:
    printf("Enter ASM point [0-500] : ");
    scanf(" %d", &_asmPtsBuffer);
    if( _asmPtsBuffer > 500 || _asmPtsBuffer < 0 )
    {
        printf("Error! Invalid amount of ASM point. \n");
        goto pts_ask;
    }
    user[i].ASMpts = _asmPtsBuffer;

    fptr = fopen( filePath, "ab");
    if( fptr == NULL) 
    {
        perror("Error opening file to append!");
        exit(1);
    }
    size_t elementWritten = fwrite( &user[i], sizeof(user[i]), 1, fptr);
    if( elementWritten != 1)
    {
        perror("Error writing created user to the file!");
        exit(1);
    }
    fclose(fptr);

    printf("Your account has been successfully created!\n");
}

void saveUserData2File()
{
    int i = 0;
    while( user[i].email[0] != '\0' && i < Max_User)  // easier to find the end of the data with email being null char
    {
        i++;
    };  // to fetch only the necessary amount of user
    fptr = fopen( filePath, "wb");  // overwrite the data every time the data is updated.
    if( fptr == NULL)
    {
        perror("Error opening the user database for saving data!");
        exit(1);
    }
    size_t writtenUserAmount = fwrite( &user[0], sizeof(user), i, fptr);
}

void userDelete( int userId)
{   
    int deleteId;
    bool isThereUser = false;
    printf("Enter the id of the user to delete : ");
    scanf( "%d", &deleteId);

    for( int i = 0; user[i].email[0] != '\0'; i++)
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
    logout(deleteId);  

    int i = 0;
    while( user[i].id != deleteId)
    {
        i++;
    };
    while( user[i].id != 0 || i != Max_User-1)
    {
        user[i].id = user[i+1].id;
        string_copy( user[i].name, user[i+1].name);
        string_copy( user[i].email, user[i+1].email);
        string_copy( user[i].password, user[i+1].password);
        user[i].ASMpts = user[i+1].ASMpts;
        i++;
    }
    saveUserData2File();
    if( deleteId == userId)
        start_menu();
}

void string_copy( char dest[], char buffer[])  // copy from buffer array to destination array
{
    int i = 0;  // to index the arrays
    while( buffer[i] != '\0')  // as long as the content is not a null character
    {
        dest[i] = buffer[i];
        i++;
    };  
    dest[i] = '\0';  // add null character manually to set the end of destination array
}

void start_menu()
{
    int _command = 0; // to get the sign in/up decision
    printf("1. Sign in \n");
    printf("2. Sign up \n");
    printf("3. Exit \n");
    scanf("%d", &_command);

    switch(_command)
    {
        case 1:
            sign_in_page();
            break;
        case 2:
            registration();
            start_menu();
            break;
        case 3:
            exit(0);
            break;
        default:
            perror("Error! Unspported input detected!\n");
            perror("Ending the program...\n");
            exit(1);
            break;
    };
}

void app_in_menu( int id)
{
    int _command = 0; // to get the app in menu decision
    printf("\nHello, This is the menu inside account.\n");
    printf("1. Profile\n");
    printf("2. Give ASM Points\n");
    printf("3. Log Out\n");
    printf("4. Log Out & Exit\n");
    if( ( id >= 1) && ( id <= 3))
        printf("5. Deleter User\n");
    scanf("%d", &_command);

    switch(_command)
    {
        case 1:
            showProfile( id);
            app_in_menu( id);
            break;
        case 2:
            ptsProcess( id);
            app_in_menu( id);
            break;
        case 3:
            logout(id);
            start_menu();
            break;
        case 4:
            logout(id);
            exit(0);
            break;
        default:
            break;
    };
    if( ( id >= 1) && ( id <= 3) && _command == 5)
    {
        userDelete(id);
        app_in_menu(id);
    }
    else
    {
        perror("Error! Unspported input detected!\n");
        perror("Ending the program...\n");
        exit(1);
    }
}

void showProfile( int id)
{
    printf("User ID     : %d\n",user[id-1].id);
    printf("User Name   : %s\n",user[id-1].name);
    printf("User Email  : %s\n",user[id-1].email);
    printf("ASM Balance : %d\n",user[id-1].ASMpts);
}

void logout( int id)
{
    int i = 0;
    while( onlineUserIDs[i] != id )
    {
        i++;
    }
    while( onlineUserIDs[i] != '\0')  // Shift the array later than the logout id
    {
        onlineUserIDs[i] = onlineUserIDs[i+1];
        i++;
    }  // '\0' is also appended this way to end the array index 
}

bool compare_string( char buffer[], char key[])
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
    askReceiveId:
    printf("Enter the user id you want to give : ");
    scanf(" %d", &receiveId);
    if( receiveId > Max_User)
    {
        printf("Error! Invalid user id.\n");
        goto askReceiveId;
    }
    for( int i = 0; user[i].id != '\0'; i++)
    {
        if( user[i].id == receiveId)
            goto askPts;
    }
    // when user is not found
    printf("Error! The user does not exist.\n");
    goto askReceiveId;

    askPts:
    printf("Enter ASM amount to give : ");
    scanf(" %d", &pts);
    if( pts > user[giveId-1].ASMpts)
    {
        printf("Error! You cannot give more than you have\n");
        goto askPts;
    }

    ptsGive( giveId, receiveId, pts);

    // apply the changes (file needs to be exclusively controlled)
    saveUserData2File();

    printf("You have given %d ASM pts to user %d successfully!\n", pts, receiveId);
}

void ptsGive( int GiveId, int ReceiveId, int pts)  // both id and pts must hvae been validated
{
    int i = 0;
    // start with deduction pts from giver
    while( user[i].id != GiveId)  // as long as the id doesn't match.
    {
        i++;
    }
    user[i].ASMpts -= pts;

    i = 0;  // start agian for receiver
    while( user[i].id != ReceiveId)  // as long as the id doesn't match.
    {
        i++;
    }
    user[i].ASMpts += pts;
}

