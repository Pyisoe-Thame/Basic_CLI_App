#ifndef TRANSAC_H
#define TRANSAC_H

#define MAX_TRANSACTION 100

typedef struct{
    int from;
    int to;
    int amount;
    // char eventTime[30];
}Transac;

void addTransac( Transac transac[MAX_TRANSACTION], int giveId, int receiveId, int pts);

void addTransac( Transac transac[MAX_TRANSACTION], int giveId, int receiveId, int pts)
{
    int i = 0;
    while( transac[i].from != 0 && i < MAX_TRANSACTION)
        i++;
    if( i == MAX_TRANSACTION)
    {
        for( int j = 0; j < MAX_TRANSACTION-1; j++)
            transac[j] = transac[j+1];
        transac[MAX_TRANSACTION-1].from = giveId;
        transac[MAX_TRANSACTION-1].to = receiveId;
        transac[MAX_TRANSACTION-1].amount = pts;
        return ;
    }
    else
    {
        transac[i].from = giveId;
        transac[i].to = receiveId;
        transac[i].amount = pts;
        return ;
    }
}

#endif