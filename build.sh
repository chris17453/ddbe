#!/bin/bash

HEADER=builds/ddb.h
SOURCE=builds/ddb.c


# This gets the curent version.. and bumps the patch +1
version=source/version.txt
major=$(cat $version| cut -d '.' -f 1)
minor=$(cat $version| cut -d '.' -f 2)
patch=$(cat $version| cut -d '.' -f 3)

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
echo $build>$version


git add -add
git commit -m 'Commiting Build '$build

echo '// ddb all in one source'>$SOURCE
echo '#include "ddb.h"'>>$SOURCE
echo ''>>$SOURCE
echo ''>>$SOURCE
for x in $(find source/ |grep '\.c')
do 
  echo '// *** BEGIN '$x>>$SOURCE
  cat $x|sed '/#include "/d'>>$SOURCE; 
  echo '// *** END'>>$SOURCE; 
  echo ''>>$SOURCE; 
done




echo '// ddb all in one header'>$HEADER
cat $SOURCE| grep include| sed 's/[ \t]*$//' | sort| uniq>>$HEADER
echo ''>>$HEADER
echo ''>>$HEADER
for x in $(cat source/include/include_order.txt)
do 
  echo '// *** BEGIN '$x>>$HEADER
  cat $x>>$HEADER; 
  echo '// *** END'>>$HEADER; 
  echo ''>>$HEADER; 
done

echo ''>>$HEADER

# delete all local includes fomr this header... 
sed -i '/#include "/d'  $HEADER

sed -i '/#include </d'  $SOURCE
