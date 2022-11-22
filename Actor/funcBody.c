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
    printf("Thread %ld: started\n", thread);
    fflush(stdout);

    // Create thread1 receiver pair socket and connect to main's sender pair
    void *context = zmq_ctx_new();
    void *sckt = zmq_socket(context, ZMQ_PAIR);
    if (zmq_connect(sckt, "tcp://127.0.0.1:5556") != 0) {
        error("Could not connect to main socket\n");
    }

    // sleep(1);
    
    char* buf = "Ready";

    printf("Thread %ld: sending Ready signal\n", thread);
    fflush(stdout);
    // for (int i = 0; i < 1000; i++) {
        if (zmq_send(sckt, buf, sizeof(buf), 0) != sizeof(buf)) {
            error("Pair send buffer length incorrect\n");
        }
    // }

    int arr[3];

    // Receive information from main thread via socket
    // for (int i = 0; i < 100; i++) {
        // sleep(5);
        printf("Thread %ld: ready to receive array\n", thread);
        fflush(stdout);
        if (zmq_recv(sckt, arr, sizeof(arr), 0) == -1) {
            error("Could not receive on thread1 receive socket\n");
        }
        printf("Thread %ld: Received array of size %d: ", thread, sizeof(arr)/sizeof(arr[0]));
        for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        fflush(stdout);
    // }

    printf("Thread %ld: sending array back\n", thread);
    fflush(stdout);
    if (zmq_send(sckt, arr, sizeof(arr), 0) != sizeof(arr)) {
        error("Pair send buffer length incorrect\n");
    }

    // Clean up socket
    zmq_close(sckt);
    zmq_ctx_destroy(context);

    printf("Thread %ld: exiting\n", thread);
    fflush(stdout);
    return NULL;
}