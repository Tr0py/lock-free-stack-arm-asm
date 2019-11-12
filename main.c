#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
					
extern void* pop(int id);
extern void push(int id, struct node* obj_ptr);

struct node{
	int data;
	struct node *next;
}; 

typedef struct node node;

node *p_top;
pthread_t hThread[1024];

void stack_init(int);
void *worker(void *arg)
{
	int id = *(int*)arg;
	while (1) {
		push(id, pop(id));
	}
	return NULL;
}

int main()
{
	stack_init(100);
	printf("program starts, stack top = %p\n", p_top);
	//printf("pop val = %p\n", pop(1));
	push(1, pop(1));
	int arg[1024];
	int nthread = 16;
	for (int i = 0; i < nthread; ++i) {
		arg[i] = i;
		pthread_create(&hThread[i], NULL, worker, &arg[i]);
	}
	for (int i = 0; i < nthread; ++i) {
		pthread_join(hThread[i], NULL);
	}

	printf("program starts, stack top = %p\n", p_top);

	return (0);
}

void stack_init(int n) 
{
	node *t;
	for (int i = 0; i < n; ++i) {
		t = malloc(sizeof(node));
		t->data = i+2;
		t->next = p_top;
		p_top = t;
	}
}
