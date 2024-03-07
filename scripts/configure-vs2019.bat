@echo off
pushd %~dp0\..\
call cmake -B build -G "Visual Studio 16 2019" -A x64 -D ENABLE_3D_MODULE=ON >> scripts/configure.log
popd
PAUSE