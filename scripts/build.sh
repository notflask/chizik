#!/bin/bash
set -e

# Default values
BUILD_TYPE="Debug"
TARGET="all"
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)
CLEAN=false

usage() {
    echo "Usage: $0 [options] [target]"
    echo "Options:"
    echo "  -t, --type <type>    Build type (Debug, Release, RelWithDebInfo, MinSizeRel). Default: Debug"
    echo "  -j, --jobs <n>       Number of parallel jobs. Default: $JOBS"
    echo "  -c, --clean          Clean build directory before building"
    echo "  -h, --help           Show this help message"
    echo ""
    echo "Targets:"
    echo "  all (default), http_server, chizik, chizik_tests"
}

# Parse options
while [[ $# -gt 0 ]]; do
    case "$1" in
        -t|--type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            TARGET="$1"
            shift
            ;;
    esac
done

BUILD_DIR="build/$(echo $BUILD_TYPE | tr '[:upper:]' '[:lower:]')"

if [ "$CLEAN" = true ]; then
    echo "Cleaning $BUILD_DIR..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Configuring for $BUILD_TYPE..."
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ../..

echo "Building target: $TARGET with $JOBS jobs..."
cmake --build . --target "$TARGET" -j "$JOBS"
