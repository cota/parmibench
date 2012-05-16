#!/bin/sh
./susan_parallel input_large.pgm output_large.smoothing.pgm -s
./susan_parallel input_large.pgm output_large.edges.pgm -e 
./susan_parallel input_large.pgm output_large.corners.pgm -c
