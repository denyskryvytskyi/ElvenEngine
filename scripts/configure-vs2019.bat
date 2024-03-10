@echo off
pushd %~dp0\..\
call cmake -B build -G "Visual Studio 16 2019" -A x64 -D ENABLE_3D_MODULE=ON -D BUILD_SANDBOX=ON -D BUILD_GAMES=ON -D EDITOR_MODE=ON -D PROFILE_MODE=ON >> scripts/configure.log
popd
PAUSE