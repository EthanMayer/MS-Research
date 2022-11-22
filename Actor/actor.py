import sys
# import everything from the module
from comp import *

print("======== Starting Actor.py ========")
n = len(sys.argv)
arr = []
for i in range(1,n):
    arr.append(int(sys.argv[i]))
arr = tuple(arr)
# arr = (int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3]))
print("Tuple: " + str(arr))
main(arr)