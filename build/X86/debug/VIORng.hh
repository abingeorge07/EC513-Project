/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/debugflaghh.py:139
 */

#ifndef __DEBUG_VIORng_HH__
#define __DEBUG_VIORng_HH__

#include "base/compiler.hh" // For namespace deprecation
#include "base/debug.hh"
namespace gem5
{

namespace debug
{

namespace unions
{
inline union VIORng
{
    ~VIORng() {}
    SimpleFlag VIORng = {
        "VIORng", "VirtIO entropy source device", false
    };
} VIORng;
} // namespace unions

inline constexpr const auto& VIORng =
    ::gem5::debug::unions::VIORng.VIORng;

} // namespace debug
} // namespace gem5

#endif // __DEBUG_VIORng_HH__
