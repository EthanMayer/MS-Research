from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import numpy
# first arg in Extension is the name of the produced module
# second arg, the list, is the cython file and the c file
# third is the numpy dir as it will be used inside the cython code
ext_modules = [Extension("weather_threads", ["pyxthreads.pyx", "cthreads.c"],
                         include_dirs = [numpy.get_include()])]
setup(
  cmdclass = {"build_ext": build_ext},
  ext_modules = ext_modules
)