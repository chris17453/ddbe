#!/bin/bash
version=source/version.txt


# This gets the curent version.. and bumps the patch +1
major=$(cat $version| cut -d '.' -f 1)
minor=$(cat $version| cut -d '.' -f 2)
patch=$(cat $version| cut -d '.' -f 3)

# overflow logic
patch=$((patch+1))
if [[ $patch -gt 999 ]]; 
then 
  patch=0
  minor=$((minor+1))
fi
if [[ $minor -gt 999 ]]; 
then 
  minor=0
  major=$((major+1))
fi
build=$major.$minor.$patch

#put new version in text file
echo $build>$version

# commit everything with build #
git add -A
git commit -m 'Commiting Build '$build
