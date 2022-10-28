#ifndef CTHREADS_H
#define CTHREADS_H

// error handling
void error(char* msg);
// the function to be parallelized
void* thread1();
// the function to be wrapped
int start_test();

#endif