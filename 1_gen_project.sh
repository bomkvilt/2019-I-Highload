if [ ! -d "build_linux" ]; then
    mkdir "build_linux"
fi
pushd build_linux
cmake ..
popd
