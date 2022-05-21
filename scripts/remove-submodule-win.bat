@echo off
pushd %~dp0\..\

set submodule_name=%1

rem Remove the submodule entry from .git/config
git submodule deinit -f Engine/vendor/%submodule_name%

rem Remove the submodule directory from the superproject's .git/modules directory
rmdir /s /q .git/modules/Engine/vendor/%submodule_name%

rem Remove the submodule files from the working tree and index:
git rm --cached example: Elven/vendor/%submodule_name%

rem Remove the entry in .gitmodules and remove the submodule directory located at path/to/submodule
git rm -f Engine/vendor/%submodule_name%

popd
PAUSE