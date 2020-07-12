#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <assert.h>


void* worker_thread(void)
{
	return 0;
}

int main(int argc, char* argv[])
{
	int nThread, i, j;
	if (argc < 2) {
		printf("Usage: %s <thread number>.\n", argv[0]);
		exit(1);
	}
	nThread = atoi(argv[1]);
	printf("Creating %d threads...\n", nThread);
	pthread_t *hThread = (pthread_t*)malloc(sizeof(pthread_t) * nThread);
	for (i=0; i < nThread; i++) {
		pthread_create(&hThread[i], NULL, worker_thread, NULL);
	}
	for (i=0; i < nThread; i++) {
		pthread_join(hThread[i], NULL);
	}
	printf("Program end.\n");
	return 0;
}



