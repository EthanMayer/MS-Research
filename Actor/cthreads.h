//  cthreads.h
//    
//  Author - Ethan Mayer
//  Fall 2022  

#ifndef CTHREADS_H
#define CTHREADS_H

// Error handling
void error(char* msg);

// Function to be called from Cython
int* start_test(int arr[], int arrSize);

#endif