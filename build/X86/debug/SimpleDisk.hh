/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/debugflaghh.py:139
 */

#ifndef __DEBUG_SimpleDisk_HH__
#define __DEBUG_SimpleDisk_HH__

#include "base/compiler.hh" // For namespace deprecation
#include "base/debug.hh"
namespace gem5
{

namespace debug
{

namespace unions
{
inline union SimpleDisk
{
    ~SimpleDisk() {}
    SimpleFlag SimpleDisk = {
        "SimpleDisk", "", false
    };
} SimpleDisk;
} // namespace unions

inline constexpr const auto& SimpleDisk =
    ::gem5::debug::unions::SimpleDisk.SimpleDisk;

} // namespace debug
} // namespace gem5

#endif // __DEBUG_SimpleDisk_HH__
