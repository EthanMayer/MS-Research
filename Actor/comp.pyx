#   comp.pyx
#    
#   Author - Ethan Mayer
#   Fall 2022  

# Import functions from the c standard library
from libc.stdlib cimport malloc, free

# Import the interface function declaration from the header file
cdef extern from "cthreads.h":
    int* start_test(int arr[], int arrSize)
    
# Main Cython function to call C
cpdef tuple main(tup):
    print("======== Comp.pyx ========")

    # Convert tuple to C array
    cdef int n = len(tup)
    cdef int* arr = <int*> malloc(n * sizeof(int))
    cdef int i = 0
    for i in range(n):
        arr[i] = tup[i]

    # Allocate array for return values
    cdef int* arr2

    # Call C file
    print("Sending tuple to C as C array")
    try:
        arr2 = start_test(arr, n)
    finally:
        free(arr)
    
    # Convert C array to tuple
    tup2 = []
    for i in range(n):
        tup2.append(int(arr2[i]))
    tup2 = tuple(tup2)
    free(arr2)

    # Return tuple to Python
    print("======== Comp.pyx ========")
    print("Sending C array to Python as tuple")
    return tup2