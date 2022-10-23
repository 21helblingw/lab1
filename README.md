# lab1
This project is the Operating System lab 1. It consists of two seperate processes that share a portion of memory. These two porcesses are the producer and the consumer. The producer produces 5 numbers starting at 0 and goes to 4. The consumer takes these numbers from the shared memory and prints them to the screen. The buffer size of the shared memory is 2 items. The producer has to run first in order for the program to work.
## Install
### Download on a Linux Machine
this project only works on linux

## Usage
```bash
gcc producer.c -pthread -lrt -o producer
gcc consumer.c -pthread -lrt -o consumer
./producer & ./consumer &
