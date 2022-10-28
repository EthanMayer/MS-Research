#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include "cthreads.h"
#include "zhelpers.h"
//#include <czmq.h>

// error handling
void error(char* msg) {
    printf("%s: %s\n", msg, strerror(errno));
    fflush(stdout);
    exit(1);
}

void* thread1() {
    printf("Thread %ld started", pthread_self());
    fflush(stdout);

    void *context = zmq_ctx_new();
    void *worker = zmq_socket(context, ZMQ_REQ);

    zmq_connect(worker, "tcp://localhost:5671");

    int total = 0;
    while (1) {
        //  Tell the broker we're ready for work
        s_send(worker, "Hi Boss");

        //  Get workload from broker, until finished
        char *workload = s_recv(worker);
        int finished = (strcmp(workload, "Fired!") == 0);
        free(workload);
        if (finished) {
            printf("Completed: %d tasks\n", total);
            fflush(stdout);
            break;
        }
        total++;

        //  Do some random work
        s_sleep(randof(500) + 1);
    }
    zmq_close(worker);
    zmq_ctx_destroy(context);
    return NULL;
}

// void* thread2() {

// }

// the function to be called from Cython
int start_test() {
    printf("Entered C code");
    fflush(stdout);
    pthread_t t1;
    pthread_t t2;

    void *context = zmq_ctx_new();
    void *broker = zmq_socket(context, ZMQ_ROUTER);

    zmq_bind(broker, "tcp://*:5671");
    srandom((unsigned)time(NULL));

    if (pthread_create(&t1, NULL, &thread1, NULL) == -1) {
        error("Can't create thread\n");
    }
    // if (pthread_create(&t2, NULL, &thread2, NULL) == -1) {
    // error("Can't create thread\n");
    // }
    
    //  Run for five seconds and then tell workers to end
    int64_t end_time = s_clock() + 5000;
    int workers_fired = 0;
    while (1) {
        //  Next message gives us least recently used worker
        char *identity = s_recv(broker);
        s_sendmore(broker, identity);
        free(identity);
        free(s_recv(broker));     //  Envelope delimiter
        free(s_recv(broker));     //  Response from worker
        s_sendmore(broker, "");

        //  Encourage workers until it's time to fire them
        if (s_clock() < end_time)
            s_send(broker, "Work harder");
        else {
            s_send(broker, "Fired!");
            if (++workers_fired == 1)
                break;
        }
    }
    

    if (pthread_join(t1, NULL) == -1) {
        error("Can't join thread 1\n");
    }

    // if (pthread_join(&t2, NULL) == -1) {
    //     error("Can't join thread 2\n");
    // }

    zmq_close(broker);
    zmq_ctx_destroy(context);
    return 1;
}