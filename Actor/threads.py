import sys
# import everything from the module
from zmq_threads import *

print("======== Starting thread test ========")
arr = (int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3]))
print("Tuple: " + str(arr))
start_thread_test(arr)