#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

const int NUM_THREADS = 16;

struct ThreadData {
    int tid;
    double stuff;
};

void* thrFunc(void* arg)
{
    ThreadData* data = (ThreadData*)arg;
    
    printf("hello World! My id = %2d, Threads number = %d]\n", data->tid, NUM_THREADS);
    
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    ThreadData thrData[NUM_THREADS];

    pthread_t thr[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
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
