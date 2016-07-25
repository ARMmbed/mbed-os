This directory is used as a git subtree. The master copy is at

    https://github.com/ARMmbed/nanomesh-applications/tree/nvm

(ie the nvm branch of that repository).

If you are seeing this as a subdirectory of another repository,
or in Platforms/common/nvm of the same repository, it has been
pulled in via git subtree.

You should not normally edit files in this directory unless you are
working on the nvm branch in a clone of the master repository.

Changes to the nvm code would be done as follows:

    # nvm branch is the "master" for the code
    git checkout nvm

    # make and commit changes
    git checkout master
    git subtree merge -P Platforms/common/nvm nvm

To take the latest version of the nvm reference code into another
repository already containing it as a subtree, use the command:

    git subtree pull -P our/path/to/nvm \
            git@github.com:ARMmbed/nanomesh-applications nvm

To take this code into use as a subtree for the first time,
use the command:

    git subtree add -P our/path/to/nvm \
            git@github.com:ARMmbed/nanomesh-applications nvm

See 'git help subtree' for more information.

