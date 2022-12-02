#   comp.pyx
#    
#   Author - Ethan Mayer
#   Fall 2022  

# Import functions from the c standard library
from libc.stdlib cimport malloc, free
from posix.dlfcn cimport dlopen, dlsym, RTLD_LAZY
from libc.string cimport strerror
from libc.errno cimport errno
from pthread cimport pthread_create, pthread_join, pthread_t

# Import the interface function declaration from the header file
cdef extern from "cthreads.h":
    int* start_test(int arr[], int arrSize)
    
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

    # Create entry for thread1's socket and add it to dictionary

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

    # Send array size via socket to threads

    # Send array via socket to thread

    # Receive array back via socket from thread

    # Clean up socket

    # Join thread
    print("Main: Joining thread\n")
    if (pthread_join(t1, NULL) == -1):
        error("Can't join thread 1")

    # Call C file
    #print("Sending tuple to C as C array")
    #try:
    #    arr2 = start_test(arr, n)
    #finally:
    #    free(arr)
    
    # Convert C array to tuple
    #tup2 = []
    #for i in range(n):
    #    tup2.append(int(arr2[i]))
    #tup2 = tuple(tup2)
    #free(arr2)

    # Return tuple to Python
    print("======== Comp.pyx ========")
    print("Sending C array to Python as tuple")
    tup2 = []
    return tup2