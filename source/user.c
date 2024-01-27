#define Max_User 100

typedef struct{
    int id;
    char name[20];
    char email[41];
    char password[30];
    int ASMpts;
}User;

User user[Max_User];  // prepare the user list
int onlineUserIDs[Max_User];  // to store the id of online user (needs concurrency control)

