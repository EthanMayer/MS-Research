import sys
# import everything from the module
from comp import *

print("======== Starting Actor.py ========")
arr = (int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3]))
print("Tuple: " + str(arr))
main(arr)