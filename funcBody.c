//  funcBody.c
//    
//  Author - Ethan Mayer
//  Fall 2022

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <zmq.h>

// Error handling
void error(char* msg) {
    printf("%s: %s\n", msg, strerror(errno));
    fflush(stdout);
    exit(1);
}

// Function to be run on a thread to send back data
void* send_back() {
    // Debug identify self
    long thread = pthread_self();
    printf("Thread %ld: Started send_back function\n", thread);
    fflush(stdout);

    // Create thread1 receiver pair socket and connect to main's sender pair
    void *context = zmq_ctx_new();
    void *sckt = zmq_socket(context, ZMQ_PAIR);
    if (zmq_connect(sckt, "tcp://127.0.0.1:5556") != 0) {
        error("Could not connect to main socket\n");
    }
    
    // Send the Ready message to main thread
    char* buf = "Ready";
    printf("Thread %ld: Sending 'Ready' signal\n", thread);
    fflush(stdout);
    if (zmq_send(sckt, buf, sizeof(buf), 0) != sizeof(buf)) {
        error("Pair send buffer length incorrect\n");
    }

    // Get the size of the incoming array
    char sizeBuf[256];
    if (zmq_recv(sckt, sizeBuf, sizeof(sizeBuf), 0) == -1) {
        error("Could not receive on thread1 receive socket\n");
    }
    int arrSize = atoi(sizeBuf);
    printf("Thread %ld: Received array size %d\n", thread, arrSize);

    // Receive information from main thread via socket
    int arr[arrSize];
    printf("Thread %ld: Ready to receive array\n", thread);
    fflush(stdout);
    if (zmq_recv(sckt, arr, arrSize * sizeof(int), 0) == -1) {
        error("Could not receive on thread1 receive socket\n");
    }
    printf("Thread %ld: Received array: ", thread);
    for (int i = 0; i < arrSize; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    fflush(stdout);

    // Send return array size to main thread
    char sizeBuf2[256];
    sprintf(sizeBuf2, "%d", arrSize);
    printf("Thread %ld: Sending array size %d\n", thread, arrSize);
    fflush(stdout);
    if (zmq_send(sckt, sizeBuf2, sizeof(sizeBuf2), 0) != sizeof(sizeBuf2)) {
        error("Pair send buffer length incorrect\n");
    }

    // Send information back to main thread
    printf("Thread %ld: Sending array back\n", thread);
    fflush(stdout);
    if (zmq_send(sckt, arr, arrSize * sizeof(int), 0) != arrSize * sizeof(int)) {
        error("Pair send buffer length incorrect\n");
    }

    // Clean up socket
    zmq_close(sckt);
    zmq_ctx_destroy(context);

    // Exit
    printf("Thread %ld: Exiting\n", thread);
    fflush(stdout);
    return NULL;
}

// Function to be run on a thread to add data
void* add() {
    // Debug identify self
    long thread = pthread_self();
    printf("Thread %ld: Started send_back function\n", thread);
    fflush(stdout);

    // Create thread1 receiver pair socket and connect to main's sender pair
    void *context = zmq_ctx_new();
    void *sckt = zmq_socket(context, ZMQ_PAIR);
    if (zmq_connect(sckt, "tcp://127.0.0.1:5556") != 0) {
        error("Could not connect to main socket\n");
    }
    
    // Send the Ready message to main thread
    char* buf = "Ready";
    printf("Thread %ld: Sending 'Ready' signal\n", thread);
    fflush(stdout);
    if (zmq_send(sckt, buf, sizeof(buf), 0) != sizeof(buf)) {
        error("Pair send buffer length incorrect\n");
    }

    // Get the size of the incoming array
    char sizeBuf[256];
    if (zmq_recv(sckt, sizeBuf, sizeof(sizeBuf), 0) == -1) {
        error("Could not receive on thread1 receive socket\n");
    }
    int arrSize = atoi(sizeBuf);
    printf("Thread %ld: Received array size %d\n", thread, arrSize);

    // Receive information from main thread via socket
    int arr[arrSize];
    printf("Thread %ld: Ready to receive array\n", thread);
    fflush(stdout);
    if (zmq_recv(sckt, arr, arrSize * sizeof(int), 0) == -1) {
        error("Could not receive on thread1 receive socket\n");
    }
    printf("Thread %ld: Received array: ", thread);

    // Calculate total from received array
    int total = 0;
    for (int i = 0; i < arrSize; i++) {
        printf("%d ", arr[i]);
        total += arr[i];
    }
    printf("\n");
    fflush(stdout);
    arrSize = 1;
    int arr2[] = {total};

    // Send return array size to main thread
    char sizeBuf2[256];
    sprintf(sizeBuf2, "%d", arrSize);
    printf("Thread %ld: Sending array size %d\n", thread, arrSize);
    fflush(stdout);
    if (zmq_send(sckt, sizeBuf2, sizeof(sizeBuf2), 0) != sizeof(sizeBuf2)) {
        error("Pair send buffer length incorrect\n");
    }

    // Send information back to main thread
    printf("Thread %ld: Sending array back\n", thread);
    fflush(stdout);
    if (zmq_send(sckt, arr2, arrSize * sizeof(int), 0) != arrSize * sizeof(int)) {
        error("Pair send buffer length incorrect\n");
    }

    // Clean up socket
    zmq_close(sckt);
    zmq_ctx_destroy(context);

    // Exit
    printf("Thread %ld: Exiting\n", thread);
    fflush(stdout);
    return NULL;
}