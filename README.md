# HilbertRTree
C++ Hilbert R-Tree implementation

Dependencies:
1. Hilbert Compact Index: https://web.cs.dal.ca/~chamilto/hilbert/ . The project uses v0.2-1
2. Boost library version >= 1.48 http://www.boost.org/doc/libs/1_48_0/
3. CMake http://www.cmake.org/

Build instructions:
1. Consider $ROOT_DIR the path to the root source directory.
2. Create a folder where you want the build files to be placed $BUILD_DIR
3. $> cd $BUILD_DIR
   $> cmake $ROOT_DIR
   $> make
   $> ./test/main_test #To run all the tests. 