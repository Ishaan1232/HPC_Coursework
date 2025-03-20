#!/bin/bash

# Define output files
OUTPUT="cuda_results.txt"

# Clear previous results
echo "N Time(s)" > $OUTPUT

# Loop over N from 1 to 1000 in steps of 50
for ((N=50; N<=1000; N+=50)); do
    echo "Running with N=$N particles"

    # Measure Serial Execution Time
    SERIAL_TIME=$( { time ./md --ic-random --T 5 --temp 20 --Lx 35 --Ly 35 --Lz 35 --N $N; } 2>&1 | grep real | awk '{print $2}' )
    echo "Serial: $N $SERIAL_TIME" >> $OUTPUT

    # Measure CUDA Execution Time
    CUDA_TIME=$( { time ./mdcuda --ic-random --T 5 --temp 20 --Lx 35 --Ly 35 --Lz 35 --N $N; } 2>&1 | grep real | awk '{print $2}' )
    echo "CUDA: $N $CUDA_TIME" >> $OUTPUT

done

echo "Benchmark complete"
