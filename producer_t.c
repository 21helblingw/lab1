#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

struct buffer {
    sem_t sem;
    size_t in;
    size_t out;
    size_t buffer;
    int buf[2];

};

int main(){
    printf("starting process P\n");

    int fd = shm_open("/sharedMem",O_CREAT | O_RDWR , S_IRUSR | S_IWUSR);
    if (fd == -1) printf("FAILED FD\n");

    if(ftruncate(fd, sizeof(struct buffer)) == -1) printf("FAILED TRUNCATE\n");
   //printf("P: created the fd\n");

    struct buffer *shm_map = mmap(NULL, sizeof(*shm_map), PROT_READ | PROT_WRITE, MAP_SHARED, fd,0);
    //printf("P: created the buffer map\n");
    if(shm_map == MAP_FAILED){
        printf("FAILED MAPPING\n");
        exit(1);
    }
    // creates the two semaphores for the processes.
    //producer will always to to CS first.
    sem_init(&shm_map->sem, 1, 1); // set to 0, waits for the consumer to connect to mem
    //printf("P: created the sem_pro\n");
    

    // waits until the consmuer maps to the shared memory and inits its sem_con
    sem_wait(&shm_map->sem);
    //in critial section
    printf("P in Critial to start and init variables\n");

    shm_map->in = 0;
    shm_map->out = 0;
    shm_map->buffer=0; // amout of items in array

    printf("... P Leaving Critial from start\n");
    sem_post(&shm_map->sem); // tells the consumer that it can use shared mem

    int counter = 0;
    int flag = 0;
    while(counter < 5){
        sleep(1);
        printf("P is waiting on critial section\n");
        sem_wait(&shm_map->sem);
        //in critial section
        //printf("P in Critial to store in mem\n");

        // if there is space in the buffer
        if(shm_map->buffer < 2){
            printf("\tP-> adding %d", counter);
            printf(" to buffer\n");
            shm_map->buf[shm_map->in % 2] = counter;
            ++shm_map->in;
            ++shm_map->buffer;
            //printf("\tP-> buffer value %zu\n",shm_map->buffer );
            ++counter;
        }
        else{
            sleep(1);
            ++flag;
        }
    //printf("... P Leaving Critial from store mem\n");
    sem_post(&shm_map->sem); 
    // tells the consumer that there are items in buffer
    }

    shm_unlink("/sharedMem");
    printf("exiting process p\n");
    exit(0);

}