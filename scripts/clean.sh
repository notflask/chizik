#!/bin/bash
set -e

# Default: clean everything
CLEAN_ALL=true
BUILD_TYPE=""

usage() {
    echo "Usage: $0 [options]"
    echo "Options:"
    echo "  -t, --type <type>    Clean only specific build type (Debug, Release, etc.)"
    echo "  -a, --all            Clean all build directories (default)"
    echo "  -h, --help           Show this help message"
}

# Parse options
while [[ $# -gt 0 ]]; do
    case "$1" in
        -t|--type)
            BUILD_TYPE="$2"
            CLEAN_ALL=false
            shift 2
            ;;
        -a|--all)
            CLEAN_ALL=true
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            usage
            exit 1
            ;;
    esac
done

if [ "$CLEAN_ALL" = true ]; then
    echo "Cleaning all build directories..."
    rm -rf build/
    echo "Done."
else
    BUILD_DIR="build/$(echo $BUILD_TYPE | tr '[:upper:]' '[:lower:]')"
    if [ -d "$BUILD_DIR" ]; then
        echo "Cleaning $BUILD_DIR..."
        rm -rf "$BUILD_DIR"
        echo "Done."
    else
        echo "Directory $BUILD_DIR does not exist."
    fi
fi
