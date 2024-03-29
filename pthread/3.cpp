#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

const int NUM_THREADS = 16;

struct ThreadData {
    int tid;
};

int global_x;
bool global_ready[NUM_THREADS];
pthread_mutex_t lock_x;


void* thrFunc(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    
	while (true) {
		pthread_mutex_lock(&lock_x);

		if (data->tid == 0 || global_ready[data->tid - 1]) {
			pthread_mutex_unlock(&lock_x);
			break;
		}

		pthread_mutex_unlock(&lock_x);
	}

    pthread_mutex_lock(&lock_x);
    
    printf("My id = %2d, x = %d\n", data->tid, ++global_x);
	global_ready[data->tid] = true;
    
    pthread_mutex_unlock(&lock_x);
    
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
	memset(global_ready, false, sizeof(global_ready));

    ThreadData thrData[NUM_THREADS];

    pthread_t thr[NUM_THREADS];

    pthread_mutex_init(&lock_x, NULL);

    for (int i = 0; i < NUM_THREADS; ++i)     {
        thrData[i].tid = i; 

        int rc = 0;

        if ((rc = pthread_create(thr + i, NULL, thrFunc, thrData + i))) {
            printf("Failed to create thread! rc = %d\n,", rc);
            
            perror("Error");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_join(thr[i], NULL);

    return 0;
}
