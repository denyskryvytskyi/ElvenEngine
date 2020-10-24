@echo off
pushd %~dp0\..\

rem Remove the submodule entry from .git/config
git submodule deinit -f path/to/submodule

rem Remove the submodule directory from the superproject's .git/modules directory
rmdir /s /q .git\modules\path\to\submodule

rem Remove the entry in .gitmodules and remove the submodule directory located at path/to/submodule
git rm -f path/to/submodule

popd
PAUSE