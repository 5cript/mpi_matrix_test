#!/bin/bash

PARTITION=lena

COMPILE=$(qsub remote_build_job.sh -W x=PARTITION:$PARTITION)
RUN=$(qsub -W depend=afterok:$COMPILE remote_run_job.sh -W x=PARTITION:$PARTITION)

echo "Compilation Task: $COMPILE"
echo "Run Task: $RUN"

CID="${COMPILE%%.*}"
RID="${RUN%%.*}"

