#!/bin/bash

NUM_PROCS=8
RUNTIME=30m

counter=1

while [ $counter -le 8 ]
do
    echo "Submitting job: ${counter} of ${NUM_PROCS}"
    OUTFILE="OUTPUT-${counter}.txt"
    sqsub -r ${RUNTIME} -q mpi --mpp=3G -n ${counter} -o ${OUTFILE} ./prime_gaps 1000000000
    ((counter++))
done
echo "All jobs have been submitted"