#!/bin/bash

# Default settings
BUILD_TYPE="Debug"
EXE_NAME="terme_examples"

# ====================================================== parse arguments
while getopts "rd" opt; do
  case $opt in
    r) BUILD_TYPE="Release" ;;
    d) BUILD_TYPE="Debug" ;;
    *) echo "Usage: $0 [-r (Release)] [-d (Debug)]"; exit 1 ;;
  esac
done

echo "Targeting $BUILD_TYPE version..."

# ====================================================== target check
if [ -f "build/$BUILD_TYPE/$EXE_NAME" ]; then
    TARGET_PATH="build/$BUILD_TYPE/$EXE_NAME"
elif [ -f "build/$EXE_NAME" ]; then
    TARGET_PATH="build/$EXE_NAME"
else
    echo "-------------------------------------------------------"
    echo "ERROR: Target '$EXE_NAME' ($BUILD_TYPE) not found."
    echo "Please run your build script first: ./xbuild.sh -$([[ "$BUILD_TYPE" == "Release" ]] && echo "r" || echo "d")"
    echo "-------------------------------------------------------"
    exit 1
fi

# ====================================================== execution
echo "--- Running: $TARGET_PATH ---"
./"$TARGET_PATH"