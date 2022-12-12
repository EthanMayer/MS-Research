#   pthreads.pxd
#   
#   Author - Ethan Mayer
#   Fall 2022
#
#   This is the Cython definition/header file for pthread.h.
#   This allows Cython to call functions defined here for pthreads. Some functions included here are for future-proofing purposes and are not currently used.

from posix.signal cimport sigset_t

cdef extern from "<pthread.h>" nogil:
    ctypedef int pthread_t

    ctypedef struct pthread_attr_t:
        pass
    ctypedef struct pthread_mutexattr_t:
        pass
    ctypedef struct pthread_mutex_t:
        pass

    enum:
        PTHREAD_CANCEL_ENABLE
        PTHREAD_CANCEL_DISABLE

    int pthread_cancel(pthread_t thread)
    int pthread_setcancelstate(int state, int *oldstate)
    pthread_t pthread_self()
    int pthread_sigmask(int how, sigset_t *set, sigset_t *oldset)
    int pthread_equal(pthread_t t1, pthread_t t2)
    int pthread_create(pthread_t *thread, pthread_attr_t *attr,
                       void *start_routine, void *arg)
    int pthread_join(pthread_t thread, void **retval)
    int pthread_kill(pthread_t thread, int sig)

    int pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *mutexattr)
    int pthread_mutex_lock(pthread_mutex_t *mutex)
    int pthread_mutex_unlock(pthread_mutex_t *mutex)