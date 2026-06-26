#!/bin/bash
# Build osgQt6 (Qt6 port of osgQt-3.5.7) against local OSG, install into external/osgQt6/
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VECDS_DIR="$(dirname "$SCRIPT_DIR")"
# Pristine osgQt-3.5.7 sources vendored in the repo, so the build does not
# depend on a system package manager (portage) or a pre-populated /tmp.
OSGQT_TARBALL="${SCRIPT_DIR}/osgqt_src/osgQt-3.5.7.tar.gz"
# Use a per-user work dir under /tmp so a stale extraction owned by another
# user (left over from an earlier build) doesn't block us with EACCES.
OSGQT_WORK="/tmp/osgQt-3.5.7-${USER}"
OSGQT_SRC="${OSGQT_WORK}/osgQt-3.5.7"
OSGQT_BUILD="${OSGQT_SRC}/build_qt6"
OSGQT_INSTALL="${VECDS_DIR}/external/osgQt6"
OSG_INSTALL="${VECDS_DIR}/external/osg"

if [ ! -d "$OSGQT_SRC" ]; then
    echo "Extracting osgQt-3.5.7..."
    mkdir -p "$OSGQT_WORK"
    tar xzf "$OSGQT_TARBALL" -C "$OSGQT_WORK"
fi

echo "Copying Qt6-ported source files..."
cp "${SCRIPT_DIR}/osgqt_src/GraphicsWindowQt.cpp" "${OSGQT_SRC}/src/osgQt/GraphicsWindowQt.cpp"
cp "${SCRIPT_DIR}/osgqt_src/GraphicsWindowQt" "${OSGQT_SRC}/include/osgQt/GraphicsWindowQt"

# Apply the standalone Qt6 CMakeLists
cp "${SCRIPT_DIR}/osgqt_CMakeLists.txt" "${OSGQT_SRC}/CMakeLists.txt"

echo "Configuring osgQt6..."
mkdir -p "$OSGQT_BUILD"
cmake -S "$OSGQT_SRC" -B "$OSGQT_BUILD" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$OSGQT_INSTALL" \
    -DCMAKE_PREFIX_PATH="$OSG_INSTALL"

echo "Building osgQt6..."
cmake --build "$OSGQT_BUILD" -j"$(nproc)"

echo "Installing osgQt6 to ${OSGQT_INSTALL}..."
cmake --install "$OSGQT_BUILD"

echo "Done. osgQt6 installed to: $OSGQT_INSTALL"
