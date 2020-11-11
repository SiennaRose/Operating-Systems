/*
 * The code is not part of the real application, and just used to 
 * illustrate the bounded-buffer problem using Semaphore and/or mutexes. 
 * Detailed requirements please refer to the assignment documentation.
 * 
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <semaphore.h>
#include "bbuffer.h"

sem_t mutex;	//used to lock shared resource, buffer, so other threads can't access at the same time
sem_t empty;	//used to keep track of how many empty buffer slots there are
sem_t full;	//used to keep track of how many full buffer slots there are

void initilization()
{
	sem_init(&empty, 0, BUFFER_SIZE);  //initialize empty semaphore to be shared by all threads and set to the value of the buffer's max size
	sem_init(&full, 0, 0); 	//initialize full semaphore to be shared by all threads and set to 0
	sem_init(&mutex, 0, 1); //initialize mutex semaphore to be shared by all threads and set to the value of 1
}

/* *
 * insert_item - thread safe(?) function to insert items to the bounded buffer
 * @param item the value to be inserted
 * @return 0 in case of sucess -1 otherwise
 */
  
int insert_item(int item, long int id)
{
     
    buffer.value[buffer.next_in] = item;
    
      
    printf("producer %ld: inserted item %d into buffer index %d\n", id, item, buffer.next_in);
       
    buffer.next_in = (buffer.next_in + 1) % BUFFER_SIZE;
        
    return 0;
}

/**
 * remove_item - thread safe(?) function to remove items to the bounded buffer
 * @param item the address of the variable that the removed value will be written
 * @return 0 in case of sucess -1 otherwise
 */
int remove_item(int *item, long int id)
{
    
    *item = buffer.value[buffer.next_out];
    buffer.value[buffer.next_out] = -1;    
    
   
    printf("consumer %ld: removed item %d from buffer index %d\n", id, *item, buffer.next_out);
       
    buffer.next_out = (buffer.next_out + 1) % BUFFER_SIZE;
    
    return 0;
}

/**
 * producer - will iterate PRODUCER_ITERATION times and call the corresponding
 * function to insert an integer to the bounded buffer
 * @param param an integer id of the producer used to distinguish between the
 * multiple producer threads
 * @return nothing
 */
void * producer(void *param)
{
    int item, i;
    long int id = (long int)param;

    printf("producer %d started!\n", id);
    i = PRODUCER_ITERATIONS;
    while (i--) {
	  sleep(rand() % 3);

	item = rand() % 10000;

	//start of critical section
	sem_wait(&empty);	//check if buffer is empty, if not wait
	sem_wait(&mutex);	//lock buffer so no other threads access it
	if (insert_item(item, id))
	    printf("Error while inserting to buffer\n");
	sem_post(&mutex);	//unlock buffer
	sem_post(&full); 	//let other thread know data has been inserted into buffer
	//end of critical section
    }

    pthread_exit(0);
}

/**
 * consumer - will iterate CONSUMER_ITERATION times and call the corresponding
 * function to remove an integer from the bounded buffer
 * @param param an integer id of the producer used to distinguish between the
 * multiple consumer threads
 * @return nothing
 */
void * consumer(void *param)
{
    int item, i;
    long int id = (long int)param;

    printf("consumer %d started!\n", id);
    i = CONSUMER_ITERATIONS;
    while (i--) {
	sleep(rand() % 3);

	//start of critical section
	sem_wait(&full);	//check if buffer is full, if not wait
	sem_wait(&mutex);	//lock buffer so no other threads access it
	if (remove_item(&item, id))
	    printf("Error while removing from buffer\n");
	sem_post(&mutex);	//unlock buffer
	sem_post(&empty); 	//let other threads know data has been removed from buffer
	//end of critical section
    }

    pthread_exit(0);
}
