# import cython

# @cython.cclass
class Person:
    # @cython.ccall
    def __init__(self, name, age):
        self.name = name
        self.age = age

    # @cython.ccall
    def myfunc(self):
        print("Hello my name is " + self.name)

# p1 = Person("John", 36)
# p1.myfunc()