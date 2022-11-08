#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include "cthreads.h"
#include <zmq.h>
#include <unistd.h>
#include <dlfcn.h>
//#include <czmq.h>

// error handling
void error(char* msg) {
    printf("%s: %s\n", msg, strerror(errno));
    fflush(stdout);
    exit(1);
}

// void* thread1() {
//     long thread = pthread_self();
//     printf("Thread %ld started\n", thread);
//     fflush(stdout);

//     void *context = zmq_ctx_new();
//     void *subscriber = zmq_socket(context, ZMQ_SUB);
//     if (zmq_connect(subscriber, "tcp://127.0.0.1:5556") != 0) {
//         error("Could not connect to pub socket\n");
//     }
//     if (zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0) != 0) {
//         error("Could not set sock opt for sub socket\n");
//     }

//     int arr[3];

//     for (int i = 0; i < 100; i++)
//     {
//         sleep(5);
//         if (zmq_recv(subscriber, arr, sizeof(arr), 0) == -1) {
//             error("Could not receive on pub socket\n");
//         }
//         printf("Thread %ld: Received array of size %d: ", thread, sizeof(arr)/sizeof(arr[0]));
//         for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) {
//             printf("%d ", arr[i]);
//         }
//         printf("\n");
//         fflush(stdout);
//     }

//     zmq_close(subscriber);
//     zmq_ctx_destroy(context);

//     printf("Thread %ld exiting\n", thread);
//     fflush(stdout);
//     return NULL;
// }

// void* thread2() {

// }

// the function to be called from Cython
int start_test(int arr[], int arrSize) {
    printf("Entered C code\n");
    printf("C code array: %d, %d, %d\n", arr[0], arr[1], arr[2]);
    fflush(stdout);
    pthread_t t1;
    //pthread_t t2;

    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUB);
    if (zmq_bind(publisher, "tcp://127.0.0.1:5556") != 0) {
        error("Could not bind pub socket\n");
    }

    char* libpath = "funcBody.so";
    void* libhandle = dlopen(libpath, RTLD_LAZY);

    if (libhandle == NULL) {
        error("Could not open shared library");
    }

    void (*thread1) = dlsym(libhandle, "thread1");

    if (pthread_create(&t1, NULL, thread1, NULL) == -1) {
        error("Can't create thread\n");
    }
    // if (pthread_create(&t2, NULL, &thread2, NULL) == -1) {
    // error("Can't create thread\n");
    // }

    printf("Total elements in array: %d. Total size of array: %d.\n", arrSize, arrSize*8);
    fflush(stdout);

    while(1)
    {
        sleep(5);
        printf("Thread Main: Sent array of size %d: %d, %d, %d\n", arrSize, arr[0], arr[1], arr[2]);
        fflush(stdout);
        if (zmq_send(publisher, arr, arrSize*8, 0) != arrSize*8) {
            error("Pub send buffer length incorrect\n");
        }

    }

    zmq_close(publisher);
    zmq_ctx_destroy(context);
    
    printf("Main joining thread\n");
    fflush(stdout);
    if (pthread_join(t1, NULL) == -1) {
        error("Can't join thread 1\n");
    }

    // if (pthread_join(&t2, NULL) == -1) {
    //     error("Can't join thread 2\n");
    // }

    return 1;
}