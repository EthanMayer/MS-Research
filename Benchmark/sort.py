import testSort
import random
from datetime import datetime
from copy import deepcopy

arr = []
for i in range(20000):
    arr.append(random.randint(0, 1000))
arr2 = deepcopy(arr)
arr3 = deepcopy(arr)
arr4 = deepcopy(arr)

start = datetime.now()
testSort.bubbleSort1(arr)
now = datetime.now()
delt1 = now - start
print("cpdef time: " + str(delt1))

start = datetime.now()
testSort.bubbleSort2(arr2)
now = datetime.now()
delt2 = now - start
print("def time: " + str(delt2))

print("Speedup: " + str(delt2/delt1))

start = datetime.now()
testSort.bubbleSort3(arr3)
now = datetime.now()
delt3 = now - start
print("cpdef -> c time: " + str(delt3))

print("Speedup: " + str(delt2/delt3))

start = datetime.now()
testSort.bubbleSort4(arr4)
now = datetime.now()
delt4 = now - start
print("def -> c time: " + str(delt4))

print("Speedup: " + str(delt2/delt4))