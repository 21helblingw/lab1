# lab1
## Program Overview
This project is the Operating System lab 1. It consists of two seperate processes that share a portion of memory. These two porcesses are the producer and the consumer. The producer produces 5 numbers starting at 0 and goes to 4. The consumer takes these numbers from the shared memory and prints them to the screen. The buffer size of the shared memory is 2 items. The producer has to run first in order for the program to work.
## Installations Needed To Run The Programs
### Recommend using Linux Machine to run  
This project uses POSIX pthreads. POSIX is included with Linux. If you want to use a Windows Machine, you would need to install POSIX library
### Installing compiling packages
Need gcc to complie the c code
First need to make sure packages are up to date
```bash
sudo apt update
```
Now we can download the compliling packages with the meta package build-essential. This is install gcc and g++ compliers for c/c++.
```bash
sudo apt install build-essential
```

## Usage
```bash
gcc producer.c -pthread -lrt -o producer
gcc consumer.c -pthread -lrt -o consumer
./producer & ./consumer &
```
## Documentation
### Shared Memory
The shared memory is created in /dev/shm/ with the name sharedMem. Creating the shared memory also returns a file descriptor, which is used to put the buffer structure in the shared memory. The file descriptor is truncated to the size of the buffer structure and mapped to a pointer called shm_map, which has the ability to read and write to the shared memory. This procedure is done to both the producer and the consumer. Both can create the shared memory or open it if it was already created.
### Process Synchronization 
Semaphores are used to synchronize the two processes. There is one shared semaphore located in the shared memory that both processes can read and write to. The semaphore is initialized to 0 in the producer so the producer has to run first so the consumer does not reference the semaphore before its creation. It is initialized to 0 because the consumer waits before accessing the shared memory, while the producer does not have to wait to initialize the variables in the shared memory. After the initialization of the variables, the producer does wait before accessing the shared memory. This ensures that the process goes into its critical section first. 
### Critical Sections
The producer has two different goals in its critical section. The first is to initialize the variables in the shared memory and the other goal is to add numbers into the buffer to be consumed. The initialization of the variables is always the first access for the critical sections from either the producer and consumer. It sets the in pointer, out pointer, and buffer count to 0. The main critical section, which is adding numbers to the buffer, has two main parts. It checks if the buffer is full. If the buffer is full, it sleeps for a small time, then leaves the critical section. This gives the consumer time to remove items from the buffer. If there is space in the buffer, it adds the vaule



The producer program should always run first because it initializes the shared memory and initializes the semaphore used to ensure synchronization. The producer also is the first to access the shared memory and sets the in, out, and buffer variables to 0. These are used to know where and how many items are in the buffer. The producer and the consumer both print when they are waiting to enter their critical section and when they are leaving their critical sections. The producer also prints what it is producing and the consumer prints what is consumed from the shared memory. 
## Example
```code
starting process P
P in Critial to start and init variables
... P Leaving Critial from start
P is waiting on critial section
	P-> adding 0 to buffer
starting process C
C: created the buffer map
P is waiting on critial section
	P-> adding 1 to buffer
C is waiting on critial section
	C-> printing: 0
P is waiting on critial section
	P-> adding 2 to buffer
C is waiting on critial section
	C-> printing: 1
P is waiting on critial section
	P-> adding 3 to buffer
C is waiting on critial section
	C-> printing: 2
P is waiting on critial section
	P-> adding 4 to buffer
exiting process p
C is waiting on critial section
	C-> printing: 3
C is waiting on critial section
	C-> printing: 4
exiting process c
```
