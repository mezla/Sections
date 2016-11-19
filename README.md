# Section 2:  [Processes](http://cs162.eecs.berkeley.edu/static/spring2015/sections/section2.pdf)

## February 1, 2015

## Reference

   * [Section: process - solution](http://cs162.eecs.berkeley.edu/static/spring2015/sections/section2soln.pdf)
   * [Section: thread](https://gist.github.com/thinkhy/8fdd0c53d05ceff7f32d)
   * [HW 1](https://gist.github.com/thinkhy/3438a84cf8ff0db7bab2)
   * [Project 1](https://gist.github.com/thinkhy/f7d932fb9ff617b60850)
   

## Contents
  
   * 1  Warmup   
       - 1.1 Hello World
   * 2  Vocabulary
   * 3  Problems
       - 3.1    Forks
       - 3.2    Fork Bomb
       - 3.3    Stack Allocation
       - 3.4    Heap Allocation
       - 3.5    Simple Wait
       - 3.6    Complicated Wait
       - 3.7    Exec
       - 3.8    Exec + Fork
 
## 1 Warmup

### 1.1  Hello World

What can C print in the below code?  Assume the child's PID is 90210 (Hint:  There is more than one correct answer)

```C
  int main() {
      pid_t pid = fork();
      printf("Hello World: %d\n", pid);
  }
```
  
My answer is:

```
Hello world: 90210
Hello world: 0
```

## 2  Vocabulary

   * process
      - a process is an instance of a computer program that is being executed.  It contains the program code and its current activity.
   * exit code
      - The exit status or return code of a process is a 1 byte number passed from a child process (or callee) to a parent process (or caller) when it has finished executing a specic procedure or delegated task
   * address space
      - The address space for a process is the set of memory addresses that it can use. The address space for each process is private and cannot be accessed by other processes unless it is shared.
   * stack  
      - The stack is the memory set aside as scratch space for a thread of execution.  When a function is called, a block is reserved on the top of the stack for local variables and some book-keeping data.  When that function returns, the block becomes unused and can be used the next time a function is called.  The stack is always reserved in a LIFO (last in first out) order; the most
recently reserved block is always the next block to be freed.
   * heap
     - The heap is memory set aside for dynamic allocation.  Unlike the stack, there's no enforced pattern to the allocation and deallocation of blocks from the heap; you can allocate a block at any time and free it at any time.
   * fork
     - A C function that calls the fork syscall that creates a new process by duplicating the calling process.   The  new  process, referred  to  as  the  child,  is  an  exact  duplicate  of  the  calling  process (except for a few details, read more in the man page).  Both the newly created process and the parent process return from the call to fork.  On success, the PID of the child process is returned in the parent, and 0 is returned in the child.  On failure, -1 is returned in the parent, no child process
is created.
   * wait
      - A class of C functions that call syscalls that are used to wait for state changes in a child of the calling process, and obtain information about the child whose state has changed.  A state change is considered to be:  the child terminated; the child was stopped by a signal; or the child was resumed by a signal.
   * exec
      - The exec() family of functions replaces the current process image with a new process image. The initial argument for these functions is the name of a file that is to be execute

## Problems

## 3.1  Forks

  How many new processes are created in the below program?

```C
int main(void)
{
for (int i = 0; i < 3; i++) {
pid_t pid = fork();
}
```

My answer is *8*

CS 162 Spring 2015                                                                                                    Section 2:  Processes

wait
- A class of C functions that call syscalls that are used to wait for state changes in a child
of the calling process, and obtain information about the child whose state has changed.  A state
change is considered to be:  the child terminated; the child was stopped by a signal; or the child
was resumed by a signal.

exec
- The exec() family of functions replaces the current process image with a new process image.
The initial argument for these functions is the name of a le that is to be executed.
3  Problems
3.1  Forks
How many new processes are created in the below program?
int main(void)
{
for (int i = 0; i < 3; i++) {
pid_t pid = fork();
}
}

## 3.2  Fork Bomb
    
  Why is the following code terrible for your system?  (Don't try this at home)

```C
  int main(void)
  {
    while(1)
    fork();
  }
```
I run the code on VM, the VM got stuck, when sshed into the system, it showed:

```
shell request failed on channel 0
```

But luckily enough, the system kept alive, and restored after I issued "Ctrl+C".

### 3.3  Stack Allocation

 What can C print?

```C
int main(void)
{
int stuff = 7;
pid_t pid = fork();
printf("Stuff is %d", stuff);
if (pid == 0)
stuff = 6;
}

Stuff is 7Stuff is 7
```
CS 162 Spring 2015                                                                                                    Section 2:  Processes
3.3  Stack Allocation
What can C print?
int main(void)
{
int stuff = 7;
pid_t pid = fork();
printf("Stuff is %d", stuff);
if (pid == 0)
stuff = 6;
}

### 3.4  Heap Allocation

What can C print?

```C
int main(void)
{
  int* stuff = malloc(sizeof(int)*1);
  *stuff = 7;
  pid_t pid = fork();
  printf("Stuff is %d", *stuff);
  if (pid == 0) {
     *stuff = 6
  }
}

Stuff is 7Stuff is 7
```

### 3.5  Simple Wait

What can C print?  Assume the child PID is 90210.

```C
int main(void)
{
pid_t pid = fork();
int exit;
if (pid != 0) {
wait(&exit);
}
printf("Hello World\n: %d\n", pid);
}

Hello World
: 0
Hello World
: 90210

```

What is the equivalent program using the waitpid function instead of wait?

```C
int main(void)
{
pid_t pid = fork();
int exit;
if (pid != 0) {
waitpid(pid, &exit, WIFEXITED);
}
printf("Hello World\n: %d\n", pid);
}

```

```C
// Official solution:
int main(void)
{
pid_t pid = fork();
int exit;
if (pid != 0) {
waitpid(-1, &exit, 0); // -1 stands current process group
}
printf("Hello World\n: %d\n", pid);
```


## 3.6  Complicated Wait

What is the exit code of the following program?  (Hint:  Try to recognize a pattern)

```C
int foo(n){
if (n < 2) {
exit(n);
} else {
int v1;
int v2;
pid_t pid = fork();
if (pid == 0)
foo(n - 1);
pid_t pid2 = fork();
if (pid2 == 0)
foo(n - 2);
waitpid(pid,&v1,0);
waitpid(pid2,&v2,0);
exit(WEXITSTATUS(v1) + WEXITSTATUS(v2));
}
}
int main() {
foo(10);
}

foo(10) = 55
f(n)=f(n-1)+f(n-2),
f(n) = n when n == 0 or n == 1
```

Why does this program start to behave (even more) strangely when we have n > 13? What is the exit code for foo(14)  (You can use a calculator if you want)

when n > 13, processes number may exceed soft limit.  *WRONG!!*

foo(14) = 121


### Official Solution

```
55 (This is the fibonacci sequence)
Why does this program start to behave (even more) strangely when we have n > 13? What is the exit
code for foo(14)? (You can use a calculator if you want)
121 ([Exit](https://en.wikipedia.org/wiki/Exit_(system_call)) codes are only 1 byte so only the last 8 bits of the 14th fibonacci number are used).
```

## 3.7  Exec

What can C print?

```C
int main(void)
{
char** argv = (char**) malloc(3*sizeof(char*));
argv[0] = "/bin/ls";
argv[1] = ".";
argv[2] = NULL;
for (int i = 0; i < 10; i++){
printf("%d\n", i);
if (i == 3)
execv("/bin/ls", argv);
}

Output:
0
1
2
3
a.out  exec.c  stack.c	wait2.c  wait3.c  wait.c

```

## 3.8  Exec + Fork

How would I modify the above program using fork so it both prints the output of ls and all the numbers from 0 to 9 (order does not matter)?  You may not remove lines from the original program; only add statements (and use fork!).

```C
int main(void)
{
        char** argv = (char**) malloc(3*sizeof(char*));
        argv[0] = "/bin/ls";
        argv[1] = ".";
        argv[2] = NULL;
        int i;
        pid_t pid;
        for (i = 0; i < 10; i++){
                printf("%d\n", i);
                if (i == 3) {
                        pid = fork();
                        if(pid == 0)
                                execv("/bin/ls", argv);
                }
        }
}
```

# [Section 3:  Threads](http://cs162.eecs.berkeley.edu/static/spring2015/sections/section3.pdf)

   * Stanley Hung and William Liu
   * February 4, 2015

## Reference
  
   * [Section: thread - solution](http://cs162.eecs.berkeley.edu/static/spring2015/sections/section3soln.pdf)
   * [Section: Process](https://gist.github.com/thinkhy/f7d932fb9ff617b60850)
   * [HW 1](https://gist.github.com/thinkhy/3438a84cf8ff0db7bab2)
   * [Project 1](https://gist.github.com/thinkhy/f7d932fb9ff617b60850)

## Contents

   * 1   Warmup 
   * 1.1 Hello World
   * 2   Vocabulary
   * 3  Problems
   * 3.1    Join
   * 3.2    Stack Allocation
   * 3.3    Heap Allocation
   * 3.4    Threads and Processes  
   * 3.5    Atomicity
   * 3.6    Synchronization
   * 3.7    Simple HTTP Server with Threads
 
## 1  Warmup

### 1.1  Hello World

What does C print in the following code?

```C
void print_hello_world() {
pid_t pid = getpid();
printf("Hello world %d\n", pid);
pthread_exit(0);
}
void main() {
pthread_t thread;
pthread_create(&thread, NULL, (void *) &print_hello_world, NULL);
print_hello_world();
}

Hello world 3209
Hello world 3209
```

## Vocabulary

   * thread
      - a thread of execution is the smallest unit of sequential instructions that can be scheduled
for execution by the operating system.  Multiple threads can share the same address space, and
each thread independently operates using its own program counter.

   * pthreads
      - A POSIX-compliant (standard specified by IEEE) implementation of threads.

   * pthread_join 
       - Waits for a specific thread to terminate, similar to waitpid(3). (Hint: man pthread_join)

   * pthread_create
       - Creates and immediately starts a child thread running in the same address space
of the thread that spawned it.  The child executes starting from the function specified.  Internally,
this is implemented by calling the clone syscall.  (Hint: man pthread_create)

   * atomic
      - An  operation  is  deemed  to  be  atomic  if  it  can  be  executed  without  interruption  or
interference from other threads/processes).

   * critical section
      - A section of code that accesses a shared resource that must not be concurrently accessed by more than a single thread.

   * semaphore
      - A synchronization primitive that can be used to protect a shared resource. Semaphores contain an integer value and support two operations:
         a.  Increment:  atomically increments the value of the semaphore.  (Hint: man sem_post)
         b.   Decrement:   waits  for  the  value  of  the  semaphore  to  become  positive,  and  then  atomically
decrements the value of the semaphore.  (Hint: man sem_wait)

                                                                                              
## 3  Problems

### 3.1  Join

What does C print in the following code?(Hint:  There may be zero, one, or multiple answers.)

```C
void main() {
pthread_t thread;
pthread_create(&thread, NULL, &helper, NULL);
printf("Hello World! 2\n");
exit(0);
}
void *helper(void* arg) {
printf("Hello World! 1\n");
pthread_exit(0);
}

[thinkhy@localhost Will_Section3_Resources]$ ./join fg
Hello World! 2
Hello World! 1
Hello World! 1
[thinkhy@localhost Will_Section3_Resources]$ ./join fg
Hello World! 2
[thinkhy@localhost Will_Section3_Resources]$ ./join fg
Hello World! 2
[thinkhy@localhost Will_Section3_Resources]$ ./join fg
Hello World! 2
[thinkhy@localhost Will_Section3_Resources]$ ./join fg
Hello World! 2
Hello World! 1

```

How can we modify the code above to always print out

```
"Hello World! 1"
followed by
"Hello World! 2"
```

Add pthread_join in main function:

```C
void main() {
        pthread_t thread;
        pthread_create(&thread, NULL, &helper, NULL);
        pthread_join(thread, NULL);
        printf("Hello World! 2\n");
        exit(0);
}
```

## 3.2  Stack Allocation

What does C print in the following code?

```C
void main() {
int i = 0;
pthread_t thread;
pthread_create(&thread, NULL, &helper, &i);
pthread_join(thread, NULL);
printf("i is %d\n", i);
}
void *helper(void *arg) {
int *num = (int*) arg;
*num = 2;
pthread_exit(0);
}

2
```

### 3.3  Heap Allocation

What does C print in the following code?

```C
void main() {
char *message = malloc(100);
strcpy(message, "I am the parent");
pthread_t thread;
pthread_create(&thread, NULL, &helper, message);
pthread_join(thread, NULL);
printf("%s\n", message);
}
void *helper(void *arg) {
char *message = (char *) arg;
strcpy(message, "I am the child");
pthread_exit(0);
}

I am the child
```

###3.4  Threads and Processes

What does C print in the following code? (Hint:  There may be zero, one, or multiple answers.)

```C
void *worker(void *arg) {
int *data = (int *) arg;
*data = *data + 1;
printf("Data is %d\n", *data);
pthread_exit(0);
}
int data;
void main() {
int status;
data = 0;
pthread_t thread;
pid_t pid = fork();
if (pid == 0) {
pthread_create(&thread, NULL, &worker, &data);
pthread_join(thread, NULL);
} else {
pthread_create(&thread, NULL, &worker, &data);
pthread_join(thread, NULL);
pthread_create(&thread, NULL, &worker, &data);
pthread_join(thread, NULL);
wait(&status);
}
}

One of the following is printed out:
"Data is 1"
"Data is 1"
"Data is 2"
"Data is 1"
"Data is 2"
"Data is 1"

```

## 3.5  Atomicity

Given: int x = 0;

Circle all the atomic operations below:

```C
printf("x is %d\n", x);
x = malloc(sizeof(int));
int y = x;
x++;

None of the operations listed are atomic. 1) Printing to stdout is not atomic because it involves
many operations, including writing to a file, some of which are blocking. 2) Allocating memory is
not atomic. 3) Assigning x to y is not atomic because it involves multiple loads/stores to registers.
4) Incrementing x is not atomic because it involves a load, store, and add operation.
```

 TODO 6/5/2015  ???

### 3.6  Synchronization

What does C print in the following code?

```C
void main() {
pthread_t threads[5];
int i, j = 0;
for (i = 0; i < 5; i++) {
pthread_create(&threads[i], NULL, &helper, &j);
}
for (i = 0; i < 5; i++) {
pthread_join(threads[i], NULL);
}
printf("j is %d\n", j);
}
void *helper(void *arg) {
int *num = (int *) arg;
*num = (*num) + 1;
pthread_exit(0);
}

5
```

How can we modify the code in the previous page to always print out "i is 5" without modifying/using pthread_join?

Using semaphore

```C
// Official solution
sem_t sem;
void main() {
pthread_t threads[5];
int i, j = 0;
sem_init(&sem, 0, 0);
for (i = 0; i < 5; i++) {
pthread_create(&threads[i], NULL, &helper, &j);
}
sem_post(&sem);
for (i = 0; i < 5; i++) {
pthread_join(threads[i], NULL);
6
CS 162 Spring 2015 Section 3: Threads
}
printf("j is %d\n", j);
}
void *helper(void *arg) {
sem_wait(&sem);
int *num = (int *) arg;
*num = (*num) + 1;
sem_post(&sem);
pthread_exit(0);
}



### 3.7  Simple HTTP Server with Threads

Implement  a  crude  HTTP  server  by  filling  in  the  blanks  with  the  necessary  operations.   This  HTTP
server spawns worker threads that are responsible for printing out the first 256 bytes from a socket.  For
this question, you can assume the following structures and functions are implemented appropriately.

```C
// a FIFO queue consisting of nodes.
struct list;
// A single element in the FIFO queue, containing an integer.
struct node {
int client_sock;
};
// This function sets up a socket, binds the socket to a specific port,
// and listens for connections.  It returns the file descriptor
// corresponding to the socket setup.
int setupSocket();
// initializes a FIFO queue
void list_init(struct list *l);
// Pops and returns the earliest enqueued node from a list
struct node *list_pop(struct list *l);
// Appends a new file descriptor as a struct node in the list.
struct node *list_append(struct list *l, int client_sock);
// Global variables
struct list         clients;
sem_t               worker_sem;
sem_t               list_sem;
// Prints out the first 256 bytes from a socket, and then closes it.
void accept_request(void * unused) {
char buf[256];
(void) unused;
while(1) {
_______________;
_______________;
int sockfd = list_pop(&clients)->client_sock;
if (______________________________ != 256) {
fprintf(stderr, "Failed to read from client %d\n", sockfd);
} else {
fprintf(stdout, "Received from client %d: %s\n", sockfd, buf);
}
_______________;
close(sockfd);
7
CS 162 Spring 2015                                                                                                      Section 3:  Threads
}
}
void main() {
pthread_t workers[10];
int client_sock;
struct sockaddr_in client_name;
socklen_t size;
int sockfd;
// initialize variables
list_init(&clients);
sem_init(&worker_sem, 0, 0);
_______________;
_______________;
// create worker threads
for (int i = 0; i < 10; i++) {
___________________________________________________________;
}
while (1) {
client_sock = ________________________________;
_______________;
list_append(&clients, client_sock);
_______________;
_______________;
}
}

My solution:

while(1) {
  sem_wait(sem_worker); ++
  sem_wait(sem_list) ++
  if ( read(sockfd, buf, 256) != 256) { ++
  	
  }
  sem_post(sem_list) ++
  close(sockfd)
    


main() {
     sem_init(&list, 0, 1);	
     ~~create_server( ... )~~

    ~~sockfd = socket(AF_INET, SOCK_STREAM, 0); ++~~
    ~~bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) ++~~
     
    sem_init(&list_sem, 0, 1);
    setupSocket() ++
     
    for (int i = 0; i < 10; i++) {
	pthread_create (&workers[i], NULL, (void *) accept_request,  sockfd); ++
    }

    while (1) {
		client_sock = accept(client_sock,(struct sockaddr *) &client_name, &size); ++
		sem_wait(&sem_list);  ++
		list_append(&clients, client_sock);
		sem_post(&sem_post); ++

		sem_post(&sem_worker) ++
	}
}

```
