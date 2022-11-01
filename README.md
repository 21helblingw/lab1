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
gcc producer_v2.c -pthread -lrt -o producer_v2
gcc consumer_v2.c -pthread -lrt -o consumer_v2
./producer_v2 & ./consumer_v2 &
```
## Documentation
### Shared Memory
The shared memory is created in /dev/shm/ with the name sharedMem. Creating the shared memory also returns a file descriptor, which is used to put the buffer structure in the shared memory. The file descriptor is truncated to the size of the buffer structure and mapped to a pointer called shm_map, which has the ability to read and write to the shared memory. This procedure is done to both the producer and the consumer. The producer creates the shared memory and the consumer opens the shared memory. After the processes have finished, the shared memory is deleted.
### Process Synchronization 
Semaphores are used to synchronize the two processes. There are two shared semaphores located in the shared memory that both processes can read and write to. There is one semaphore for the producer and one for the consumer. The producer's semaphore and the consumer's semaphore are initialized to 0 in the producer so the producer has to run first so the consumer does not reference the semaphore before its creation. They are initialized to 0 so the producer can set the variables in the shared memory to 0 first. This is done by making the consumer wait before accessing the shared memory, while the producer does not have to wait to initialize the variables in the shared memory. After the initialization of the variables, the producer does wait before accessing the shared memory. This ensures that the producer goes into its critical section first. 
### Critical Sections
#### Producer
The producer has two different goals in its critical section. The first is to initialize the variables in the shared memory and the other goal is to add numbers into the buffer to be consumed. The initialization of the variables is always the first critical section from both the producer and consumer. It sets the in, out , and buffer count variables to 0. The main critical section, which is adding numbers to the buffer, has two main parts. It checks if the buffer is full. If the buffer is full,it leaves the critical section. If there is space in the buffer, it adds the value of the local counter variables to the buffer array and prints a statement to the console that says what it added to the buffer. It increments the in variable so it points to the next position in the buffer and increments the buffer counter and count variable. After the producer leaves the critical section, it signals the consumer's semaphore with the post() function, allowing the consumer to enter its critical section.
#### Consumer	
After the consumer connects to the shared memory, it waits on the semaphore, which is only set to 1 after the producer goes through its initialization critical section. The consumer only has one critical section, which consists of two parts. If the buffer is empty, it leaves the critical section. If there are items in the buffer, it removes it from the buffer and replaces it with a zero. It also increments the out variable to point to the next position and decrements the buffer count variable. After the consumer leaves the critical section, it signals the consumer's semaphore with the post() function, allowing the consumer to enter its critical section.
### Issues
A drawback to this implementation is that the producer has to run first in order to initialize the semaphore. If the consumer runs first, then it will wait on a semaphore that has not been initialized yet. It also does not fully utilize the buffer size of 2. With two semaphores, the producer and consumer take turns entering the critical sections in a strict alternating pattern so the producer will never have a chance to fill the buffer.
### Print Statements
Both processes print to the console when the process starts, when they are waiting on their critical section, leaving their critical section, and when the process is exiting. The producer also prints what it is adding to the buffer and the consumer prints the item that it consumed from the buffer. Leaving critical statements always start with "..." and prints from within a critical section start with either a P or C followed by a ->, depending on if it is the producer or consumer. For example, if the producer is in a critical section, the print will start with "P-> ". The leaving print with "store mem" stands for store memory and it is the critical section where the producer puts items in the buffer.
## Examples
### Example 1:
```code
starting process P
P in Critical to start and init variables
... P Leaving Critical from start
P is waiting on critical section
	P-> adding 0 to buffer
... P Leaving Critical from store mem
P is waiting on critical section
starting process C
C is waiting on critical section
	C-> printing: 0
...C Leaving Critical
C is waiting on critical section
	P-> adding 1 to buffer
... P Leaving Critical from store mem
P is waiting on critical section
	C-> printing: 1
...C Leaving Critical
C is waiting on critical section
	P-> adding 2 to buffer
... P Leaving Critical from store mem
P is waiting on critical section
	C-> printing: 2
...C Leaving Critical
C is waiting on critical section
	P-> adding 3 to buffer
... P Leaving Critical from store mem
P is waiting on critical section
	C-> printing: 3
...C Leaving Critical
C is waiting on critical section
	P-> adding 4 to buffer
... P Leaving Critical from store mem
exiting process p
	C-> printing: 4
...C Leaving Critical
exiting process c
```
### Example 2:
```code
starting process P
P in Critical to start and init variables
... P Leaving Critical from start
P is waiting on critical section
	P-> adding 0 to buffer
... P Leaving Critical from store mem
P is waiting on critical section
starting process C
C is waiting on critical section
	C-> printing: 0
...C Leaving Critical
C is waiting on critical section
	P-> adding 1 to buffer
... P Leaving Critical from store mem
P is waiting on critical section
	C-> printing: 1
...C Leaving Critical
C is waiting on critical section
	P-> adding 2 to buffer
... P Leaving Critical from store mem
P is waiting on critical section
	C-> printing: 2
...C Leaving Critical
C is waiting on critical section
	P-> adding 3 to buffer
... P Leaving Critical from store mem
P is waiting on critical section
	C-> printing: 3
...C Leaving Critical
C is waiting on critical section
	P-> adding 4 to buffer
... P Leaving Critical from store mem
exiting process p
	C-> printing: 4
...C Leaving Critical
exiting process c
```
### Explanation
This implementation of the program allows for a consistent output every time. The producer will always enter the critical section first to initialize the variables. It will then enter again to produce the first item and add it to the buffer. After that, it will signal the consumer’s semaphore with the wait() function. The consumer enters the critical section and signals the producer’s semaphore when it exits the critical section. This cycle continues until the processes end. This follows a strict alternation with the producer always being the first in the critical section and the consumer always being the last to enter the critical section. 
