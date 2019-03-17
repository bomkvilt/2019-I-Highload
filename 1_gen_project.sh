if [ ! -d "build" ]; then
    mkdir "build"
fi
pushd build
cmake ..
popd