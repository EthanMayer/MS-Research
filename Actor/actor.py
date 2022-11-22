import sys
# import everything from the module
from comp import *

print("======== Actor.py ========")
n = len(sys.argv)
tup = []
for i in range(1,n):
    tup.append(int(sys.argv[i]))
tup = tuple(tup)
# arr = (int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3]))
print("Tuple: " + str(tup))
tup2 = main(tup)
print("======== Actor.py ========")
print("Returned tuple: " + str(tup2))