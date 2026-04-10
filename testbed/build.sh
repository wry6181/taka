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

    mkdir -p "../bin/testbed.app/Contents/MacOS"
    mkdir -p "../bin/testbed.app/Contents/Resources"
    mkdir -p "../bin/testbed.app/Contents/Frameworks"

    cat > "../bin/testbed.app/Contents/Info.plist" << 'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>testbed</string>
    <key>CFBundleIdentifier</key>
    <string>com.taka.testbed</string>
    <key>CFBundleName</key>
    <string>testbed</string>
    <key>CFBundleVersion</key>
    <string>1.0</string>
    <key>LSMinimumSystemVersion</key>
    <string>10.13</string>
    <key>NSPrincipalClass</key>
    <string>NSApplication</string>
</dict>
</plist>
EOF

    includeFlags="-Isrc -I../engine/src/"
    defines="-D_DEBUG -DKIMPORT -DT_PLATFORM_MACOS=1"
    linkerFlags="-L../bin -lengine -Wl,-rpath,@executable_path/../Frameworks -framework AppKit -framework Foundation"
    output="../bin/testbed.app/Contents/MacOS/testbed"
    cFilenames=$(find . -type f -name "*.c")

    if [ -f "../bin/libengine.dylib" ]; then
        cp "../bin/libengine.dylib" "../bin/testbed.app/Contents/Frameworks/libengine.dylib"
        chmod +w "../bin/testbed.app/Contents/Frameworks/libengine.dylib"
        install_name_tool -id "@rpath/libengine.dylib" "../bin/testbed.app/Contents/Frameworks/libengine.dylib" 2>/dev/null || true
        install_name_tool -change "@rpath/libengine.dylib" "@executable_path/../Frameworks/libengine.dylib" "../bin/testbed.app/Contents/MacOS/testbed" 2>/dev/null || true
    fi

elif [ "$OS" = "Linux" ]; then
    echo "Building testbed for Linux"

    includeFlags="-Isrc -I../engine/src/"
    defines="-D_DEBUG -DKIMPORT -DT_PLATFORM_LINUX=1"
    linkerFlags="-L../bin -lengine -Wl,-rpath,\$ORIGIN"
    output="../bin/$assembly"
    cFilenames=$(find . -type f -name "*.c")

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
