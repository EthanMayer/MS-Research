import cython

cpdef list bubbleSort1(list arr):
    cdef int n = len(arr)
    cdef bint swapped = False
    cdef int i = 0
    cdef int j = 0
    for i in range(n-1):
        for j in range(0, n-i-1):
            if arr[j] > arr[j + 1]:
                swapped = True
                arr[j], arr[j + 1] = arr[j + 1], arr[j]        
        if not swapped:
            break
    return arr

def bubbleSort2(arr):
    n = len(arr)
    swapped = False
    for i in range(n-1):
        for j in range(0, n-i-1):
            if arr[j] > arr[j + 1]:
                swapped = True
                arr[j], arr[j + 1] = arr[j + 1], arr[j]        
        if not swapped:
            break
    return arr

cpdef list bubbleSort3(list arr):
    return bubbleSortC(arr)

def bubbleSort4(arr):
    return bubbleSortC(arr)

cdef list bubbleSortC(list arr):
    cdef int n = len(arr)
    cdef bint swapped = False
    cdef int i = 0
    cdef int j = 0
    for i in range(n-1):
        for j in range(0, n-i-1):
            if arr[j] > arr[j + 1]:
                swapped = True
                arr[j], arr[j + 1] = arr[j + 1], arr[j]        
        if not swapped:
            break
    return arr