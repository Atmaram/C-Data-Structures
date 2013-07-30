#!/bin/bash

echo "Cleaning stale builds..."
rm -rf build/*
echo "Entering private directory..."
cd private
echo "Compiling hashmap.c..."
gcc -c -g hashmap.c -o ../build/hashmap.o
echo "Leaving private directory..."
cd -
echo "Entering build directory..."
cd build
echo "Building libhashmap.a..."
ar rcs libhashmap.a hashmap.o
echo "Compiling and linking test file with static libraries..."
gcc -g ../test.c libhashmap.a -o test.out
echo "Running test..."
chmod +x test.out
./test.out
echo "Leaving build directory..."
cd ..
