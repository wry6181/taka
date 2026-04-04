#!/bin/bash
# Build script for rebuilding everything

echo "Building everything..."

pushd engine > /dev/null
source build.sh
ERRORLEVEL=$?
popd > /dev/null
if [ $ERRORLEVEL -ne 0 ]
then
    echo "Error: $ERRORLEVEL" && exit
fi

pushd testbed > /dev/null
source build.sh
ERRORLEVEL=$?
popd > /dev/null
if [ $ERRORLEVEL -ne 0 ]
then
    echo "Error: $ERRORLEVEL" && exit
fi

# Merge compile_commands.json files
echo "Merging compile_commands.json..."
jq -s 'add' engine/compile_commands.json testbed/compile_commands.json > compile_commands.json

echo "All assemblies built successfully."
