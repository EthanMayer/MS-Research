from libc.stdlib cimport malloc, free

# import the interface function declaration from the header file
cdef extern from "cthreads.h":
    int start_test(int arr[], int arrSize)
    
cpdef int main(tup):
    print("======== Starting Comp.pyx ========")
    # print("Cython tuple: " + str(tup))
    cdef int n = len(tup)
    cdef int[3] arr
    cdef int i = 0
    for i in range(n):
        arr[i] = tup[i]
        # print(str(i) + ": " + str(arr[i]) + ", " + str(tup[i]))
    print("Cython C array: " + str(arr))
    # print("Starting C code")
    cdef int val = start_test(arr, n)
    print("Returned with value ")
    return 0
