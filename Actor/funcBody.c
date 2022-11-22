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

    // Create thread1 receiver pair socket and connect to main's sender pair
    void *context = zmq_ctx_new();
    void *pair_t_r = zmq_socket(context, ZMQ_PAIR);
    if (zmq_bind(pair_t_r, "inproc://t1_r") != 0) {
        error("Could not bind thread1 receiver socket\n");
    }

    // Create thread1 sender pair socket and connect to main's receiver pair
    void *pair_t_s = zmq_socket(context, ZMQ_PAIR);
    if (zmq_bind(pair_t_s, "inproc://t1_s") != 0) {
        error("Could not bind thread1 sender socket\n");
    }
    if (zmq_connect(pair_t_s, "inproc://m_r") != 0) {
        error("Could not connect to main receiver socket\n");
    }

    // sleep(1);
    
    char* buf = "Ready";

    printf("Thread sending Ready signal\n");
    fflush(stdout);
    for (int i = 0; i < 1000; i++) {
        if (zmq_send(pair_t_s, buf, sizeof(buf), 0) != sizeof(buf)) {
            error("Pair send buffer length incorrect\n");
        }
    }

    int arr[3];

    // Receive information from main thread via socket
    // for (int i = 0; i < 100; i++) {
        // sleep(5);
        printf("Thread ready to receive array\n");
        fflush(stdout);
        if (zmq_recv(pair_t_r, arr, sizeof(arr), 0) == -1) {
            error("Could not receive on thread1 receive socket\n");
        }
        printf("Thread %ld: Received array of size %d: ", thread, sizeof(arr)/sizeof(arr[0]));
        for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        fflush(stdout);
    // }

    // Clean up socket
    zmq_close(pair_t_s);
    zmq_close(pair_t_r);
    zmq_ctx_destroy(context);

    printf("Thread %ld exiting\n", thread);
    fflush(stdout);
    return NULL;
}