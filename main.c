#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <assert.h>
					
extern void* pop();
extern void push(struct node* obj_ptr);

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
	long tid = syscall(__NR_gettid);
	printf("thread starts, tid = %ld\n", tid);
	int i = 0xFFFF;
	while (--i) {
		push(pop());
		/* FIXME: this is not atomic since reading p_top and p_top->next 
		 * is not atomic. So how to detect aba?
		 */
		/*
		if (p_top == p_top->next) {
                       printf("[test]\tp == p->next. Stack is smashed.\n");
                       exit(1);
               }
	       */
	}
	printf("thread ends, tid = %ld\n", tid);
	return NULL;
}

void print_stack()
{
	printf("===============Printing stack...=============\n");
	struct node *p, p1, p2;
	p = p_top;
	while (p) {
		printf("node %p data %d next %p\n", p, p->data, p->next);
		if (p == p->next) {
			printf("p == p->next. Stack is smashed.\n");
			exit(1);
		}

		p = p->next;
	}
}

int main()
{
	int arg[1024];
	int nthread = 16;
	stack_init(nthread + 10);
	printf("program starts, stack top = %p\n", p_top);
	printf("pop val = %p\n", pop());
	print_stack();
	printf("===============Creating threads=============\n");
	for (int i = 0; i < nthread; ++i) {
		arg[i] = i;
		pthread_create(&hThread[i], NULL, worker, &arg[i]);
	}
	for (int i = 0; i < nthread; ++i) {
		pthread_join(hThread[i], NULL);
	}

	print_stack();
	printf("program ends, stack top = %p\n", p_top);
	printf("ABA problem test passed!\n");

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
