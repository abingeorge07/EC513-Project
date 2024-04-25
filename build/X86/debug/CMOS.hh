/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/debugflaghh.py:139
 */

#ifndef __DEBUG_CMOS_HH__
#define __DEBUG_CMOS_HH__

#include "base/compiler.hh" // For namespace deprecation
#include "base/debug.hh"
namespace gem5
{

namespace debug
{

namespace unions
{
inline union CMOS
{
    ~CMOS() {}
    SimpleFlag CMOS = {
        "CMOS", "Accesses to CMOS devices", false
    };
} CMOS;
} // namespace unions

inline constexpr const auto& CMOS =
    ::gem5::debug::unions::CMOS.CMOS;

} // namespace debug
} // namespace gem5

#endif // __DEBUG_CMOS_HH__
