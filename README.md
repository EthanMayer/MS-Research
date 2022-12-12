Cythonize cython file + compile related .c files:
python3 setup.py build_ext --inplace

Compile funcBody.c into .so:

Comiple the .c file:
clang -Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX11.sdk -I/opt/homebrew/Cellar/zeromq/4.3.4/include -c funcBody.c -o build/temp.macosx-11-arm64-3.9/funcBody.o

Bundle resulting .o file into .so:
clang -bundle -undefined dynamic_lookup -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX11.sdk build/temp.macosx-11-arm64-3.9/funcBody.o -L/opt/homebrew/Cellar/zeromq/4.3.4/lib -lzmq -o "/Users/EthanMayer_1/Library/CloudStorage/OneDrive-Personal/Documents/MS Thesis Research/Cython Tests/Actor/funcBody.so"