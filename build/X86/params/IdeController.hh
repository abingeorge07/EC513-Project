/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/sim_object_param_struct_hh.py:235
 */

#ifndef __PARAMS__IdeController__
#define __PARAMS__IdeController__

namespace gem5 {
class IdeController;
} // namespace gem5
#include <cstddef>
#include "base/types.hh"
#include <vector>
#include "params/IdeDisk.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/PciDevice.hh"

namespace gem5
{
struct IdeControllerParams
    : public PciDeviceParams
{
    gem5::IdeController * create() const;
    uint32_t ctrl_offset;
    std::vector< gem5::IdeDisk * > disks;
    uint32_t io_shift;
};

} // namespace gem5

#endif // __PARAMS__IdeController__
