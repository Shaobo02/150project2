#include <stddef.h>
#include <stdlib.h>
#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	/* TODO Phase 3 */
  queue_t blocked_queue;
  int my_count;
  int lock;
};

int test_and_set(int *mem) {
    int old_val = *mem;
    *mem = 1;
    return old_val;
}
void spinlock_lock(int *lock) {
    while (test_and_set(lock) == 1) {
      uthread_yield();
    }
}

void spinlock_unlock(int *lock) {
    *lock = 0;
}

sem_t sem_create(size_t count) {
	/* TODO Phase 3 */
	  struct semaphore* sem = calloc(1,sizeof(struct semaphore));
    if (sem == NULL) {
      return NULL;
    }
    sem->blocked_queue = queue_create();
    sem->my_count = count;
    sem->lock = 0;
    return sem;
}

int sem_destroy(sem_t sem) {
	/* TODO Phase 3 */
  // go through the entire blocked queue and delete everything
  if (sem == NULL || queue_length(sem->blocked_queue) != 0) {
    return -1;
  }
  free(sem);
  return 0;
}

int sem_down(sem_t sem) {
	if(sem == NULL){
		return -1;
	}
	/* TODO Phase 3 */
  spinlock_lock(&(sem->lock));
  if (sem->my_count == 0) {
      queue_enqueue(sem->blocked_queue,uthread_current());
      spinlock_unlock(&(sem->lock));
      uthread_block();
      spinlock_lock(&(sem->lock));
  }
  sem->my_count -= 1;
  spinlock_unlock(&(sem->lock));
  return 0;
}

int sem_up(sem_t sem) {
	/* TODO Phase 3 */
  spinlock_lock(&(sem->lock));
  if(sem == NULL){
	  return -1;
  }
  sem->my_count += 1;
  /* Wake up first in line */
  /* (if any)              */
  // first dequeue
  if(queue_length(sem->blocked_queue) == 0){
    spinlock_unlock(&(sem->lock));
    return 0;
  }
  void* temp_tcb;
  queue_dequeue(sem->blocked_queue, &temp_tcb);
  uthread_unblock(temp_tcb);
  spinlock_unlock(&(sem->lock));
  return 0;
}