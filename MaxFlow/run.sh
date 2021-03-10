#!/bin/bash

rm -f flow_res.txt 
rm -f data/Graph.txt data/Vertex.txt data/VIC.txt
cd src/
python save_max_flow_net.py
python Schedule.py