# EC513-Project

## UPDATE 1: Changes Made from Original GEM5 Source
- [l1_cache.py](src/python/gem5/components/cachehierarchies\ruby\caches/mesi_two_level/l1_cache.py) : Lines 62 & 69 changed replacement policy to random
- [l2_cache.py](src/python/gem5/components/cachehierarchies\ruby\caches/mesi_two_level/l2_cache.py) : Line 46 changed replacement policy to random

- [config file](configs\example\gem5_library\x86-spec-cpu2017-benchmarks.py) : Updated config file to set number of instructions to run, but not working too well :(


## UPDATE 2: Changes made to use "Classic Cache Hierarchy"
- [new-config file](configs\example\gem5_library\x86-spec-cpu2017-benchmarks.py):
instead of using MESI cache, it uses PrivateL1SharedL2CacheHierarchy

- [l2cache.py](/src/python/gem5/components/cachehierarchies/classic/caches/l2cache.py): Using a new cache class (Cache2) which is a copy of Cache.py, the main reason for this change is to be able to use two cache replacement policy for each level of cache at the same time. (src/python/gem5/components/cachehierarchies/classic/caches/l2cache.py)

- [Cache.py](/src/mem/cache/Cache.py): Includes the class Cache2 which is used by the L2 cache. (src/mem/cache/Cache.py)

## IMPORTANT NOTE
- After a clean install, execute these to avoid errors:
```bash
chmod +x util/cpt_upgrader.py
chmod +x ext/libelf/native-elf-format
```
If you see an error like
```bash
param_HawkeyeRP.cc:(.rodata.cst8+0x28): undefined reference to `gem5::HawkeyeRPParams::create() const'
```
then please refer to this link:
https://stackoverflow.com/questions/72502976/unable-to-build-gem5-after-creation-of-custom-replacement-policy-undefined-refe
### Implementation
1) Create your own replacement policy.
2) In order to set the replacement policies for both caches, change the replacement policy attribute of both classes Cache(L1D & L1I) and Cache2(L2) which can be found [/src/mem/cache/Cache.py](/src/mem/cache/Cache.py).

### Note
- The stat file is now a little different since we using a different hierarchy, but miss % and CPI can still be found.
- It would be nice if we could change the replacement policy from the config file and not have to re-compile everytime.
