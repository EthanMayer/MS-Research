#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <zmq.h>

// Error handling
void error(char* msg) {
    printf("%s: %s\n", msg, strerror(errno));
    fflush(stdout);
    exit(1);
}

// Function to be run on a thread
void* thread1() {
    // Debug identify self
    long thread = pthread_self();
    printf("Thread %ld started\n", thread);
    fflush(stdout);

    // Create subscriber socket and connect to main thread publisher socket
    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    if (zmq_connect(subscriber, "tcp://127.0.0.1:5556") != 0) {
        error("Could not connect to pub socket\n");
    }
    if (zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0) != 0) {
        error("Could not set sock opt for sub socket\n");
    }

    int arr[3];

    // Receive information from main thread via socket
    for (int i = 0; i < 100; i++)
    {
        sleep(5);
        if (zmq_recv(subscriber, arr, sizeof(arr), 0) == -1) {
            error("Could not receive on pub socket\n");
        }
        printf("Thread %ld: Received array of size %d: ", thread, sizeof(arr)/sizeof(arr[0]));
        for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        fflush(stdout);
    }

    // Clean up socket
    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    printf("Thread %ld exiting\n", thread);
    fflush(stdout);
    return NULL;
}