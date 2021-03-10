#!/bin/bash


rm -rf AllClique.res Allleaf.txt SubNetwork/ result/
mkdir SubNetwork

cd src/
# exec
make clean && make all
# Divide the network
./OverlapCommunityShell $1

mv All* ../
mv Sub* ../SubNetwork/
mv result/ ../
echo "complete!"