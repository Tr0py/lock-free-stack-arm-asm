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
pthread_t hThread[4096];
static int threadNum = 0, loopNum = 0;
static char* elfname;

void stack_init(int);
void *worker(void *arg)
{
	int id = *(int*)arg;
	long tid = syscall(__NR_gettid);
	printf("thread starts, tid = %ld\n", tid);
	int i = loopNum;
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
        /* Check stack. */
		if (p == p->next) {
			printf("p == p->next. Stack is smashed.\n");
			exit(1);
		}

		p = p->next;
	}
}

int printUsage()
{
    printf("Usage: %s -t <threads number> -l <loop number>.\n"
            "Example: %s -t 16 -l 1000000.\n", elfname, elfname);
    exit(-1);
}


int main(int argc, char* argv[])
{
	int arg[1024];
    int opt;
    
    /* Parse args */
    elfname = argv[0];
    while ((opt = getopt(argc, argv, "t:l:")) != -1) {
        switch(opt) {
            case 't':
                threadNum = atoi(optarg);
                break;
            case 'l':
                loopNum = atoi(optarg);
                break;
            default:
                printUsage();
        }
    }
    /* Check args */
    if (!threadNum || !loopNum) {
        printUsage();
    }
    printf("Running with %d threads for %d times.", threadNum, loopNum);

    /* Init stack. */
	stack_init(threadNum + 10);
	printf("program starts, stack top = %p\n", p_top);
	printf("pop val = %p\n", pop());
	print_stack();
	printf("===============Creating threads=============\n");
	for (int i = 0; i < threadNum; ++i) {
		arg[i] = i;
		pthread_create(&hThread[i], NULL, worker, &arg[i]);
	}
	for (int i = 0; i < threadNum; ++i) {
		pthread_join(hThread[i], NULL);
	}

    /* Check stack. */
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
