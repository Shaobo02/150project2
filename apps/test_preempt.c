#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "uthread.h"

// a global variable 
// to be changed by thread 2
int x = 0;

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

void thread2(void *arg) {
	(void)arg;
	x = 1;
}

void thread1(void *arg) {
	(void)arg;
	uthread_create(thread2, NULL);
	while(1) {
		if (x == 1)
			break;
	}
}

int main(void) {
	uthread_run(true, thread1, NULL);
	// if preemption works, then the global variable should be set to 1
  	TEST_ASSERT(x != 0);
	return 0;
}
