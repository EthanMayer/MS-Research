import cython
import Person

def get_person(Person p):
    age_increase(p.age)

cpdef int age_increase(int age):
    age = age * 2
    return age