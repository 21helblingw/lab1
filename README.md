# lab1
## Program Overview
This project is the Operating System lab 1. It consists of two separate processes that share a portion of memory. These two processes are the producer and the consumer. The producer produces 5 numbers starting at 0 and goes to 4. The consumer takes these numbers from the shared memory and prints them to the screen. The buffer size of the shared memory is 2 items. The producer has to run first in order for the program to work.
## Installations Needed To Run The Programs
### Recommend using Linux Machine to run  
This project uses POSIX pthreads. POSIX is included with Linux. If you want to use a Windows Machine, you would need to install POSIX library
### Installing compiling packages
Need gcc to compile the c code
First need to make sure packages are up to date
```bash
sudo apt update
```
Now we can download the compiling packages with the meta package build-essential. This is to install gcc and g++ compilers for c/c++.
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
The shared memory is created in /dev/shm/ with the name sharedMem. Creating the shared memory also returns a file descriptor, which is used to put the buffer structure in the shared memory. The file descriptor is truncated to the size of the buffer structure and mapped to a pointer called shm_map, which has the ability to read and write to the shared memory. This procedure is done to both the producer and the consumer. The producer can create the shared memory and the consumer opens it. After the processes have finished, the shared memory is deleted.
### Process Synchronization 
Semaphores are used to synchronize the two processes. There is one shared semaphore located in the shared memory that both processes can read and write to. The semaphore is initialized to 0 in the producer so the producer has to run first so the consumer does not reference the semaphore before its creation. It is initialized to 0 because the consumer waits before accessing the shared memory, while the producer does not have to wait to initialize the variables in the shared memory. After the initialization of the variables, the producer does wait before accessing the shared memory. This ensures that the producer goes into its critical section first. 
### Critical Sections
#### Producer
The producer has two different goals in its critical section. The first is to initialize the variables in the shared memory and the other goal is to add numbers into the buffer to be consumed. The initialization of the variables is always the first critical section from both the producer and consumer. It sets the in, out , and buffer count variables to 0. The main critical section, which is adding numbers to the buffer, has two main parts. It checks if the buffer is full. If the buffer is full,it leaves the critical section. Before the producer invokes wait, it sleeps for a small period of time. This gives the consumer time to remove items from the buffer. If there is space in the buffer, it adds the value of the local counter variables to the buffer array and prints a statement to the console that says what it added to the buffer. It increments the in variable so it points to the next position in the buffer and increments the buffer counter and count variable. After that it leaves the critical section and sleeps for a small period of time to allow the consumer to also enter the critical section.
#### Consumer	
After the consumer connects to the shared memory, it waits on the semaphore, which is only set to 1 after the producer goes through its initialization critical section. The consumer only has one critical section, which consists of two parts. If the buffer is empty, it leaves the critical section. Before the consumer invokes wait, it sleeps for a small period of time. This allows for the producer to add items to the buffer. If there are items in the buffer, it removes it from the buffer and replaces it with a zero and prints the item to the console. It also increments the out variable to point to the next position and decrements the buffer count variable. After that it leaves the critical section.
### Issues
A drawback to this implementation is that the producer has to run first in order to initialize the semaphore. If the consumer runs first, then it will wait on a semaphore that has not been initialized yet. Another issue with this implementation is that the processes use only one semaphore. This has the benefit of allowing a process to enter its critical section twice in a row. However it has the drawback of making a process sleep for a small period of time before invoking wait so it is slow. If two semaphores were used, it would remove the need for the sleep()  and would run faster, but a process could not enter its critical section twice in a row so it would not be able to utilize the buffer size of 2 to its fullest capacity. I choose to use one semaphore to fully utilize the buffer, which can hold two items. The implementation of the two semaphores can be found in the version-2 branch of this github.
### Print Statements
Both processes print to the console when the process starts, when they are waiting on their critical section, leaving their critical section, and when the process is exiting. The producer also prints what it is adding to the buffer and the consumer prints the item that it consumed from the buffer. Leaving critical statements always start with "..." and prints from within a critical section, starting with either a P or C followed by a  ->, depending on if it is the producer or consumer. For example, if the producer is in a critical section, the print will start with "P-> ".
## Examples
### Example 1:
```code
starting process P
P in critical to start and initialize variables
... P Leaving Critical from start
P is waiting on critical section
	P-> adding 0 to buffer
... P Leaving Critical
starting process C
P is waiting on critical section
	P-> adding 1 to buffer
... P Leaving Critical
C is waiting on critical section
	C-> printing: 0
...C Leaving Critical
C is waiting on critical section
	C-> printing: 1
...C Leaving Critical
P is waiting on critical section
	P-> adding 2 to buffer
... P Leaving Critical
C is waiting on critical section
	C-> printing: 2
...C Leaving Critical
P is waiting on critical section
	P-> adding 3 to buffer
... P Leaving Critical
C is waiting on critical section
	C-> printing: 3
...C Leaving Critical
P is waiting on critical section
	P-> adding 4 to buffer
... P Leaving Critical
exiting process p
C is waiting on critical section
	C-> printing: 4
...C Leaving Critical
exiting process c
```
### Example 2:
```code
starting process P
P in critical to start and initialize variables
... P Leaving Critical from start
P is waiting on critical section
	P-> adding 0 to buffer
... P Leaving Critical
starting process C
P is waiting on critical section
C is waiting on critical section
	P-> adding 1 to buffer
... P Leaving Critical
	C-> printing: 0
...C Leaving Critical
P is waiting on critical section
	P-> adding 2 to buffer
... P Leaving Critical
C is waiting on critical section
	C-> printing: 1
...C Leaving Critical
P is waiting on critical section
	P-> adding 3 to buffer
... P Leaving Critical
C is waiting on critical section
	C-> printing: 2
...C Leaving Critical
P is waiting on critical section
	P-> adding 4 to buffer
... P Leaving Critical
exiting process p
C is waiting on critical section
	C-> printing: 3
...C Leaving Critical
C is waiting on critical section
	C-> printing: 4
...C Leaving Critical
exiting process c
```
### Explanation
Both examples have the producer run first but the clear difference between the two was the order in which they accessed their critical sections. In both examples, the producer was able to access the critical sections twice in a row, but in the first example, the consumer was also able to have two critical sections in a row. In the second example, the consumer was not able to get two critical sections in a row until the producer program finished. With this implementation of the semaphores, the order of which process accesses their critical sections can change. The only exception to this is the producer's initialization critical section, which allows goes first.
