@echo off
pushd %~dp0\..\
mkdir build
cd build
call cmake .. -G "Visual Studio 17 2022" -A x64
popd
PAUSE