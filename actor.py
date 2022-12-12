#   actor.py
#    
#   Author - Ethan Mayer
#   Fall 2022  
#
#   This the top-level file of the project, somewhat emulating actor.py in RIAPS. 
#   This file takes in user input and interfaces with the cython comp.pyx file via zmq PAIR sockets.

# Includes
import sys
import zmq
import threading
# Import everything from the project's built cython module comp
from comp import *

print("======== Actor.py ========")

# Generate tuple from list of command line arguments
n = len(sys.argv)
tup = []
for i in range(1,n):
    tup.append(int(sys.argv[i]))
tup = tuple(tup)

print("Tuple: " + str(tup))

# Create pair socket to communicate with comp.pyx
context = zmq.Context()
socket = context.socket(zmq.PAIR)
socket.bind("tcp://127.0.0.1:5555")

# Hand tuple to cython main function via a python thread
tup2 = []
comp = threading.Thread(target = main, args = (tup, tup2), daemon = True)
comp.start()

# Wait for ready from Cython
msg = socket.recv()
print("Python: received " + str(msg))

# Send function to be loaded
func = input("Enter function to be loaded: ")
socket.send_string(func)

# Wait for Cython to finish for result
comp.join()

# Clean up ZMQ
socket.close()
context.destroy()

# Print the tuple returned by cython
print("======== Actor.py ========")
print("Returned tuple: " + str(tuple(tup2)))