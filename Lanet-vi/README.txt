1. yum install gcc
   yum install gcc-c++

2. yum install gsl-devel

3. yum install blas
   yum install blas-devel

4. yum install librsvg2
   yum install librsvg2-tools -y

5. make && make clean

6. 运行
$ ./lanet -input input.txt -output res -logstdout -render svg -coresfile aaa.txt
$ ./lanet -input facebook_combined.txt -edges 0.2 -opacity 0.1 -render svg -logstdout -logfile -W 1920 -H 1080  -drawCircles
$ ./lanet -input ../facebook_combined.txt -edges 0.2 -opacity 0.1 -render svg -logstdout -logfile -W 2560 -H 1440 -bckgnd white



rpm2cpio gcc-4.8.5-39.el7.x86_64.rpm |cpio -idvm
rpm2cpio gcc-c++-4.8.5-39.el7.x86_64.rpm |cpio -idvm
rpm2cpio gsl-devel-1.15-13.el7.x86_64.rpm |cpio -idvm
rpm2cpio blas-3.4.2-8.el7.x86_64.rpm |cpio -idvm
rpm2cpio blas-devel-3.4.2-8.el7.x86_64.rpm |cpio -idvm
rpm2cpio librsvg2-2.40.20-1.el7.x86_64.rpm |cpio -idvm
rpm2cpio librsvg2-tools-2.40.20-1.el7.x86_64.rpm |cpio -idvm

