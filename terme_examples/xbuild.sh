#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

BUILD_TYPE="Debug"
USE_LOCAL=0

# ====================================================== parse arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    -r) BUILD_TYPE="Release" ;;
    -d) BUILD_TYPE="Debug" ;;
    --local) USE_LOCAL=1 ;;
    -h|--help)
      echo "Usage: $0 [-r (Release)] [-d (Debug)] [--local]"
      echo "  --local   Use ../../nbkit and ../../terme/terme instead of GitHub recipes"
      exit 0
      ;;
    *)
      echo "Usage: $0 [-r (Release)] [-d (Debug)] [--local]"
      exit 1
      ;;
  esac
  shift
done

echo "--- Starting Project Build Process ($BUILD_TYPE) ---"

# ====================================================== cleanup & directory setup
rm -rf "build/$BUILD_TYPE"
mkdir -p "build/$BUILD_TYPE"
cd "build/$BUILD_TYPE" || exit

CONAN_EXTRA=()
if [[ "$USE_LOCAL" -eq 1 ]]; then
    CONAN_EXTRA+=(-c "user.terme_examples:local=True")
fi

# ====================================================== conan install
echo "[INFO] Installing project dependencies..."
conan install ../.. --build=missing -s build_type="$BUILD_TYPE" \
    -s compiler.cppstd=20 -c tools.system.package_manager:mode=skip \
    "${CONAN_EXTRA[@]}" || exit 1

# ====================================================== find toolchain
CMAKE_TOOLCHAIN=$(find . -name conan_toolchain.cmake | head -n 1)

if [ -z "$CMAKE_TOOLCHAIN" ]; then
    echo "ERROR: Could not find conan_toolchain.cmake"
    exit 1
fi

# ====================================================== build
echo "[INFO] Configuring CMake..."

cmake ../.. \
    -DCMAKE_TOOLCHAIN_FILE="$CMAKE_TOOLCHAIN" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_EXE_LINKER_FLAGS="-lX11 -lpthread -ldl" \
    -DCMAKE_SHARED_LINKER_FLAGS="-lX11"

echo "[INFO] Building Project ($BUILD_TYPE)..."
cmake --build . --config "$BUILD_TYPE" -- -j$(nproc) || { echo "BUILD FAILED"; exit 1; }

echo -e "\n[SUCCESS] Build completed ($BUILD_TYPE)."
