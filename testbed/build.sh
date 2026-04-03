#!/bin/bash
set -e
set -x

OS="$(uname)"

mkdir -p ../bin

cFilenames=$(find . -type f -name "*.c")

assembly="testbed"
compiler="clang"
compilerFlags="-g -fPIC"
defines="-D_DEBUG -DKIMPORT"

if [ "$OS" = "Darwin" ]; then
    echo "Building testbed for macOS"

    includeFlags="-Isrc -I../engine/src/"
    linkerFlags="-L../bin -lengine -Wl,-rpath,@executable_path"
    output="../bin/$assembly"

elif [ "$OS" = "Linux" ]; then
    echo "Building testbed for Linux"

    includeFlags="-Isrc -I../engine/src/"
    linkerFlags="-L../bin -lengine -Wl,-rpath,."
    output="../bin/$assembly"

else
    echo "Unsupported OS: $OS"
    exit 1
fi

echo "Building $assembly..."
$compiler $cFilenames $compilerFlags $defines $includeFlags $linkerFlags -o $output || exit 1

if [ ! -f "../bin/$assembly" ]; then
    echo "Build failed: output not found"
    exit 1
fi
