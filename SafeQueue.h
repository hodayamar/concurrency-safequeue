#ifndef EXCELLENTEAM_ELLA_CONCURRENCY_SAFEQUEUE_HODAYAMAR_SAFEQUEUE_H
#define EXCELLENTEAM_ELLA_CONCURRENCY_SAFEQUEUE_HODAYAMAR_SAFEQUEUE_H

#include <iostream>         //std
#include <semaphore.h>
#include <queue> // std::queue
#include <unistd.h> // sleep

template< class T, size_t SIZE = 3 >
class SafeQueue
{
public:

    SafeQueue();
    ~SafeQueue();
    T pop();
    void push( const T& );

private:

    std::queue< T > m_safeQueue;
    sem_t semPop;
    sem_t semPush;
    pthread_mutex_t mtx;

    void initSync();
    void deleteSync();
};


template< class T, size_t SIZE >
SafeQueue< T, SIZE >::SafeQueue()
{
    initSync();
}

template< class T, size_t SIZE >
SafeQueue< T, SIZE >::~SafeQueue()
{
    deleteSync();
}

template< class T, size_t SIZE >
void SafeQueue< T, SIZE >::initSync()
{
    sem_init( &semPush, 0, SIZE );
    sem_init( &semPop, 0, 0 );
    if ( pthread_mutex_init ( &mtx, NULL ) != 0 )
    {
        std::cout << "\n mutex init failed\n" << std::endl;
        deleteSync();
    }
}

template< class T, size_t SIZE >
void SafeQueue< T, SIZE >::deleteSync()

{
    sem_destroy( &semPush );
    sem_destroy( &semPop );
    pthread_mutex_destroy( &mtx );
}

template< class T, size_t SIZE >
T SafeQueue< T, SIZE >::pop()
{
    T tmp;

    sem_wait( &semPop );

    pthread_mutex_lock( &mtx );

    tmp = m_safeQueue.front();
    m_safeQueue.pop();
    std::cout << "Extract ---  " << tmp << std::endl;
    sleep(1);


    pthread_mutex_unlock( &mtx );

    sem_post( &semPush );

    return tmp;
}


template< class T, size_t SIZE >
void SafeQueue< T, SIZE >::push( const T& elementToPush )
{

    sem_wait( &semPush );

    pthread_mutex_lock( &mtx );

    m_safeQueue.push( elementToPush );
    std::cout << "Push ---- " << elementToPush << std::endl;
    sleep(1);


    pthread_mutex_unlock( &mtx );

    sem_post( &semPop );

}

#endif //EXCELLENTEAM_ELLA_CONCURRENCY_SAFEQUEUE_HODAYAMAR_SAFEQUEUE_H
