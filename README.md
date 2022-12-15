
# Master's Research

This research's goal is to convert RIAPS (Resilient Information Architecture Platform for Smart Grid) to a hard-realtime system. Since it is written mostly in Python, Cython is being used to explore the possibility to replace the psuedo-multithreading allowed by the Python GIL with POSIX pthreads.


## Contents

* [Authors](#authors)
* [Background](#background)
* [Setup](#setup)
* [Usage](#usage)
## Authors

- [@EthanMayer](https://github.com/EthanMayer)


## Background

RIAPS is a software platform for building distributed real-time embedded applications using a component-oriented approach. The application area is power systems - the ‘Smart Grid’ where RIAPS can serve as the foundation for implementing the distributed intelligence. RIAPS was created by the Vanderbilt Institute of Software Integrated Systems (ISIS) with funding from the Advanced Research Projects Agency-Energy (ARPA-E), U.S. Department of Energy.

RIAPS is written mostly in Python, so the threading done for the developer's embedded application is done using Python's `threading` library. Due to the Global Interpreter Lock (GIL), this only amounts to a pseudo-multithreaded environment. In order for RIAPS to be a hard-realtime embedded system, true multithreading is required. 

To learn more about RIAPS, visit:

* https://riaps.github.io
* https://riaps.isis.vanderbilt.edu

## Setup

This project, by default, depends on the Homebrew installation of Python 3.9 and ZMQ on MacOS. In order to install Python 3.9 using Homebrew, run:

`brew install python@3.9`

In order to install ZMQ using Homebrew, run:

`brew install zeromq`

The included `build.sh` Bash script is used to automate the building and compiling necessary to run the project. To use it, run:

`bash build.sh`

If this completes successfully, the project is ready to be run (see [Usage](#usage)). If there are errors, see below.

**Additional Information**

Cython must be built before it can be used. The `setup.py` Python file is used to compile the Cython files, but may require some changes depending on the build environment. Its usage can be found in the `build.sh` script.

The `include_dirs`, `library_dirs`, and `libraries` arguments are using the default location for Homebrew's installation of Python 3.9 and ZMQ. Change these if you wish to use a different installation or are not using MacOS.

This project uses shared library files (.so) generated from C code to load and use functions from. The compile and bundle commands can be found in the `build.sh` script.

To compile, these .c files use the default location of the MacOS11.sdk. and link the default location for Homebrew's installation of ZMQ. Change these if you wish to use a different installation or are not using MacOS.
## Usage

The program takes arbitrarily-many integers as command line arguments. To start the program, run:

`python3 actor.py x1, x2, ... ,xn`

Where x1, x2, ... ,xn are the input integers that the program will be operating on. For example:

`python3 actor.py 1, 2, 3, 4, 5`

You will then be prompted for which C function you would like to operate on the provided integers. Currently, there are two C functions available for the project: `send_back` and `add`, which just return the input values and sum the input values, respectively.

Since this project is experimental, there are many debug print messages. The identifier currently printing the message (Python, Cython, or Thread X) represent the level (high, mid, and low, respectively) that is currently reporting.


