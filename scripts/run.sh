#!/bin/bash
set -e

# Default values
BUILD_TYPE="Debug"
TARGET="http_server"

usage() {
    echo "Usage: $0 [options] [target] [-- [args]]"
    echo "Options:"
    echo "  -t, --type <type>    Build type (Debug, Release, RelWithDebInfo, MinSizeRel). Default: Debug"
    echo "  -h, --help           Show this help message"
    echo ""
    echo "Targets:"
    echo "  http_server (default), chizik_tests"
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
            TARGET="$1"
            shift
            ;;
    esac
done

BUILD_DIR="build/$(echo $BUILD_TYPE | tr '[:upper:]' '[:lower:]')"
EXECUTABLE="$BUILD_DIR/$TARGET"

if [[ "$TARGET" == "chizik_tests" ]]; then
    EXECUTABLE="$BUILD_DIR/chizik/tests/chizik_tests"
fi

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable $EXECUTABLE not found. Building first..."
    ./scripts/build.sh -t "$BUILD_TYPE" "$TARGET"
fi

echo "Running $EXECUTABLE..."
"$EXECUTABLE" "$@"
