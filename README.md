# EC513-Project

## Changes Made from Original GEM5 Source
- [l1_cache.py](src/python/gem5/components/cachehierarchies\ruby\caches/mesi_two_level/l1_cache.py) : Lines 62 & 69 changed replacement policy to random
- [l2_cache.py](src/python/gem5/components/cachehierarchies\ruby\caches/mesi_two_level/l2_cache.py) : Line 46 changed replacement policy to random

- [config file](configs\example\gem5_library\x86-spec-cpu2017-benchmarks.py) : Updated config file to set number of instructions to run, but not working too well :(