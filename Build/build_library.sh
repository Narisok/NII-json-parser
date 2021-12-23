#!/bin/bash

mkdir -p Build/Release/Sources

cd Build/Release/Sources

g++ -std=c++2a -c -I ../../../includes ../../../sources/NII/json-files/*.cpp

cd ..
ar rcs libjson.a Sources/*.o