@echo off
pushd %~dp0\..\
call cmake -B build -G "Visual Studio 17 2022" -A x64 -D BUILD_SANDBOX=ON -D BUILD_GAMES=ON -D PROFILE_MODE=ON -D MODULE_EDITOR_ENABLED=ON -D MODULE_3D_ENABLED=ON -D MODULE_SOUND_ENABLED=ON >> scripts/configure.log
popd
PAUSE