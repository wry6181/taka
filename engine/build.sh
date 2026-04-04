#!/bin/bash
set -e

OS="$(uname)"
VULKAN_SDK="${VULKAN_SDK:-$HOME/VulkanSDK/1.4.341.1}"

mkdir -p ../bin

cFilenames=$(find . -type f -name "*.c")

assembly="engine"
compiler="clang"
compilerFlags="-g -shared -fPIC"
defines="-D_DEBUG -DKEXPORT"

if [ "$OS" = "Darwin" ]; then
    echo "Building engine for macOS"

    includeFlags="-Isrc -I$VULKAN_SDK/macOS/include"
    linkerFlags="-L$VULKAN_SDK/macOS/lib -lvulkan -lm -ldl -lpthread -Wl,-install_name,@rpath/libengine.dylib -Wl,-rpath,$VULKAN_SDK/macOS/lib"
    output="../bin/lib$assembly.dylib"

elif [ "$OS" = "Linux" ]; then
    echo "Building engine for Linux"

    includeFlags="-Isrc -I$VULKAN_SDK/include"
    linkerFlags="-L$VULKAN_SDK/lib -lvulkan -lxcb -lX11 -lX11-xcb -lxkbcommon -lm -ldl -lpthread"
    output="../bin/lib$assembly.so"

else
    echo "Unsupported OS: $OS"
    exit 1
fi

# GLFW (both platforms)
includeFlags="$includeFlags $(pkg-config --cflags glfw3)"
linkerFlags="$linkerFlags $(pkg-config --libs glfw3)"

echo "Building $assembly..."
$compiler $cFilenames $compilerFlags $defines $includeFlags $linkerFlags -o $output || exit 1

if [ ! -f "../bin/lib$assembly.dylib" ] && [ ! -f "../bin/lib$assembly.so" ]; then
    echo "Build failed: output not found"
    exit 1
fi
