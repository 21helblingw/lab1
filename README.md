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
