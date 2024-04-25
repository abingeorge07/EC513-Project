/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/sim_object_param_struct_hh.py:235
 */

#ifndef __PARAMS__AbstractMemory__
#define __PARAMS__AbstractMemory__

namespace gem5 {
namespace memory {
class AbstractMemory;
} // namespace memory
} // namespace gem5
#include <cstddef>
#include <cstddef>
#include <string>
#include <cstddef>
#include <cstddef>
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include "base/addr_range.hh"
#include <cstddef>

#include "params/ClockedObject.hh"

namespace gem5
{
struct AbstractMemoryParams
    : public ClockedObjectParams
{
    bool conf_table_reported;
    std::string image_file;
    bool in_addr_map;
    bool kvm_map;
    bool null;
    AddrRange range;
    bool writeable;
};

} // namespace gem5

#endif // __PARAMS__AbstractMemory__
