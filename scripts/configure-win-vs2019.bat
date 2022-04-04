@echo off
pushd %~dp0\..\
mkdir build
cd build
call cmake .. -G "Visual Studio 16 2019" -A x64
popd
PAUSE