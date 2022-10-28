from libc.stdlib cimport malloc, free

# import the interface function declaration from the header file
cdef extern from "cthreads.h":
    int start_test()
    
def start_thread_test():
    print("Starting C code")
    cdef int val = start_test()
    print("Returned with value ")