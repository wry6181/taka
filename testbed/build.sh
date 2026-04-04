#!/bin/bash
set -e

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
    defines="-D_DEBUG -DKIMPORT -DT_PLATFORM_MACOS=1"
    linkerFlags="-L../bin -lengine -Wl,-rpath,@executable_path"
    output="../bin/$assembly"

elif [ "$OS" = "Linux" ]; then
    echo "Building testbed for Linux"

    includeFlags="-Isrc -I../engine/src/"
    defines="-D_DEBUG -DKIMPORT -DT_PLATFORM_LINUX=1"
    linkerFlags="-L../bin -lengine -Wl,-rpath,\$ORIGIN"
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

echo "Generating compile_commands.json..."
json="["
first=true
for f in $cFilenames; do
    if [ "$first" = true ]; then
        first=false
    else
        json="$json,"
    fi
    absdir="$(pwd)"
    f_clean="${f#./}"
    json="$json{\"directory\": \"$absdir\", \"file\": \"$absdir/$f_clean\", \"command\": \"clang $f_clean $compilerFlags $defines $includeFlags\"}"
done
json="$json]"
echo "$json" > compile_commands.json
