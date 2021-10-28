// Sean King: Operating Systems -- Producer & Conusmer Problem
// Consumer Implementation

/* Program links together the 'producer.c' file and the 'consumer.c'
file using shared memory and produces two items onto a 'table' and
then consumes the two items from the table using semaphores so there
are no items left in the table. This process then repeats twenty-four
times. */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>


int main() {

	// Opening and allocating the shared memory between the Producer and Consumer.
	int Table = shm_open("Table", O_RDWR, 0666);
	int* tbl = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, Table, 0);
	//////////////////////////////////////////////////////////////////////////////////

	// Initializing the semaphores that will consume the items from the table.
	sem_t* full = sem_open("full", O_CREAT, 0666, 0);
	sem_t* open = sem_open("open", O_CREAT, 0666, 2);
    sem_t* mutex = sem_open("mutex", O_CREAT, 0666, 1);
	////////////////////////////////////////////////////

	// Setting up the program to run five times.
	int ROUNDS = 5;
	printf("\nThe Consumer is ready to consume.\n");

	for (int i = 0; i < ROUNDS; ++i) {

		// Waiting for the table to be full to consume items.
		sem_wait(full);
		////////////////

	    // Generating an item to take from the table.
		int j = rand() % 2 + 1;
		sleep(j);
		////////////////////////

        // Takes an item off of the table. (Consuming)
        sem_wait(mutex);
		--(*tbl);
        sem_post(mutex);
		/////////////////

		// Saying that an item has been consumed and signals the Producer.
		printf("An item has been consumed, there are now %d item(s) on the table.\n", *tbl);
		sem_post(open);
		/////////////////////////////////////////////////////////////////////////////////////
	}
	///////////////////////////////////////////////

	// Closes the Semaphores.
	sem_close(full);
	sem_close(open);
    sem_close(mutex);
	//////////////////

	// Unlinks the semaphores.
	sem_unlink("full");
	sem_unlink("open");
    sem_unlink("mutex");
	/////////////////////

	// Closes and unlinks the shared memory between the Producer and Consumer.
	munmap(tbl, sizeof(int));
	close(Table);
	shm_unlink("tbl");
	//////////////////////////

	// Saying the Consumer has been cleaned up and will no longer consume.
	printf("The Consumer has been cleaned up.\n");
	return 0;
	///////////////////////////////////////////////
}