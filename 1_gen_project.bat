@echo off
if not exist build_windows mkdir build_windows
pushd build_windows
cmake -G "Visual Studio 16 2019" ..
popd
