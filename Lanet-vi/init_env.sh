#!/bin/bash

# step 1
yum install gcc -y
yum install gcc-c++ -y

# step 2
yum install gsl-devel -y

# step 3
yum install blas -y
yum install blas-devel -y

# step 4
yum install librsvg2 -y
yum install librsvg2-tools -y

# step 5
cd LaNet-linux-vi_3.0.1
make && make clean
