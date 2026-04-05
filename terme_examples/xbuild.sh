#!/bin/bash

BUILD_TYPE="Debug"

# ====================================================== parse arguments
while getopts "rd" opt; do
  case $opt in
    r) BUILD_TYPE="Release" ;;
    d) BUILD_TYPE="Debug" ;;
    *) echo "Usage: $0 [-r (Release)] [-d (Debug)]"; exit 1 ;;
  esac
done

echo "--- Starting Project Build Process ($BUILD_TYPE) ---"

# ====================================================== cleanup & directory setup
rm -rf "build/$BUILD_TYPE"
mkdir -p "build/$BUILD_TYPE"
cd "build/$BUILD_TYPE" || exit

# ====================================================== helper function
check_and_create_dep() {
    local NAME=$1
    local VERSION=$2
    local REPO=$3
    local SUBDIR=$4
    local FULL_REF="$NAME/$VERSION"

    if ! conan list "$FULL_REF#*" 2>/dev/null | grep -q "revisions"; then
        echo "[INFO] $NAME NOT found. Downloading from GitHub..."
        TMP_DIR=$(mktemp -d)
        git clone --depth 1 "$REPO" "$TMP_DIR/$NAME"
        
        local CREATE_PATH="$TMP_DIR/$NAME"
        [ -n "$SUBDIR" ] && CREATE_PATH="$CREATE_PATH/$SUBDIR"

        conan create "$CREATE_PATH" --name="$NAME" --version="$VERSION" \
            -s build_type="$BUILD_TYPE" -s compiler.cppstd=20 --build=missing
        rm -rf "$TMP_DIR"
    else
        echo "[INFO] Found $NAME/$VERSION in local cache."
    fi
}

# ====================================================== download dependencies
check_and_create_dep "nbkit" "1.0.0" "https://github.com/nico-bertoli/nbkit.git"
check_and_create_dep "terme" "1.0.0" "https://github.com/nico-bertoli/terme.git" "terme"

# ====================================================== conan install
echo "[INFO] Installing project dependencies..."
conan install ../.. --build=missing -s build_type="$BUILD_TYPE" \
    -s compiler.cppstd=20 -c tools.system.package_manager:mode=skip || exit 1

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