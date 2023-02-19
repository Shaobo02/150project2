#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ucontext.h>
#include "private.h"
#include "uthread.h"
#include "queue.h"
queue_t all_ready_tcbs;
queue_t zombie_queue;
// the current tcb that is executing
static struct uthread_tcb* current;
// states of our tcb
enum state {
	running,
	ready,
	blocked,
	zombie,
};
// by default should be 0, because it is global
static int tid_assignment;
struct uthread_tcb {
	enum state tcb_state;
	int tid;
	uthread_ctx_t context;
	void* stack_location;
	uthread_func_t function;
	void* function_args;
	int retVal;
	/* TODO Phase 2 */
};

struct uthread_tcb *uthread_current(void) {
	return current;
	/* TODO Phase 2/3 */
}

void uthread_yield(void) {
	// pop out the tcb on top
	struct uthread_tcb* next_tcb;
	queue_dequeue(all_ready_tcbs, (void**)(&next_tcb));
	// push the current running tcb to the very bottom of the ready to run queue
	struct uthread_tcb* current_running = uthread_current();
	current_running->tcb_state = ready;
	queue_enqueue(all_ready_tcbs, current_running);
	// save a temp for the previous running tcb
	struct uthread_tcb* previous_running_tcb = current_running;
	// change the nextTcb to current and set its state to running
	current = next_tcb;
	next_tcb->tcb_state = running;
	uthread_ctx_switch(&(previous_running_tcb->context), &(next_tcb->context));
}

void uthread_exit(void) {
	/* TODO Phase 2 */
	// the current one that is running
	struct uthread_tcb* running_Tcb = calloc(1, sizeof(struct uthread_tcb));
	running_Tcb = current;
	running_Tcb -> tcb_state = zombie;
	queue_enqueue(zombie_queue,running_Tcb);
	//uthread_ctx_destroy_stack(current->stack_location);
	//free(running_Tcb);
	// swap context
	struct uthread_tcb* next_Tcb = calloc(1,sizeof(struct uthread_tcb));
	next_Tcb->tcb_state = zombie;
	while(next_Tcb->tcb_state == zombie){
		queue_dequeue(all_ready_tcbs, (void**)(&next_Tcb));
	}
	current = next_Tcb;
	uthread_ctx_switch(&(running_Tcb->context),&(next_Tcb->context));
}

int uthread_create(uthread_func_t func, void *arg) {
	/* TODO Phase 2 */
	struct uthread_tcb* create_tcb = calloc(1, sizeof(struct uthread_tcb));
	if (create_tcb == NULL) {
		return -1;
	}
	create_tcb->tid = tid_assignment;
	tid_assignment += 1;
	create_tcb->function = func;
	create_tcb->function_args = arg;
	create_tcb->stack_location = uthread_ctx_alloc_stack();
	uthread_ctx_init(&(create_tcb->context), create_tcb->stack_location, func, arg);
	if (create_tcb->stack_location == NULL) {
		return -1;
	}
	create_tcb->tcb_state = ready;
	queue_enqueue(all_ready_tcbs, create_tcb);
	return 0;
}

void clear_zombie_queue(queue_t zombie, void* data) {
	struct uthread_tcb* node = (struct uthread_tcb*)data;
	queue_delete(zombie, data);
	uthread_ctx_destroy_stack(node -> stack_location);
}

int uthread_run(bool preempt, uthread_func_t func, void *arg) {
	/* TODO Phase 2 */
	// store the current execution process
	all_ready_tcbs = queue_create();
	zombie_queue = queue_create();
	struct uthread_tcb* orig = calloc(1,sizeof(struct uthread_tcb));
	if (orig == NULL) {
		return -1;
	}
	orig->tid = tid_assignment;
	tid_assignment++;
	current = orig;
	int fail = uthread_create(func,arg);
	if (fail == -1) {
		return -1;
	}
	preempt_start(preempt);
	// run the given process
	while(1){
		// yield will run the next available thread
		uthread_yield();
		// need to rid of the thread that has been finished
		// only orig context remains in the queue
		if(queue_length(all_ready_tcbs) == 0){
			queue_iterate(zombie_queue, clear_zombie_queue);
			if(preempt){
				preempt_stop();
			}
			break;
		}
	}
	return 0;
}

void uthread_block(void) {
	/* TODO Phase 3 */
	// same as yield, but does not push it back to the ready to run queue_enqueue
	// pop out the tcb on top
	// version 1
	struct uthread_tcb* next_tcb;
	queue_dequeue(all_ready_tcbs, (void**)(&next_tcb));
	// push the current running tcb to the very bottom of the ready to run queue
	struct uthread_tcb* currentRunning = uthread_current();
	// set the currently running to blocked, it will be collected by the semahphore
	currentRunning->tcb_state = blocked;
	// save a temp for the previous running tcb
	struct uthread_tcb* previousRunningTcb = currentRunning;
	// change the nextTcb to current and set its state to running
	current = next_tcb;
	next_tcb->tcb_state = running;
	uthread_ctx_switch(&(previousRunningTcb->context), &(next_tcb->context));
}

void uthread_unblock(struct uthread_tcb *uthread) {
	/* TODO Phase 3 */
	// the thread gets dequeued from the semaphore queue
	// then it is pushed to the end of the ready to run queue_enqueue
	uthread->tcb_state = ready;
	queue_enqueue(all_ready_tcbs, uthread);
}