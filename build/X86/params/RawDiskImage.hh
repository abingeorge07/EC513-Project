/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/sim_object_param_struct_hh.py:235
 */

#ifndef __PARAMS__RawDiskImage__
#define __PARAMS__RawDiskImage__

namespace gem5 {
class RawDiskImage;
} // namespace gem5

#include "params/DiskImage.hh"

namespace gem5
{
struct RawDiskImageParams
    : public DiskImageParams
{
    gem5::RawDiskImage * create() const;
};

} // namespace gem5

#endif // __PARAMS__RawDiskImage__
