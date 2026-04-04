#!/bin/bash
set -e

OS="$(uname)"
VULKAN_SDK="${VULKAN_SDK:-$HOME/VulkanSDK/1.4.341.1}"

mkdir -p ../bin

assembly="engine"
compiler="clang"
compilerFlags="-g -shared -fPIC -fobjc-arc"
defines="-D_DEBUG -DKEXPORT"

if [ "$OS" = "Darwin" ]; then
    echo "Building engine for macOS"

    includeFlags="-Isrc -I$VULKAN_SDK/macOS/include"
    defines="-D_DEBUG -DKEXPORT -DT_PLATFORM_MAC=1 -fobjc-arc"
    linkerFlags="-L$VULKAN_SDK/macOS/lib -lvulkan -lm -ldl -lpthread -Wl,-install_name,@rpath/libengine.dylib -Wl,-rpath,$VULKAN_SDK/macOS/lib -framework AppKit -framework Foundation"
    output="../bin/lib$assembly.dylib"
    cFilenames=$(find . -type f -name "*.c" | grep -v platform_linux | grep -v platform_win32)
    cFilenames="$cFilenames $(find . -type f -name "*.m")"

elif [ "$OS" = "Linux" ]; then
    echo "Building engine for Linux"

    includeFlags="-Isrc -I$VULKAN_SDK/include"
    defines="-D_DEBUG -DKEXPORT -DT_PLATFORM_LINUX=1"
    linkerFlags="-L$VULKAN_SDK/lib -lvulkan -lxcb -lX11 -lX11-xcb -lxkbcommon -lm -ldl -lpthread"
    output="../bin/lib$assembly.so"
    cFilenames=$(find . -type f -name "*.c" | grep -v platform_mac | grep -v platform_win32)

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
    cmd="clang $f_clean $compilerFlags $defines $includeFlags"
    json="$json{\"directory\": \"$absdir\", \"file\": \"$absdir/$f_clean\", \"command\": \"$cmd\"}"
done
json="$json]"
echo "$json" > compile_commands.json
