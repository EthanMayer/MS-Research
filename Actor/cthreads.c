//  cthreads.c
//    
//  Author - Ethan Mayer
//  Fall 2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <zmq.h>
#include <dlfcn.h>
#include "uthash.h"
#include "cthreads.h"

// Hash table for ZMQ ports
struct port_dict {
    char* name;             // The key is the name of the port
    void* port;      // Storage is a pointer to ZMQ port
    UT_hash_handle hh; /* makes this structure hashable */
};

// Initialize list of ports
struct port_dict *ports = NULL;

// Adds a port structure to the list
void add_port(struct port_dict *s) {
    HASH_ADD_INT(ports, name, s);
}

// Finds a port in the list by looking up key (name)
struct port_dict *find_port(char* port_name) {
    struct port_dict *s;

    HASH_FIND_INT(ports, &port_name, s);
    return s;
}

// Deletes a port from the list
void delete_user(struct port_dict *port) {
    HASH_DEL(ports, port);
}

// Error handling
void error(char* msg) {
    printf("%s: %s\n", msg, strerror(errno));
    fflush(stdout);
    exit(1);
}

// Function to be called from Cython
int* start_test(int arr[], int arrSize) {
    pthread_t t1;

    // Debug print to ensure array is correct
    printf("======== Entered C code ========\n");
    printf("Main: C array: ");
    fflush(stdout);
    for(int i = 0; i < arrSize - 1; i++) {
        printf("%d, ", arr[i]);
        fflush(stdout);
    }
    printf("%d\n", arr[arrSize - 1]);
    printf("Main: Total elements in array: %d. Total size of array: %d.\n", arrSize, arrSize*8);
    fflush(stdout);

    // Create main thread pair socket
    void *context = zmq_ctx_new();
    void *sckt = zmq_socket(context, ZMQ_PAIR);

    if (zmq_bind(sckt, "tcp://127.0.0.1:5556") != 0) {
        error("Could not bind main receiver socket\n");
    }

    // Create entry for thread1's socket and add it to the hash table
    struct port_dict t_1 = {
        .name = "thread1",
        .port = sckt
    };

    add_port(&t_1);

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

    // Receive "Ready" message to know the thread is ready
    char buf[6];
    if (zmq_recv(sckt, buf, sizeof(buf), 0) == -1) {
        error("Could not receive on main receive socket\n");
    }
    printf("Main: Received '%s'\n", buf);
    fflush(stdout);

    // Send array size via socket to thread
    char sizeBuf[256];
    sprintf(sizeBuf, "%d", arrSize);
    printf("Main: Sent array size\n");
    fflush(stdout);
    if (zmq_send(sckt, sizeBuf, sizeof(sizeBuf), 0) != sizeof(sizeBuf)) {
        error("Pair send buffer length incorrect\n");
    }

    // Send array via socket to thread
    printf("Main: Sent array\n");
    fflush(stdout);
    if (zmq_send(sckt, arr, arrSize * sizeof(int), 0) != arrSize * sizeof(int)) {
        error("Pair send buffer length incorrect\n");
    }

    // Receive array back via socket from thread
    int* arr2 = malloc(arrSize * sizeof(int));
    if (zmq_recv(sckt, arr2, arrSize * sizeof(int), 0) == -1) {
        error("Could not receive on main receive socket\n");
    }
    printf("Main: Received array of size %d: ", arrSize);
    for (int i = 0; i < arrSize; i++) {
        printf("%d ", arr2[i]);
    }
    printf("\n");
    fflush(stdout);

    // Clean up socket
    zmq_close(sckt);
    zmq_ctx_destroy(context);
    
    // Join thread
    printf("Main: Joining thread\n");
    fflush(stdout);
    if (pthread_join(t1, NULL) == -1) {
        error("Can't join thread 1\n");
    }

    // Return array to Cython
    return arr2;
}