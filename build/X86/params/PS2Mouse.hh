/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/sim_object_param_struct_hh.py:235
 */

#ifndef __PARAMS__PS2Mouse__
#define __PARAMS__PS2Mouse__

namespace gem5 {
namespace ps2 {
class PS2Mouse;
} // namespace ps2
} // namespace gem5

#include "params/PS2Device.hh"

namespace gem5
{
struct PS2MouseParams
    : public PS2DeviceParams
{
    gem5::ps2::PS2Mouse * create() const;
};

} // namespace gem5

#endif // __PARAMS__PS2Mouse__
