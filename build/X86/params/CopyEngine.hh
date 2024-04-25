/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/sim_object_param_struct_hh.py:235
 */

#ifndef __PARAMS__CopyEngine__
#define __PARAMS__CopyEngine__

namespace gem5 {
class CopyEngine;
} // namespace gem5
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/PciDevice.hh"

namespace gem5
{
struct CopyEngineParams
    : public PciDeviceParams
{
    gem5::CopyEngine * create() const;
    uint8_t ChanCnt;
    uint64_t XferCap;
    Tick latAfterCompletion;
    Tick latBeforeBegin;
    unsigned int port_dma_connection_count;
};

} // namespace gem5

#endif // __PARAMS__CopyEngine__
