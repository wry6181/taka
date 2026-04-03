#!/bin/bash
# Build script for rebuilding everything
set echo on

echo "Building everything..."


pushd engine
source build.sh
popd

ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
echo "Error:"$ERRORLEVEL && exit
fi

pushd testbed
source build.sh
popd
ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
echo "Error:"$ERRORLEVEL && exit
fi

# Merge compile_commands.json files
echo "Merging compile_commands.json..."
jq -s '.[0] + .[1]' engine/compile_commands.json testbed/compile_commands.json > compile_commands.json

echo "All assemblies built successfully."
