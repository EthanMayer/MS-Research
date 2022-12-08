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
from cpython.pycapsule cimport PyCapsule_New, PyCapsule_IsValid, PyCapsule_GetPointer, PyCapsule_GetName
from pthread cimport pthread_create, pthread_join, pthread_t
from zmq.backend.cython cimport libzmq as z
import zmq
    
# Error handling
cpdef error(msg):
    print(msg + ": " + strerror(errno))
    exit(1)

# Destructor for pointers stored in PyCapsule (only needed for dynamically allocated pointers stored as PyCapsules)
cdef void free_ptr(object cap):
    free(PyCapsule_GetPointer(cap,PyCapsule_GetName(cap)))

# Wrap a void* socket pointer in a PyCapsule and store in a Python dictionary
cdef void store_sckt(const char* name, void* sckt, portDict):
    portDict[name] = PyCapsule_New(sckt, name, NULL)

# Unrwap a void* socket pointer stored in a PyCapsule that is stored in a Python dictionary
cdef void* get_sckt(const char* name, portDict):
    # First check to ensure capsule is valid
    if (PyCapsule_IsValid(portDict[name], name)):
        return PyCapsule_GetPointer(portDict[name], name)
    else:
        error("Could not retrieve socket pointer from dictionary in comp.pyx")

# Main Cython function to call C
cpdef tuple main(tup, tup2):
    print("======== Comp.pyx ========")
    cdef int* arr2 = <int*> malloc(n * sizeof(int)) # Array for return values
    cdef pthread_t t1    # Thread 1's ID
    portDict = {}   # Dictionary for ports

    # Convert python tuple to C array
    cdef int n = len(tup)
    cdef int* arr = <int*> malloc(n * sizeof(int))
    cdef int i = 0
    for i in range(n):
        arr[i] = tup[i]

    # Create socket to communicate with actor.py (Python)
    context = zmq.Context()
    socket = context.socket(zmq.PAIR)
    socket.connect("tcp://127.0.0.1:5555")

    # Send actor the Ready signal
    print("Cython: Sending 'Ready' signal")
    socket.send_string("Ready")

    # Wait for function from actor
    func = socket.recv()
    print("Cython: Received function" + str(func))

    # Create main thread pair socket and bind to IP
    cdef void* ctx = z.zmq_ctx_new()
    cdef void* sckt = z.zmq_socket(ctx, z.ZMQ_PAIR)

    if (z.zmq_bind(sckt, "tcp://127.0.0.1:5556") != 0):
        error("Could not bind comp.pyx PAIR socket address")

    # Store socket in dictionary
    store_sckt("Thread1", sckt, portDict)

    # Open .so shared library and grab function from it
    cdef char* libpath = "funcBody.so";
    cdef void* libhandle = dlopen(libpath, RTLD_LAZY);

    if (libhandle == NULL):
        error("Could not open shared library in comp.pyx")

    # Create thread with the .so function as body
    cdef void *thread1 = dlsym(libhandle, func)

    if (pthread_create(&t1, NULL, thread1, NULL) == -1):
        error("Could not create thread in comp.pyx")

    # Receive "Ready" message to know the thread is ready
    cdef char buf[6]
    if (z.zmq_recvbuf(get_sckt("Thread1", portDict), buf, sizeof(buf), 0) == -1):
        error("Could not receive on comp.pyx PAIR socket")
    print("Cython: received " + str(buf))

    # Send array size via socket to threads
    cdef char sizeBuf[256]
    sprintf(sizeBuf, "%d", n)
    if (z.zmq_sendbuf(get_sckt("Thread1", portDict), sizeBuf, sizeof(sizeBuf), 0) != sizeof(sizeBuf)):
        error("Comp.pyx PAIR send message buffer length incorrect")
    print("Cython: Sent array size")

    # Send array via socket to thread
    if (z.zmq_sendbuf(get_sckt("Thread1", portDict), arr, n * sizeof(int), 0) != n * sizeof(int)):
        error("Comp.pyx PAIR send message buffer length incorrect\n")
    print("Cython: Sent array")

    # Receive array back via socket from thread
    if (z.zmq_recvbuf(get_sckt("Thread1", portDict), arr2, n * sizeof(int), 0) == -1):
        error("Could not receive on comp.pyx PAIR socket")
    
    # Convert received array to Python tuple
    # tup2 = []
    for i in range(n):
        tup2.append(int(arr2[i]))
    tup2 = tuple(tup2)
    print("Cython: Received tuple: " + str(tup2))
    free(arr2)

    # Clean up socket
    if (z.zmq_close(get_sckt("Thread1", portDict)) == -1):
        error("Could not close comp.pyx PAIR socket")
    if (z.zmq_ctx_destroy(ctx) == -1):
        error("Could not destroy comp.pyx ZMQ context")

    # Join thread
    print("Cython: Joining thread")
    if (pthread_join(t1, NULL) == -1):
        error("Could not join thread1 in comp.pyx")

    # Clean up PyZMQ
    socket.close()
    context.destroy()

    # Return tuple to Python
    print("Sending C array to Python as tuple")
    return tup2