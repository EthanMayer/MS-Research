# import testDS
import Person

def getAge(p):
    return p.age

p1 = Person.Person("John", 36)
print(getAge(p1))