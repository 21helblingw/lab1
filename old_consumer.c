#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

struct buffer {
    sem_t sem_pro;
    sem_t sem_con;
    size_t in;
    size_t out;
    size_t buffer;
    int buf[2];

};

int main(){
    sleep(1);
    printf("starting process C\n");

    int fd = shm_open("/sharedMem",O_RDWR , S_IRUSR | S_IWUSR);
    if (fd == -1) printf("FAILED FD\n");

    if(ftruncate(fd, sizeof(struct buffer)) == -1) printf("FAILED TRUNCATE\n");
    //printf("C: created the fd\n");

    struct buffer *shm_map = mmap(NULL, sizeof(*shm_map), PROT_READ | PROT_WRITE, MAP_SHARED, fd,0);
    //printf("C: created the buffer map\n");
    if(shm_map == MAP_FAILED){
        printf("FAILED MAPPING\n");
        exit(1);
    }

     // set to 0, waits for the producer to go first
    //printf("C: created the sem_con\n");
    // tells the producer that it is ready to start consuming
    int counter = 0;
    int flag = 0;
    while(counter < 5 && flag < 10){
        printf("C is waiting on critial section\n");
        sem_wait(&shm_map->sem_con);
        // in critial section
        printf("C in Critial\n");


        if(shm_map->buffer > 0 && shm_map->buffer < 3){
            int temp = shm_map->buf[shm_map->out % 2];
            shm_map->buf[shm_map->out % 2] = 0;
            printf("\tC-> printing: %d\n",temp);
            ++shm_map->out;
            --shm_map->buffer;
            ++counter;
        }
        else{
            printf("\tC-> nothing to consume\n");
            ++flag;
        }
        printf("...C Leaving Critial\n");
        sem_post(&shm_map->sem_pro);
    }

    shm_unlink("/sharedMem");
    printf("exiting process c\n");
    exit(0);
    

}