#!/bin/bash
set -e

VECDS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Parse arguments
VECDS_ONLY=0
for arg in "$@"; do
    case "$arg" in
        -v|--vecds-only)
            VECDS_ONLY=1
            ;;
        -h|--help)
            echo "Usage: $0 [--vecds-only|-v]"
            echo ""
            echo "  --vecds-only, -v   Build only vecds incrementally, skipping OSG,"
            echo "                     osgQt6 and FEMApp. Reuses the existing build/"
            echo "                     directory (no clean rebuild)."
            exit 0
            ;;
        *)
            echo "Unknown argument: $arg (use --help)" >&2
            exit 1
            ;;
    esac
done

if [ "$VECDS_ONLY" -eq 1 ]; then
    echo "=== BUILDING vecds only (incremental) ==="
    cd "$VECDS_DIR"
    OSG_INSTALL="${VECDS_DIR}/external/osg"
    # Configure only if the build dir hasn't been set up yet.
    if [ ! -f build/CMakeCache.txt ]; then
        cmake -S . -B build \
            -DCMAKE_PREFIX_PATH="${OSG_INSTALL}"
    fi
    cmake --build build --config Release -j"$(nproc)"
    cmake --install build --prefix bin
    mv build/vecds bin/vecds
    echo "=== BUILD COMPLETE ==="
    exit 0
fi

# Step 1: build OSG locally if not already installed
if [ ! -f "${VECDS_DIR}/external/osg/lib/libosg.so" ]; then
    echo "=== BUILDING OpenSceneGraph 3.6.5 (local install) ==="
    bash "${VECDS_DIR}/scripts/build_osg.sh"
fi

# Step 2: build osgQt6 if not already installed
if [ ! -f "${VECDS_DIR}/external/osgQt6/lib/libosgQt6.so" ]; then
    echo "=== BUILDING osgQt6 ==="
    bash "${VECDS_DIR}/scripts/build_osgqt.sh"
fi

echo "=== BUILDING FEMApp ==="
cd "$VECDS_DIR"
./cleanallreleaseFEMApp
./makeallreleaseFEMApp

echo "=== BUILDING vecds ==="
OSG_INSTALL="${VECDS_DIR}/external/osg"
rm -rf build
mkdir -p build
cmake -S . -B build \
    -DCMAKE_PREFIX_PATH="${OSG_INSTALL}"
cmake --build build --config Release -j"$(nproc)"
cmake --install build --prefix bin
mv build/vecds bin/vecds

echo "=== BUILD COMPLETE ==="
