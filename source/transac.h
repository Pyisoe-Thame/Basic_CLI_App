#ifndef TRANSAC_H
#define TRANSAC_H

#define MAX_TRANSACTION 100

#include <time.h>

typedef struct{
    int from;
    int to;
    int amount;
/*
    no dynamic data (and pointer) is recommended to store into binary file
    time_info's fields; tm_mday, tm_yday and tm_isdst are less likely to be used but,
    extracting others and storing into the structure involves more processing time so,
    less runtime overhead at the cost of more memory usage. 
*/
    struct tm time_info;  
}Transac;

void addTransac( Transac transac[MAX_TRANSACTION], int giveId, int receiveId, int pts);

void addTransac( Transac transac[MAX_TRANSACTION], int giveId, int receiveId, int pts)
{
    int i = 0;
    time_t current_time;
    time(&current_time);  // get the current time stamp

    // push i to the last transaction entry index
    while( transac[i].from != 0 && i < MAX_TRANSACTION)  
        i++;
    if( i != MAX_TRANSACTION)  // first case scenerio first
    {  
        transac[i].from = giveId;
        transac[i].to = receiveId;
        transac[i].amount = pts;
        transac[i].time_info = *localtime(&current_time);  // convert into local timezone 
        return ;
    }
    else
    {  // shift the transaction data and delete the oldest transaction record
        for( int j = 0; j < MAX_TRANSACTION-1; j++)
            transac[j] = transac[j+1];
        transac[MAX_TRANSACTION-1].from = giveId;
        transac[MAX_TRANSACTION-1].to = receiveId;
        transac[MAX_TRANSACTION-1].amount = pts;
        transac[i].time_info = *localtime(&current_time);
        return ;
    }
    
}

#endif