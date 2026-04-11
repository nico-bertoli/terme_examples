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
      echo "  --local   Build nbkit and terme from ../../nbkit and ../../terme/terme (relative to this script)"
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

# ====================================================== helper functions
create_dep_from_path() {
    local NAME=$1
    local VERSION=$2
    local CREATE_PATH=$3

    echo "[INFO] Creating Conan package $NAME/$VERSION from: $CREATE_PATH"
    conan create "$CREATE_PATH" --name="$NAME" --version="$VERSION" \
        -s build_type="$BUILD_TYPE" -s compiler.cppstd=20 --build=missing || exit 1
}

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

        create_dep_from_path "$NAME" "$VERSION" "$CREATE_PATH"
        rm -rf "$TMP_DIR"
    else
        echo "[INFO] Found $NAME/$VERSION in local cache."
    fi
}

create_deps_local() {
    local NBKIT_SRC="$SCRIPT_DIR/../../nbkit"
    local TERME_SRC="$SCRIPT_DIR/../../terme/terme"

    if [[ ! -d "$NBKIT_SRC" ]]; then
        echo "ERROR: nbkit not found at $NBKIT_SRC (expected sibling of terme_examples)"
        exit 1
    fi
    if [[ ! -d "$TERME_SRC" ]]; then
        echo "ERROR: terme package dir not found at $TERME_SRC (expected .../terme/terme with conanfile.py)"
        exit 1
    fi

    NBKIT_SRC="$(cd "$NBKIT_SRC" && pwd)"
    TERME_SRC="$(cd "$TERME_SRC" && pwd)"

    echo "[INFO] --local: building nbkit from $NBKIT_SRC"
    echo "[INFO] --local: building terme from $TERME_SRC"

    create_dep_from_path "nbkit" "1.0.0" "$NBKIT_SRC"
    create_dep_from_path "terme" "1.0.0" "$TERME_SRC"
}

# ====================================================== download / local dependencies

# used for development, builds local terme and nbkit
if [[ "$USE_LOCAL" -eq 1 ]]; then
    create_deps_local
else
    check_and_create_dep "nbkit" "1.0.0" "https://github.com/nico-bertoli/nbkit.git"
    check_and_create_dep "terme" "1.0.0" "https://github.com/nico-bertoli/terme.git" "terme"
fi

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