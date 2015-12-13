#include <stdio.h>
#include <pthread.h>
// For debugging in GDB with threads: methodology 1
//   (gdb) info threads
//   (gdb) thread 2
//   (gdb) # and so on for other threads
//   (gdb) where # each thread has its own stack
//   (gdb) frame 2  # to go to call frame 2

// For debugging in GDB: methodology 2
//   (gdb) break consumer
//   (gdb) run
//   (gdb) print buf
//   (gdb) next
//   (gdb) print buf  # and so on
struct sem_t {
  int count;
  pthread_mutex_t mutex;
};
int buffer[4];
void sleep(int time);
int rand();
void push_buf(int work_item);
int take_from_buf();
int sem_init(struct sem_t *sem, int ignore, int init);
int sem_post(struct sem_t *sem);
int sem_wait(struct sem_t *sem);
void *producer(void *arg);
void *consumer(void *arg);
void block(struct sem_t *sem);
int num = 3;
int first_occupied_slot = 0;
int first_empty_slot = 0;
struct sem_t sem_producer;  // Should count number of empty slots available
struct sem_t sem_consumer;
struct sem_t sem_check;
pthread_mutex_t mut_buf = PTHREAD_MUTEX_INITIALIZER;  // Lock for anybody touching buf


int main() {
 // ... uses pthread_create to start producer and consumer
 // WARNING:  the primary thread runs main().  When main exits, the primary
 //             thread exits
 sem_consumer.count = 0;
 sem_producer.count = 4;
for(int i =0; i<num; i++) { 
 pthread_t c;
 pthread_create(&c, NULL, consumer, NULL);
}
for(int i = 0; i<num; i++) {
 pthread_t p;
 pthread_create(&p, NULL, producer, NULL);
}
 
 while (1);  // Don't let the primary thread exit
   
}

int sem_init(struct sem_t *sem, int ignore, int init) {
  sem->count = 0;
  pthread_mutex_init(&(sem->mutex), NULL);
}
 
int sem_post(struct sem_t *sem) {
  pthread_mutex_lock(&(sem->mutex));
  sem->count +=1;
//  printf("count after plus %d\n", sem->count);
//  fflush(stdout);
  pthread_mutex_unlock(&(sem->mutex));
}

int sem_wait(struct sem_t *sem) {
  pthread_mutex_lock(&(sem->mutex));
  sem->count -=1;
 // printf("count after minus:%d\n", sem->count);
 // fflush(stdout);
  if(sem->count < 0){
 //   printf("block\n");
 //   fflush(stdout);
    pthread_mutex_unlock(&(sem->mutex));
    block(sem);
    pthread_mutex_lock(&(sem->mutex));
  }
  pthread_mutex_unlock(&(sem->mutex));
}

void block(struct sem_t *sem) {
  int mycount;
  int newslot;
  int oldcount = first_occupied_slot;
  while(1) {
    pthread_mutex_lock(&(sem->mutex));
    mycount = sem->count;
 if(oldcount < -1 && mycount > oldcount|| mycount>=0) { 
//    if(-(sem->count) >= num_threads_blocked) {
      pthread_mutex_unlock(&(sem->mutex));
 //     printf("unblocked\n");
 //     fflush(stdout);
      return;
    }
    pthread_mutex_unlock(&(sem->mutex));
    sleep(1);
  } 
}
void *producer(void *arg) {
  int work_item = 1;
  while (1) {
    sleep( rand() % 5 );
    sem_wait(&sem_producer);  // Wait for empty slots
    pthread_mutex_lock(&mut_buf);
      push_buf(work_item++);  // inside critical section with mut_buf lock
    sem_post(&sem_consumer);  // Tell the consumer there's a new work item
    pthread_mutex_unlock(&mut_buf);
   }
}
void push_buf(int work_item) {
//  printf("produce: %d\n" ,  work_item, first_empty_slot % 4);
//  fflush(stdout);
  buffer[first_empty_slot%4] = work_item;
  first_empty_slot++;
}
// Exactly the same, but the inverse:
void *consumer(void *arg) {
  int print_item;
  while (1) {
    sleep( rand() % 5 );
    sem_wait(&sem_consumer);
    pthread_mutex_lock(&mut_buf);
      print_item = take_from_buf();
    sem_post(&sem_producer);
    pthread_mutex_unlock(&mut_buf);

    printf("%d \n", print_item);
    fflush(stdout);  // Force printing now; don't wait for the newline
  }
}

int take_from_buf() {
  int val = buffer[first_occupied_slot%4];
 // printf("get position:%d\n", first_occupied_slot % 4);
 // fflush(stdout);
  first_occupied_slot++;
  return val;
}
