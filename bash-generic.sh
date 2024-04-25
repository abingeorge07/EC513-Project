#!/bin/bash -l

#$ -l h_rt=72:00:00
#$ -N bwaves-1
#$ -pe omp 2

source /projectnb/ec513/materials/HW6/spec-2017/sourceme

cd /projectnb/ec513/students/abg309/Project/HW6/spec-2017/EC513-Project


build/X86/gem5.opt --outdir="/projectnb/ec513/students/abg309/Project/Results/generic-results/" \
/projectnb/ec513/students/abg309/Project/HW6/spec-2017/EC513-Project/configs/example/gem5_library/x86-spec-cpu2017-benchmarks.py \
--image ../disk-image/spec-2017/spec-2017-image/spec-2017 \
--partition 1 \
--benchmark 500.perlbench_r \
--size test 
