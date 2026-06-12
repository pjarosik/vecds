#!/bin/bash
# Build OpenSceneGraph 3.6.5 from source to a local user prefix (no root required).
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VECDS_DIR="$(dirname "$SCRIPT_DIR")"
OSG_TARBALL="/usr/portage/distfiles/OpenSceneGraph-3.6.5.tar.gz"
OSG_SRC="/tmp/OpenSceneGraph-OpenSceneGraph-3.6.5"
OSG_BUILD="${OSG_SRC}/build"
OSG_INSTALL="${VECDS_DIR}/external/osg"

if [ ! -d "$OSG_SRC" ]; then
    echo "Extracting OpenSceneGraph-3.6.5..."
    tar xzf "$OSG_TARBALL" -C /tmp/
fi

echo "Configuring OpenSceneGraph..."
mkdir -p "$OSG_BUILD"
cmake -S "$OSG_SRC" -B "$OSG_BUILD" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$OSG_INSTALL" \
    -DBUILD_OSG_EXAMPLES=OFF \
    -DBUILD_OSG_APPLICATIONS=OFF \
    -DOSG_USE_QT=OFF \
    -DCMAKE_CXX_FLAGS="-Wno-deprecated-declarations" \
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
    -DBUILD_OSG_PLUGIN_FFMPEG=OFF

echo "Building OpenSceneGraph (this takes a few minutes)..."
cmake --build "$OSG_BUILD" -j"$(nproc)"

echo "Installing OpenSceneGraph to ${OSG_INSTALL}..."
cmake --install "$OSG_BUILD"

echo "Done. OSG installed to: $OSG_INSTALL"
