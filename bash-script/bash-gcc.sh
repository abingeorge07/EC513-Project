#!/bin/bash -l

#$ -l h_rt=72:00:00
#$ -N gcc
#$ -pe omp 2

source /projectnb/ec513/materials/HW6/spec-2017/sourceme

cd /projectnb/ec513/students/abg309/gem5/HW6/spec-2017/gem5

build/X86/gem5.opt --outdir="/projectnb/ec513/students/abg309/gem5/HW6/Milestone2/gcc-1/" \
configs/example/gem5_library/x86-spec-cpu2017-benchmarks-1.py \
--image ../disk-image/spec-2017/spec-2017-image/spec-2017 \
--partition 1 \
--benchmark 502.gcc_r \
--size test

build/X86/gem5.opt --outdir="/projectnb/ec513/students/abg309/gem5/HW6/Milestone2/gcc-2/" \
configs/example/gem5_library/x86-spec-cpu2017-benchmarks-2.py \
--image ../disk-image/spec-2017/spec-2017-image/spec-2017 \
--partition 1 \
--benchmark 502.gcc_r \
--size test

build/X86/gem5.opt --outdir="/projectnb/ec513/students/abg309/gem5/HW6/Milestone2/gcc-4/" \
configs/example/gem5_library/x86-spec-cpu2017-benchmarks-4.py \
--image ../disk-image/spec-2017/spec-2017-image/spec-2017 \
--partition 1 \
--benchmark 502.gcc_r \
--size test