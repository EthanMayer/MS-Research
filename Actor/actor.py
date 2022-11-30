#   actor.py
#    
#   Author - Ethan Mayer
#   Fall 2022  

import sys
# Import everything from my cython module comp
from comp import *

print("======== Actor.py ========")

# Generate tuple from list of command line arguments
n = len(sys.argv)
tup = []
for i in range(1,n):
    tup.append(int(sys.argv[i]))
tup = tuple(tup)

print("Tuple: " + str(tup))

# Hand tuple to cython main function
tup2 = main(tup)

# Print the tuple returned by cython
print("======== Actor.py ========")
print("Returned tuple: " + str(tup2))