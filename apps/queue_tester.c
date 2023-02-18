#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"


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

char inserted_value[5];
/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* destroy */
void test_destroy(void)
{
	fprintf(stderr, "*** TEST destroy ***\n");
	queue_t q;
	q = queue_create();
	void* q_addr = &q;
	int value = 1;
	queue_enqueue(q, &value);
	TEST_ASSERT(queue_destroy(q) == -1);
	void* data;
	queue_dequeue(q, &data);
	TEST_ASSERT(queue_destroy(q) == 0);
	void* destroyed = &q;
	TEST_ASSERT(queue_length(q) == 0);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
	queue_destroy(q);
}

void enqueue_tester_helper(queue_t q, void* data)
{
	int value = *((int*)data); 
	inserted_value[value] = value;
}

// enqueue tester
void test_enqueue(void)
{
	queue_t q;
	fprintf(stderr, "*** TEST enqueue ***\n");

	q = queue_create();
	int test_data_0 = 0;
	int result_null_data = queue_enqueue(q, NULL);
	TEST_ASSERT(result_null_data == -1);
	int result_null_queue = queue_enqueue(NULL, &test_data_0);
	TEST_ASSERT(result_null_queue == -1);
	int result_sucess = queue_enqueue(q, &test_data_0);
	TEST_ASSERT(result_sucess == 0);
	TEST_ASSERT(queue_length(q) == 1);

	
}

void test_enqueue_multiple_insertion(void)
{
	queue_t q;
	fprintf(stderr, "*** TEST multiple insertion ***\n");
	q = queue_create();
	int test_data_0 = 0;
	int test_data_1 = 1;
	int test_data_2 = 2;
	int test_data_3 = 3;
	int test_data_4 = 4;
	queue_enqueue(q, &test_data_0);
	queue_enqueue(q, &test_data_1);
	queue_enqueue(q, &test_data_2);
	queue_enqueue(q, &test_data_3);
	queue_enqueue(q, &test_data_4);
	queue_iterate(q, enqueue_tester_helper);
	TEST_ASSERT(inserted_value[0] == 0);
	TEST_ASSERT(inserted_value[1] == 1);
	TEST_ASSERT(inserted_value[2] == 2);
	TEST_ASSERT(inserted_value[3] == 3);
	TEST_ASSERT(inserted_value[4] == 4);
	TEST_ASSERT(queue_length(q) == 5);
}

// dequeue tester
void test_dequeue(void)
{
	queue_t q;
	fprintf(stderr, "*** TEST dequeue ***\n");

	q = queue_create();
	int test_value = 1;
	void* test_data;
	int result_null_data = queue_dequeue(q, NULL);
	TEST_ASSERT(result_null_data == -1);
	int result_null_queue = queue_dequeue(NULL, &test_data);
	TEST_ASSERT(result_null_queue == -1);
	queue_enqueue(q, &test_value);
	int result_sucess = queue_dequeue(q, &test_data);
	TEST_ASSERT(result_sucess == 0);
	TEST_ASSERT(*((int*)test_data) == 1);
	TEST_ASSERT(queue_length(q) == 0);
	queue_destroy(q);
}

// test dequeue from a long queue
void test_multiple_dequeue(void)
{
	queue_t q;
	fprintf(stderr, "*** TEST multiple dequeue ***\n");
	q = queue_create();
	int test_data_0 = 0;
	int test_data_1 = 1;
	int test_data_2 = 2;
	int test_data_3 = 3;
	int test_data_4 = 4;
	queue_enqueue(q, &test_data_0);
	queue_enqueue(q, &test_data_1);
	queue_enqueue(q, &test_data_2);
	queue_enqueue(q, &test_data_3);
	queue_enqueue(q, &test_data_4);
	
	TEST_ASSERT(queue_length(q) == 5);

	void* value;
	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 0);
	TEST_ASSERT(queue_length(q) == 4);

	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 1);
	TEST_ASSERT(queue_length(q) == 3);

	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 2);
	TEST_ASSERT(queue_length(q) == 2);

	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 3);
	TEST_ASSERT(queue_length(q) == 1);

	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 4);
	TEST_ASSERT(queue_length(q) == 0);
}

// delete tester
void test_delete(void)
{
	queue_t q;
	fprintf(stderr, "*** TEST delete ***\n");

	q = queue_create();
	int test_value = 1;
	void* test_data;

	queue_enqueue(q, &test_value);
	TEST_ASSERT(queue_delete(NULL, &test_value) == -1);
	TEST_ASSERT(queue_delete(q, NULL) == -1);
	int not_found = queue_delete(q, &test_data);
	TEST_ASSERT(not_found == -1);
	TEST_ASSERT(queue_length(q) == 1);
	int success = queue_delete(q, &test_value);
	TEST_ASSERT(success == 0);
	TEST_ASSERT(queue_length(q) == 0);
	queue_destroy(q);
}

// test multiple delete
void test_multiple_delete(void)
{
	queue_t q;
	fprintf(stderr, "*** TEST multiple delete ***\n");
	q = queue_create();
	int test_data_0 = 0;
	int test_data_1 = 1;
	int test_data_2 = 2;
	int test_data_3 = 3;
	int test_data_4 = 4;
	queue_enqueue(q, &test_data_0);
	queue_enqueue(q, &test_data_1);
	queue_enqueue(q, &test_data_2);
	queue_enqueue(q, &test_data_3);
	queue_enqueue(q, &test_data_4);

	
	
	queue_delete(q, &test_data_0);
	queue_delete(q, &test_data_2);
	queue_delete(q, &test_data_4);
	TEST_ASSERT(queue_length(q) == 2);

	void* value;
	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 1);
	TEST_ASSERT(queue_length(q) == 1);
	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 3);
	TEST_ASSERT(queue_length(q) == 0);
	queue_destroy(q);
}

static void iterator_inc(queue_t q, void *data)
{
    int *a = (int*)data;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
}

void test_iterator(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Increment every item of the queue, delete item '42' */
    queue_iterate(q, iterator_inc);
	TEST_ASSERT(data[0] == 2);
	TEST_ASSERT(queue_length(q) == 9);
	void* value;
	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 2);
	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 3);
	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 4);
	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 5);
	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 6);
	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 7);
	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 8);
	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 9);
	queue_dequeue(q, &value);
	TEST_ASSERT(*((int*)value) == 10);

}

int main(void)
{
	test_create();
	test_destroy();
	test_queue_simple();
	test_enqueue();
	test_enqueue_multiple_insertion();
	test_dequeue();
	test_multiple_dequeue();
	test_delete();
	test_multiple_delete();
	test_iterator();
	return 0;
}
