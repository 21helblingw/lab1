#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

struct buffer {
    sem_t sem; // semaphore used for process syn
    size_t in; // points to the first empty space in the buffer
    size_t out; // points to the first full space in the buffer
    size_t buffer; // amount of items in the buffer
    int buf[2]; // array to hold the items (can only hold 2)

};

int main(){
    printf("starting process P\n");
    // creates the shared memory location and returns the file discripter for the shared memory
    int fd = shm_open("/sharedMem",O_CREAT | O_RDWR , S_IRUSR | S_IWUSR);
    // checks to make sure the fd is created properly
    if (fd == -1) printf("FAILED FD\n");
    // truncates the size of the file discripter to the size of the buffer structure
    if(ftruncate(fd, sizeof(struct buffer)) == -1) printf("FAILED TRUNCATE\n");

    // maps shm_map, which is the buffer structure , to the shared memory
    struct buffer *shm_map = mmap(NULL, sizeof(*shm_map), PROT_READ | PROT_WRITE, MAP_SHARED, fd,0);
    // checks to make sure the mapping worked
    if(shm_map == MAP_FAILED){
        printf("FAILED MAPPING\n");
        exit(1);
    }

    // creates the semaphore for the processes.
    // sets to 0. this allows allows the producer to run first
    // because the consumer waits while the producer posts after first access to their critial section
    sem_init(&shm_map->sem, 1, 0);
    //printf("P: created the sem_pro\n");
    

    //in first critical section to init varialbes
    // this also posts the semaphore, which allows the consumer to start as well
    printf("P in critical to start and initialize variables\n");
    // sets the variables to 0 
    shm_map->in = 0;
    shm_map->out = 0;
    shm_map->buffer=0; 

    printf("... P Leaving Critial from start\n");
    // tells the consumer that it can use shared mem or allows the the producer to go in its critical section again
    sem_post(&shm_map->sem);

    int counter = 0;
    while(counter < 5){
        // allows for the two processes to take turns in thir critical sections
        sleep(1);
        printf("P is waiting on critical section\n");
        sem_wait(&shm_map->sem); // waits unitl it can access the critical section
        //in critical section

        // if there is space in the buffer
        // adds the counter into the buffer in shared memory
        // increses the buffer size, coutner, and the in increases to point to the next free spot
        if(shm_map->buffer < 2){
            printf("\tP-> adding %d", counter);
            printf(" to buffer\n");
            shm_map->buf[shm_map->in % 2] = counter;
            ++shm_map->in;
            ++shm_map->buffer;
            ++counter;
        }
        else{
            // if the buffer is full, wait and check back later
            printf("\tP-> Buffer is full\n");
        }
    printf("... P Leaving Critial\n");
    sem_post(&shm_map->sem); //producer leaving critical section
    
    }

    shm_unlink("/sharedMem");
    printf("exiting process p\n");
    exit(0);

}