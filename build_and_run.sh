#!/bin/bash

# Define project directories
SOURCE_DIR="./src"
BUILD_DIR="./build/mac"
RAYLIB_DIR="./raylib/src"
OUTPUT_FILE="raycasting"

# Ensure the build directory exists
mkdir -p $BUILD_DIR

# Compile the project
clang -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL $RAYLIB_DIR/libraylib.a -I$RAYLIB_DIR $SOURCE_DIR/main.c -o $BUILD_DIR/$OUTPUT_FILE

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Build succeeded. Running the project..."
    # Open the application
    open $BUILD_DIR/$OUTPUT_FILE
    # close the terminal
    exit 0
else
    echo "Build failed."
fi