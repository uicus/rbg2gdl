#/bin/sh

git pull
git submodule foreach --recursive git fetch
git submodule foreach git merge origin master
