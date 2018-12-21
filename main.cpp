#include <stdlib.h>
#include <pthread.h>

#include "SafeQueue.h"

#define NUM_OF_THREAD 4


void* Poping( void* queue )
{
    while( true )

        ( ( SafeQueue< int, 5 >* ) queue )-> pop();
}


void* Pushing( void* queue )
{
    int rand_a;
    while( true )
    {
        rand_a = rand();
        ( ( SafeQueue< int, 5 >* ) queue )-> push( rand_a );
    }
}

void* randomPushPop( void* queue )
{
    int randPopPush, numToInsert, popNum;

    while( true )
    {
        randPopPush = rand();

        if( randPopPush < 1000 )
        {
            numToInsert = rand();
            ( ( SafeQueue< int, 5 >* ) queue )-> push( randPopPush );
            std::cout << "Push ---- " << numToInsert << std::endl;
        }

        {
            popNum = ( ( SafeQueue< int, 5 >* ) queue )-> pop();
            std::cout << "Push ---- " << popNum << std::endl;
        }

    }
}

int main()
{
    SafeQueue< int, 10 > mySafeQueue;

    pthread_t threads [ NUM_OF_THREAD ], threadForPush, threadForPop;

    pthread_create( &threadForPush, NULL, Pushing, ( void* )&mySafeQueue );
    pthread_create( &threadForPop, NULL, Poping, ( void* )&mySafeQueue );

    for ( int i = 0 ; i < NUM_OF_THREAD ; i++ )

        pthread_create( &threads[i], NULL, randomPushPop, ( void* )&mySafeQueue );

    for ( int i = 0 ; i < NUM_OF_THREAD ; i++ )

        pthread_join( threads[i], NULL );



    pthread_join( threadForPop,NULL );
    pthread_join( threadForPush, NULL );

    return 0;
}