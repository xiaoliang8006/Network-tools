#!/bin/bash
input_file=facebook_combined.txt
./lanet -input $input_file -edges 0.2 -opacity 0.1 -render svg -logstdout -logfile -W 1920 -H 1080  -drawCircles
#./lanet -input ../facebook_combined.txt -edges 0.2 -opacity 0.1 -render svg -logstdout -logfile -W 2560 -H 1440 -bckgnd white

mv *.svg result/
mv *.png result/

