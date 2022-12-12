from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
# first arg in Extension is the name of the produced module
# second arg, the list, is the cython file and the c file
# third is the numpy dir as it will be used inside the cython code
ext_modules = [Extension("comp", ["comp.pyx"],
                         include_dirs = ["/opt/homebrew/Cellar/zeromq/4.3.4/include", "/opt/homebrew/lib/python3.9/site-packages/zmq/utils"],
                         library_dirs = ["/opt/homebrew/Cellar/zeromq/4.3.4/lib"],
                         libraries = ["zmq"])]
setup(
  cmdclass = {"build_ext": build_ext},
  ext_modules = ext_modules
)