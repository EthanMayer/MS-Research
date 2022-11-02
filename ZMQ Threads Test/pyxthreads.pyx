from libc.stdlib cimport malloc, free

# import the interface function declaration from the header file
cdef extern from "cthreads.h":
    int start_test(int arr[3])
    
cpdef int start_thread_test(tup):
    print("Cython tuple: " + str(tup))
    cdef int n = len(tup)
    cdef int[3] arr
    cdef int i = 0
    for i in range(n):
        arr[i] = tup[i]
        print(str(i) + ": " + str(arr[i]) + ", " + str(tup[i]))
    print("Cython array: " + str(arr))
    print("Starting C code")
    cdef int val = start_test(arr)
    print("Returned with value ")
    return 0
