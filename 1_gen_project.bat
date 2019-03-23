@echo off
set dir=./build/windows

if not exist "%dir%" mkdir "%dir%"

pushd "%dir%"
cmake -G "Visual Studio 16 2019" ./../..
popd
