#   actor.py
#    
#   Author - Ethan Mayer
#   Fall 2022  

import sys
# Import everything from my cython module comp
from comp import *
import zmq
import threading

print("======== Actor.py ========")

# Generate tuple from list of command line arguments
n = len(sys.argv)
tup = []
for i in range(1,n):
    tup.append(int(sys.argv[i]))
tup = tuple(tup)

print("Tuple: " + str(tup))

# Create pair socket to communicate with comp.pyx
# context = zmq.Context()
# socket = context.socket(zmq.PAIR)
# socket.bind("tcp://127.0.0.1:5555")

# Hand tuple to cython main function
tup2 = []
comp = threading.Thread(target = main, args = (tup, tup2), daemon = True)
comp.start()

comp.join()
#tup2 = main(tup)

# Print the tuple returned by cython
print("======== Actor.py ========")
print("Returned tuple: " + str(tuple(tup2)))