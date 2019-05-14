#@echo off
IF not EXIST %~dp0\..\build (
    echo "not has dir"
    mkdir %~dp0\..\build
)
pushd %~dp0\..\build
echo %cd%
cl -Zi ..\code\lesson002.cpp user32.lib gdi32.lib
popd
