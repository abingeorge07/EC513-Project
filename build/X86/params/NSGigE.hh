/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/sim_object_param_struct_hh.py:235
 */

#ifndef __PARAMS__NSGigE__
#define __PARAMS__NSGigE__

namespace gem5 {
class NSGigE;
} // namespace gem5
#include <cstddef>
#include <cstddef>
#include <cstddef>

#include "params/EtherDevBase.hh"

namespace gem5
{
struct NSGigEParams
    : public EtherDevBaseParams
{
    gem5::NSGigE * create() const;
    bool dma_data_free;
    bool dma_desc_free;
    bool dma_no_allocate;
};

} // namespace gem5

#endif // __PARAMS__NSGigE__
