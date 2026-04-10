#!/usr/bin/env bash

PACKAGENAME=digitsynth-aarch64

cd build/
mkdir -p $PACKAGENAME/DEBIAN/
mkdir -p $PACKAGENAME/usr/bin/ 

cp control.in $PACKAGENAME/DEBIAN/control
version="$(cat ../VERSION)-$(git rev-parse HEAD)"
sed -i "s/VERSION/$version/g" $PACKAGENAME/DEBIAN/control 

cp digitsynth $PACKAGENAME/usr/bin/
dpkg-deb --root-owner-group --build $PACKAGENAME

