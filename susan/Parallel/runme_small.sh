#!/bin/sh
./susan_parallel input_small.pgm output_small.smoothing.pgm -s
./susan_parallel input_small.pgm output_small.edges.pgm -e
./susan_parallel input_small.pgm output_small.corners.pgm -c

