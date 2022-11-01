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
    // makes sure that the producer runs first
    sleep(1);
    printf("starting process C\n");
    // opens the shared memory location and returns the file discripter for the shared memory
    int fd = shm_open("/sharedMem",O_RDWR , S_IRUSR | S_IWUSR);
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

    
    int counter = 0;
    while(counter < 5){
        printf("C is waiting on critical section\n");
        sem_wait(&shm_map->sem_con);  // waits until it can access the shared memory(producer has finsihed its critical section)
        // in critial section

        // if the buffer is not empty or full
        // consume and print an item in the buffer and replace it with 0
        // decrease the buffer item counter and increase the out so it points to the next possible full item
        if(shm_map->buffer > 0 && shm_map->buffer < 3){
            int temp = shm_map->buf[shm_map->out % 2];
            shm_map->buf[shm_map->out % 2] = 0;
            printf("\tC-> printing: %d\n",temp);
            ++shm_map->out;
            --shm_map->buffer;
            ++counter;
        }
        // buffer is empty
        else{
            printf("\tC-> nothing to consume\n");
        }
        printf("...C Leaving Critical\n");
        sem_post(&shm_map->sem_pro); // tells the producer that it can enter its critical section
    }

    shm_unlink("/sharedMem");
    printf("exiting process c\n");
    exit(0);
    

}