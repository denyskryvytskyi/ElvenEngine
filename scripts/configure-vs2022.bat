@echo off
pushd %~dp0\..\
call cmake -B build -G "Visual Studio 17 2022" -A x64 -D THREE_D_MODE=ON -D BUILD_SANDBOX=ON -D BUILD_GAMES=ON -D EDITOR_MODE=ON -D PROFILE_MODE=ON >> scripts/configure.log
popd
PAUSE