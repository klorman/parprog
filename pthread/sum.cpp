#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

const int NUM_THREADS = 16;
long long N;

struct ThreadData {
    int tid;
};

long double global_sum = 0;

pthread_mutex_t lock_x;

void* thrFunc(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    long double res = 0;

    for(long long i = data->tid; i <= N; i += NUM_THREADS) {
        if (i != 0) {
            res += 1.0 / (double)i;
        }
    }

    pthread_mutex_lock(&lock_x);

    global_sum += res;

    pthread_mutex_unlock(&lock_x);

    pthread_exit(NULL);
}

int main(int argc, char** argv) {    
    N = atoll(argv[1]);

    if(N < 0) {
        printf("N must be non-negative\n");
        exit(EXIT_FAILURE);
    }

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

    printf("Sum = %Lf\n", global_sum);

    return 0;
}
