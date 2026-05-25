#!/bin/bash
set -e

# Default values
BUILD_TYPE="Debug"

usage() {
    echo "Usage: $0 [options] [-- [ctest-args]]"
    echo "Options:"
    echo "  -t, --type <type>    Build type (Debug, Release, RelWithDebInfo, MinSizeRel). Default: Debug"
    echo "  -h, --help           Show this help message"
}

# Parse options
while [[ $# -gt 0 ]]; do
    case "$1" in
        -t|--type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        --)
            shift
            break
            ;;
        *)
            # Stop parsing and pass rest to ctest
            break
            ;;
    esac
done

BUILD_DIR="build/$(echo $BUILD_TYPE | tr '[:upper:]' '[:lower:]')"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory $BUILD_DIR not found. Building first..."
    ./scripts/build.sh -t "$BUILD_TYPE"
fi

cd "$BUILD_DIR"
echo "Running tests in $BUILD_DIR..."
ctest --output-on-failure "$@"
