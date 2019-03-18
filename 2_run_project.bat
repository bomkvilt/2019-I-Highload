@echo off
pushd build_windows
for %%f in (*.sln) do (
    start %%f
)
popd
