#@echo off
IF not EXIST %~dp0\..\build (
    echo "not has dir"
    mkdir %~dp0\..\build
)
pushd %~dp0\..\build
echo %cd%
cl -Zi ..\code\lesson001.cpp user32.lib
popd
