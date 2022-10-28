#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include "cthreads.h"
#include <zmq.h>
//#include <czmq.h>

// error handling
void error(char* msg) {
    printf("%s: %s\n", msg, strerror(errno));
    fflush(stdout);
    exit(1);
}

void* thread1() {
    long thread = pthread_self();
    printf("Thread %ld started\n", thread);
    fflush(stdout);

    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    if (zmq_connect(subscriber, "tcp://127.0.0.1:5556") != 0) {
        error("Could not connect to pub socket\n");
    }
    if (zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0) != 0) {
        error("Could not set sock opt for sub socket\n");
    }

    char message[12];

    for (int i = 0; i < 100; i++)
    {
        if (zmq_recv(subscriber, message, 12, 0) == -1) {
            error("Could not receive on pub socket\n");
        }
        printf("%d: %s\n", i, message);
    }

    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    // void *context = zmq_ctx_new();
    // void *worker = zmq_socket(context, ZMQ_SUB);

    // zmq_connect(worker, "tcp://localhost:5671");
    
    // int total = 0;
    // while (1) {
    //     printf("Thread %ld entered while loop\n", thread);
    //     fflush(stdout);
    //     //  Tell the broker we're ready for work
    //     s_send(worker, "Hi Boss");
    //     printf("Thread %ld sent 'Hi Boss'\n", thread);
    //     fflush(stdout);

    //     //  Get workload from broker, until finished
    //     char *workload = s_recv(worker);
    //     printf("Thread %ld received %s\n", thread, workload);
    //     fflush(stdout);
    //     int finished = (strcmp(workload, "Fired!") == 0);
    //     free(workload);
    //     if (finished) {
    //         printf("Completed: %d tasks\n", total);
    //         fflush(stdout);
    //         break;
    //     }
    //     total++;

    //     //  Do some random work
    //     s_sleep(randof(500) + 1);
    // }
    // zmq_close(worker);
    // zmq_ctx_destroy(context);
    printf("Thread %ld exiting\n", thread);
    fflush(stdout);
    return NULL;
}

// void* thread2() {

// }

// the function to be called from Cython
int start_test() {
    printf("Entered C code\n");
    fflush(stdout);
    pthread_t t1;
    pthread_t t2;

    // void *context = zmq_ctx_new();
    // void *broker = zmq_socket(context, ZMQ_PUB);

    // zmq_bind(broker, "tcp://*:5671");
    // srandom((unsigned)time(NULL));

    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUB);
    if (zmq_bind(publisher, "tcp://127.0.0.1:5556") != 0) {
        error("Could not bind pub socket\n");
    }

    if (pthread_create(&t1, NULL, &thread1, NULL) == -1) {
        error("Can't create thread\n");
    }
    // if (pthread_create(&t2, NULL, &thread2, NULL) == -1) {
    // error("Can't create thread\n");
    // }
    
    //  Run for five seconds and then tell workers to end
    // int64_t end_time = s_clock() + 5000;
    // int workers_fired = 0;
    // while (1) {
    //     printf("Main entered while loop\n");
    //     fflush(stdout);
    //     //  Next message gives us least recently used worker
    //     char *identity = s_recv(broker);
    //     s_sendmore(broker, identity);
    //     free(identity);
    //     free(s_recv(broker));     //  Envelope delimiter
    //     free(s_recv(broker));     //  Response from worker
    //     s_sendmore(broker, "");

    //     printf("Main entering if statement\n");
    //     fflush(stdout);
    //     //  Encourage workers until it's time to fire them
    //     if (s_clock() < end_time)
    //         s_send(broker, "Work harder");
    //     else {
    //         s_send(broker, "Fired!");
    //         if (++workers_fired == 1)
    //             break;
    //     }
    // }



    while(1)
    {
        if (zmq_send(publisher, "Hello World!", 12, 0) != 12) {
            error("Pub send buffer length incorrect\n");
        }
    }

    zmq_close(publisher);
    zmq_ctx_destroy(context);

    // return 0;
    
    printf("Main joining thread\n");
    fflush(stdout);
    if (pthread_join(t1, NULL) == -1) {
        error("Can't join thread 1\n");
    }

    // if (pthread_join(&t2, NULL) == -1) {
    //     error("Can't join thread 2\n");
    // }

    // zmq_close(broker);
    // zmq_ctx_destroy(context);
    return 1;
}