#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "user.c"


char special[] = { '\"', '\'', '~', '`', '@', '#', '$', '%', '^', '&', 
                    '*', '(', ')', '-', '_', '+', '=', '<', '>', ',', 
                    '.', '?', '/', '\\', '|', '[', ']', '{', '}'};

FILE * fptr;
const char * filePath = "../data/user.bin";

void sign_in_page();
void registration();
void saveUserData2File();
void userDelete( int id);  // id needed to know if the user is deleting its own or not
void string_copy( char [], char []);
void start_menu();
void app_in_menu( int);

void showProfile( int id);
void logout( int);

bool compare_string( char [], char []);
bool isStrongPasswd( char []);
bool isValidEmail( char []);

void ptsProcess( int GiveId);
void ptsGive( int GiveId, int ReceiveId, int pts);

