#!/bin/bash
# Build osgQt6 (Qt6 port of osgQt-3.5.7) against local OSG, install into external/osgQt6/
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VECDS_DIR="$(dirname "$SCRIPT_DIR")"
OSGQT_TARBALL="/usr/portage/distfiles/osgQt-3.5.7.tar.gz"
OSGQT_SRC="/tmp/osgQt-3.5.7"
OSGQT_BUILD="${OSGQT_SRC}/build_qt6"
OSGQT_INSTALL="${VECDS_DIR}/external/osgQt6"
OSG_INSTALL="${VECDS_DIR}/external/osg"

if [ ! -d "$OSGQT_SRC" ]; then
    echo "Extracting osgQt-3.5.7..."
    tar xzf "$OSGQT_TARBALL" -C /tmp/
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
