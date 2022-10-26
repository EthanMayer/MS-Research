cimport numpy as np
import numpy as np
from functools import wraps
from libc.stdlib cimport malloc, free

# import the interface function declaration from the header file
cdef extern from "cthreads.h":
    char ** weather_work(char ** cities, int NUM_CITIES)
    
cdef extern from "Python.h":
    char* PyUnicode_AsUTF8(object unicode)

# converting python lists into C arrays to pass it to C code
cdef char ** to_c_string_list(str_list):
    cdef int l = len(str_list)
    cdef char **c_string = <char **>malloc(l * sizeof(char *))
    for i in range(l):
        c_string[i] = PyUnicode_AsUTF8(str_list[i])
    return c_string

# a function that splits the message into a tuple of values instead of a single string
cdef np.ndarray fill_array(np.ndarray arr, char ** carr, int l):
    cdef int i = 0
    for i in range(l):
        arr[i] = tuple(carr[i].decode().replace('"', "").split(","))
    
    # free the memory for the response array allocated in the C code
    for s in range(l):
        free(carr[s])
    free(carr)
    return arr

############## DECORATE AND MEMOIZE

def memoize(weather_query):
    queried = {}
    @wraps(weather_query)
    def memorize(cities):
        response = []
        # if a single string, convert it to list
        # C code takes an array
        if isinstance(cities, str):
            cities = [cities]
        # check which cities are already queried before
        for c in cities:
            if c in queried:
                # add the already queried data to the output 
                response.append(queried.get(c))
                # remove the city from the cities list to be queried
                #cities.remove(c) # for some reason remove doesn't work properly
        # this filter is used because .remove not working well
        cities = [c for c in cities if c not in queried]

        if len(cities) > 0:
            # call the C function on the shortlisted cities list
            weather = weather_query(cities)
            # add the new responses to the queried data
            for i in range(len(weather)):
                queried[weather[i][0]] = weather[i]
                response.append(weather[i])
        # return the desired output
        return response
    # return the wrapper function
    return memorize

## decorate the weather_query function
@memoize
def weather_query(cities):
    # convert python list to C array
    cdef char ** ccities = to_c_string_list(cities)
    cdef int ln = len(cities)
    # get responses
    cdef char ** msg = weather_work(ccities, ln)
    # define an empty numpy array to be filled with data
    cdef np.ndarray[dtype = object, ndim = 1] arr = np.empty(ln, dtype = object)
    # free the C array memory    
    free(ccities)
    # return the clean version of the output
    # the msg array will be free in the fill_array function defined above
    return fill_array(arr, msg, ln)