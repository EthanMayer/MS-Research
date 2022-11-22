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

// Error handling
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

// Function to be called from Cython
int start_test(int arr[], int arrSize) {
    pthread_t t1;

    // Debug print to ensure array is correct
    printf("======== Entered C code ========\n");
    printf("C code array: ");
    fflush(stdout);
    for(int i = 0; i < arrSize - 2; i++) {
        printf("%d, ", arr[i]);
        fflush(stdout);
    }
    printf("%d\n", arr[arrSize - 1]);
        printf("Total elements in array: %d. Total size of array: %d.\n", arrSize, arrSize*8);
    fflush(stdout);

    // Create main sender pair socket
    void *context = zmq_ctx_new();
    void *pair_m_s = zmq_socket(context, ZMQ_PAIR);
    if (zmq_bind(pair_m_s, "inproc://m_s") != 0) {
        error("Could not bind main sender socket\n");
    }

    // Create main receiver pair socket
    void *pair_m_r = zmq_socket(context, ZMQ_PAIR);
    if (zmq_bind(pair_m_r, "inproc://m_r") != 0) {
        error("Could not bind main receiver socket\n");
    }

    // Open .so shared library and grab function from it
    char* libpath = "funcBody.so";
    void* libhandle = dlopen(libpath, RTLD_LAZY);

    if (libhandle == NULL) {
        error("Could not open shared library");
    }

    // Create thread with the .so function as body
    void (*thread1) = dlsym(libhandle, "thread1");

    if (pthread_create(&t1, NULL, thread1, NULL) == -1) {
        error("Can't create thread\n");
    }

    char* buf = "";
    if (zmq_recv(pair_m_r, buf, sizeof(buf), 0) == -1) {
        error("Could not receive on main receive socket\n");
    }
    printf("Received: %s", buf);
    fflush(stdout);

    if (zmq_connect(pair_m_s, "inproc://t1_r") != 0) {
        error("Could not connect to thread1 receiver socket\n");
    }

    // Send array via socket to thread
    // while(1) {
        // sleep(5);
        printf("Thread Main: Sent array\n");
        fflush(stdout);
        if (zmq_send(pair_m_s, arr, arrSize*8, 0) != arrSize*8) {
            error("Pair send buffer length incorrect\n");
        }
    // }

    // Clean up socket
    zmq_close(pair_m_s);
    zmq_close(pair_m_r);
    zmq_ctx_destroy(context);
    
    // Join thread
    printf("Main joining thread\n");
    fflush(stdout);
    if (pthread_join(t1, NULL) == -1) {
        error("Can't join thread 1\n");
    }

    return 1;
}