#!/bin/bash

#
# Export FEATURE_NANOSTACK libraries into mbed OS
#
# Usage:
#
# To remove all repositories call: ./clone_client.sh clean
# To replace all with clone repositores: ./clone_client.sh clone
# To export from cloned repositories: ./clone_client.sh export
#
# For developers:
#    Use "clone" functionality to get master repositories to work with.
#
# For maintainers: Creating a release
#    Use "clone" to get master repositories.
#    For every directory checkout the release branch.
#    Now use "export" function to flatten the git repositories into bare directories
#    Now you can "git add" each directory and do "git commit"
#
# Note "clone" does implicit "clean" so you lose all changes done into directories.


# REPOSITORIES and DIRECTORIES are array of repositories where to clone
# and directories where to clone
# these arrays MUST be in sync
#  * Same number of elements
#  * Same order. Each line should have maching pair in same index at other array
REPOSITORIES=(
    git@github.com:ARMmbed/sal-stack-nanostack.git
    git@github.com:ARMmbed/coap-service.git
    )

DIRECTORIES=(
    sal-stack-nanostack
    coap-service
    )

# Exit immediately on fail, thread unset variables as error
set -eu

# Count number of repositories
N=0
for repo in ${REPOSITORIES[*]}; do
    let N=N+1
done
let N=N-1 # Substract one, because indexes start from 0

print_usage() {
    echo -e "Usage: $0 [clean | clone | export ]"
    echo -e "\tclean : Remove all repositories"
    echo -e "\tclone : Replace repositories with Git cone"
    echo -e "\texport : Export cloned Git repositories"
}

clean() {
    for dir in ${DIRECTORIES[*]}; do
        rm -rf $dir
    done
    clean_nanostack_binaries
}

clone() {
    for i in $(seq 0 $N); do
        git clone ${REPOSITORIES[$i]} ${DIRECTORIES[$i]}
    done
}

export_repos() {
    for i in $(seq 0 $N); do
        echo "Exporting ${DIRECTORIES[$i]}"
        cd ${DIRECTORIES[$i]}
        git archive HEAD -o ../${DIRECTORIES[$i]}.tar.gz
        cd ..
        rm -rf ${DIRECTORIES[$i]}
        mkdir ${DIRECTORIES[$i]}
        tar xzf ${DIRECTORIES[$i]}.tar.gz -C ${DIRECTORIES[$i]}
        rm ${DIRECTORIES[$i]}.tar.gz
    done
}

copy_nanostack_binaries() {
    mkdir -p ../nanostack-binaries
    mv sal-stack-nanostack/FEATURE_* ../nanostack-binaries/
}

clean_nanostack_binaries() {
    rm -rf ../nanostack-binaries
}

case "${1-}" in
    clean)
        clean
        ;;
    clone)
        clean
        clone
        ;;
    export)
        export_repos
        copy_nanostack_binaries
        ;;
    *)
        print_usage
        exit 1
        ;;
esac
