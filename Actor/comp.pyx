#   comp.pyx
#    
#   Author - Ethan Mayer
#   Fall 2022  

# Import functions from the c standard library
from libc.stdlib cimport malloc, free
from posix.dlfcn cimport dlopen, dlsym, RTLD_LAZY
from libc.string cimport strerror
from libc.errno cimport errno
from libc.stdio cimport sprintf
from pthread cimport pthread_create, pthread_join, pthread_t
from zmq.backend.cython cimport libzmq as z
    
# Error handling
cpdef error(msg):
    print(msg + ": " + strerror(errno))
    exit(1)

# Main Cython function to call C
cpdef tuple main(tup):
    print("======== Comp.pyx ========")
    cdef int* arr2  # Array for return values
    cdef pthread_t t1    # Thread 1's ID
    portDict = {}   # Dictionary for ports

    # Convert python tuple to C array
    cdef int n = len(tup)
    cdef int* arr = <int*> malloc(n * sizeof(int))
    cdef int i = 0
    for i in range(n):
        arr[i] = tup[i]

    # Create main thread pair socket
    cdef void* context = z.zmq_ctx_new()
    cdef void* sckt = z.zmq_socket(context, z.ZMQ_PAIR)

    if (z.zmq_bind(sckt, "tcp://127.0.0.1:5556") != 0):
        error("Could not bind main receiver socket")

    # Create entry for thread1's socket and add it to dictionary
    portDict["thread1"] = sckt

    # Open .so shared library and grab function from it
    cdef char* libpath = "funcBody.so";
    cdef void *libhandle = dlopen(libpath, RTLD_LAZY);

    if (libhandle == NULL):
        error("Could not open shared library")

    # Create thread with the .so function as body
    cdef void *thread1 = dlsym(libhandle, "thread1")

    if (pthread_create(&t1, NULL, thread1, NULL) == -1):
        error("Can't create thread")

    # Receive "Ready" message to know the thread is ready
    cdef char buf[6]
    if (z.zmq_recvbuf(sckt, buf, sizeof(buf), 0) == -1):
        error("Could not receive on main receive socket")
    print("Main received: " + str(buf))

    # Send array size via socket to threads
    cdef char sizeBuf[256]
    sprintf(sizeBuf, "%d", n)
    if (z.zmq_sendbuf(sckt, sizeBuf, sizeof(sizeBuf), 0) != sizeof(sizeBuf)):
        error("Pair send buffer length incorrect\n")
    print("Main: Sent array size")
    # socket.send_pyobj(n)

    # Send array via socket to thread
    if (z.zmq_sendbuf(sckt, arr, n * sizeof(int), 0) != n * sizeof(int)):
        error("Pair send buffer length incorrect\n")
    print("Main: Sent array")

    # Receive array back via socket from thread
    if (z.zmq_recvbuf(sckt, arr2, n * sizeof(int), 0) == -1):
        error("Could not receive on main receive socket")
    
    # Convert received array to Python tuple
    tup2 = []
    for i in range(n):
        arr2[i] = tup2[i]
    free(arr2)

    # Clean up socket
    z.zmq_close(sckt)
    z.zmq_ctx_destroy(context)

    # Join thread
    print("Main: Joining thread\n")
    if (pthread_join(t1, NULL) == -1):
        error("Can't join thread 1")

    # Return tuple to Python
    print("Sending C array to Python as tuple")
    return tup2