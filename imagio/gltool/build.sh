#!/bin/bash

INCLUDE_PATH="-isystem/usr/local/include -isystem../libs/gl3w -isystem/usr/include/eigen3"
LIBRARIES="-lglut -lGL -lGLEW -lGLU -lglfw3 -lX11 -ldl \
                        -lXxf86vm -lXrandr -lpthread -lXi -lm -lrt \
                        -lXcursor -lXinerama -lboost_filesystem -lboost_system"


g++ ../libs/gl3w/GL/gl3w.c gltool.cpp -std=c++11 $INCLUDE_PATH $LIBRARIES -o gltool

