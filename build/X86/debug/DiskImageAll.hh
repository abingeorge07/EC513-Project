/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/debugflaghh.py:139
 */

#ifndef __DEBUG_DiskImageAll_HH__
#define __DEBUG_DiskImageAll_HH__

#include "base/compiler.hh" // For namespace deprecation
#include "base/debug.hh"
#include "debug/DiskImageRead.hh"
#include "debug/DiskImageWrite.hh"
namespace gem5
{

namespace debug
{

namespace unions
{
inline union DiskImageAll
{
    ~DiskImageAll() {}
    CompoundFlag DiskImageAll = {
        "DiskImageAll", "", {
            (Flag *)&::gem5::debug::DiskImageRead,
            (Flag *)&::gem5::debug::DiskImageWrite
        }
    };
} DiskImageAll;
} // namespace unions

inline constexpr const auto& DiskImageAll =
    ::gem5::debug::unions::DiskImageAll.DiskImageAll;

} // namespace debug
} // namespace gem5

#endif // __DEBUG_DiskImageAll_HH__
