pathToSubmodule = $1

# On Linux path is path/to/submodule

# Remove the submodule entry from .git/config
git submodule deinit -f pathToSubmodule

# Remove the submodule directory from the superproject's .git/modules directory
rm -rf .git/modules/pathToSubmodule

# Remove the submodule files from the working tree and index
git rm --cached Elven/vendor/imgui

# Remove the entry in .gitmodules and remove the submodule directory located at path/to/submodule
git rm -f pathToSubmodule
