#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* DoWork(void* args) {
    printf("ID: %lu, CPU: %d\n", pthread_self(), sched_getcpu());
    return 0;
}

void heavyComputation()
{
	//volatile unsigned long long i;
	unsigned long long i;

	for (i=0;i<1000000000ULL;i++);
}

int main() {   

    pthread_attr_t attr;
    cpu_set_t cpus;

    pthread_attr_init(&attr);
    int i;

    int numberOfProcessors = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Number of processors: %d\n", numberOfProcessors);

    pthread_t threads[numberOfProcessors];

    for (i = 0; i < numberOfProcessors; i++) {
       CPU_ZERO(&cpus);
       CPU_SET(i, &cpus);
       pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus);
       pthread_create(&threads[i], &attr, DoWork, NULL);
    }

    for (i = 0; i < numberOfProcessors; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
