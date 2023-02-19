@echo off
pushd %~dp0\..\Engine\vendor
call cmake -B ..\..\build_vendor -A x64 -DFT_DISABLE_ZLIB=ON -DFT_DISABLE_BZIP2=ON -DFT_DISABLE_PNG=ON -DFT_DISABLE_HARFBUZZ=ON -DFT_DISABLE_BROTLI=ON
xcopy irrklang\bin\win\ ..\..\build_vendor\irrklang\win\
popd
pushd %~dp0\..\
call cmake --build build_vendor --config Debug
call cmake --build build_vendor --config Release
popd
PAUSE