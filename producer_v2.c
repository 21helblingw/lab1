#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

struct buffer {
    sem_t sem_pro; // semaphore for producer
    sem_t sem_con; // semaphore for consumer
    size_t in; // points to the first empty space in buffer
    size_t out; // points to the first full space in buffer
    size_t buffer; // amount of items in the buffer
    int buf[2]; // array to hold the items (can only hold 2)

};

int main(){
    printf("starting process P\n");
    // creates the shared memory location and returns the file discripter for the shared memory
    int fd = shm_open("/sharedMem",O_CREAT | O_RDWR , S_IRUSR | S_IWUSR);
    if (fd == -1) printf("FAILED FD\n");
    // checks to make sure the fd is created properly
    if(ftruncate(fd, sizeof(struct buffer)) == -1) printf("FAILED TRUNCATE\n");
   
    // maps shm_map, which is the buffer structure, to the shared memory
    struct buffer *shm_map = mmap(NULL, sizeof(*shm_map), PROT_READ | PROT_WRITE, MAP_SHARED, fd,0);
    // checks to make sure the mapping worked
    if(shm_map == MAP_FAILED){
        printf("FAILED MAPPING\n");
        exit(1);
    }
    // creates the two semaphores for the processes, both set to 0.
    sem_init(&shm_map->sem_pro, 1, 0);
    sem_init(&shm_map->sem_con, 1, 0); 
    

    // first critical section to init varaibles
    // this also posts the producer semaphore, allowing it to start producing
    printf("P in Critical to start and init variables\n");

    shm_map->in = 0;
    shm_map->out = 0;
    shm_map->buffer=0; // amout of items in array

    printf("... P Leaving Critical from start\n");
    sem_post(&shm_map->sem_pro); // tells the producer that it can use shared mem to produce items

    int counter = 0;
    while (counter < 5){
        printf("P is waiting on critical section\n");
        sem_wait(&shm_map->sem_pro); // waits until it can access the shared memory(consumer has finsihed its critical section)
        //in critial section

        // if there is space in the buffer
        // adds the coutner to the buffer in shared memory
        // increases the buffer size, in , and coutner by one. it in points to the next free spot
        if(shm_map->buffer < 2){
            printf("\tP-> adding %d", counter);
            printf(" to buffer\n");
            shm_map->buf[shm_map->in % 2] = counter;
            ++shm_map->in;
            ++shm_map->buffer;
            ++counter;
        }
        // buffer is full
        else{
            printf("\tP-> Buffer is full\n");
        }
    printf("... P Leaving Critical from store mem\n");
    sem_post(&shm_map->sem_con); // tells the consumer that there are items in buffer
    }

    shm_unlink("/sharedMem");
    printf("exiting process p\n");
    exit(0);

}