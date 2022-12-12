#   setup.py
#   
#   Author: Ethan Mayer
#   Fall 2022
#   
#   This file handles building the Cython components of the project.
#   Any includes and libraries needed for the Cython code to be compiled should be included here.

# Necessary includes for building Cython code
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

# First arg in Extension is the name of the produced module
# Second arg, the list, is the cython file and the c file
# Third is the numpy dir as it will be used inside the cython code
ext_modules = [Extension("comp", ["comp.pyx"],
                         include_dirs = ["/opt/homebrew/Cellar/zeromq/4.3.4/include", "/opt/homebrew/lib/python3.9/site-packages/zmq/utils"],
                         library_dirs = ["/opt/homebrew/Cellar/zeromq/4.3.4/lib"],
                         libraries = ["zmq"])]

# Build Cython code
setup(
  cmdclass = {"build_ext": build_ext},
  ext_modules = ext_modules
)