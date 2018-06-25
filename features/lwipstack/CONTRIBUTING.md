# Description

This document describes how to update the Arm Mbed OS LWIP stack. The Mbed OS LWIP stack is a copy of the LWIP master repository. Stack is located in `features/lwipstack/lwip` directory.

When you add new releases or single commits from the LWIP master repository, you must fetch them using the Mbed OS LWIP fork repository. Use the repository to rename LWIP source files with the `lwip_` prefix to make them compatible with the Mbed OS build system.

* The LWIP master repository is part of the [LWIP project](https://savannah.nongnu.org/projects/lwip).
* The Mbed OS LWIP fork repository is part of [`ARMmbed`](https://github.com/ARMmbed/lwip).

## How to integrate a release from the LWIP master repository to the Mbed OS

You can integrate a new release using the following steps.

* Fetch the release to the Mbed OS LWIP fork.
* Prefix files in the Mbed OS LWIP fork with the `lwip_` prefix.
* Merge the release to the Mbed OS repository.

### Fetching the release to the Mbed OS LWIP fork and prefixing files

1. Clone the Mbed OS LWIP fork repository.

    `git clone git@github.com:ARMmbed/lwip.git`

2. Go to the Mbed OS LWIP fork root directory, and add the LWIP master repository as remote.

    `git remote add <remote name> https://git.savannah.nongnu.org/git/lwip.git`

    `e.g. git remote add lwip https://git.savannah.nongnu.org/git/lwip.git`

3. Pull the release from the LWIP master repository using the release tag. You can find tags [here](<https://savannah.nongnu.org/projects/lwip>).

    `git pull <remote name> <tag>`

    `e.g. git pull lwip STABLE-2_0_2_RELEASE_VER`

4. Push the tag to the Mbed OS LWIP fork repository to keep the fork in sync.

    `git push origin <tag>`

    `e.g. git push origin STABLE-2_0_2_RELEASE_VER`

5. Create a branch for release. Use the same naming convention as previous branches.

    `git branch <branch name> <tag>`

    `e.g. git branch mbed-os-lwip-stable-2_0_2 STABLE-2_0_2_RELEASE_VER`

6. Push the branch to the Mbed OS LWIP fork repository to keep the fork in sync.

    `git push origin <branch name>`

    `e.g. git push origin mbed-os-lwip-stable-2_0_2`

7. Fetch the previous prefixed branch from the Mbed OS LWIP fork repository.

    `git fetch <remote name> <remote branch>:<local branch>` 

    `e.g. git fetch origin mbed-os-lwip-stable-2_0_1-prefixed:mbed-os-lwip-stable-2_0_1-prefixed`

8. Checkout the latest prefixed branch.

    `git checkout <prefixed branch>`

    `e.g. git checkout mbed-os-lwip-stable-2_0_1-prefixed`

9. Merge the new branch to the prefixed branch.

    `git merge <branch name>`

    `e.g. git merge mbed-os-lwip-stable-2_0_2`

10. Check if there are any new or colliding files.

    If there are new c-files rename those with `lwip_` prefix and make a commit about changes to the prefixed branch. Also, check that LWIP stack header file names do not collide with headers under the Mbed OS directory tree. If there are collisions, rename the colliding LWIP headers with `lwip_` prefix.

    If needed, rename files, and make a commit. Update the C-files to include any renamed header files.

    `git mv <old filename> <new filename>`
    
    `git commit`    

11. Rename the local prefixed branch to the new version. (Do not change branch names on remotes.)

    `git branch -m <new prefixed branch>`

    `e.g. git branch -m mbed-os-lwip-stable-2_0_2-prefixed`

12. Push the new local prefixed branch to the Mbed OS LWIP fork repository.

    `git push origin <new prefixed branch>`

    `e.g. git push origin mbed-os-lwip-stable-2_0_2-prefixed`

### Merging the prefixed release to the Mbed OS repository

1. Clone the Mbed OS repository.

    `git clone git@github.com:ARMmbed/mbed-os.git`

2. Go to the Mbed OS root directory, and add the Mbed OS LWIP fork repository as remote. Fetch branches from fork.

    `git remote add <remote name> https://github.com/ARMmbed/lwip.git`

    `e.g. git remote add lwip-fork https://github.com/ARMmbed/lwip.git`

    `git fetch <remote name>`

    `e.g. git fetch lwip-fork`

3. Do a subtree pull for LWIP prefixed branch in the Mbed OS root directory.

    `git subtree pull --squash -P features/lwipstack/lwip <remote name> <new prefixed branch> -m "<commit message>"`

    `e.g. git subtree pull --squash -P features/lwipstack/lwip lwip-fork mbed-os-lwip-stable-2_0_2-prefixed -m "Merged lwip 2.0.2 stable"`

    After this step, there is a new commit visible in the Mbed OS master branch that contains the changes.

4. Verify that changes in the new commit are correct, and create a new branch. Push the branch to your Mbed OS fork for GitHub review.
