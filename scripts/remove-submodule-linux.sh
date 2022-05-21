# Back to root directory
cd -

# Get submodule name from arguments
submodule_name = $1

# Remove the submodule entry from .git/config
git submodule deinit -f ../Engine/vendor/$submodule_name

# Remove the submodule directory from the superproject's .git/modules directory
rm -rf .git/modules/Engine/vendor/$submodule_name

# Remove the submodule files from the working tree and index
git rm --cached Engine/vendor/$submodule_name

# Remove the entry in .gitmodules and remove the submodule directory located at path/to/submodule
git rm -f Engine/vendor/$submodule_name
