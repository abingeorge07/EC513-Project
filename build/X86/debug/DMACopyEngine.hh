/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/debugflaghh.py:139
 */

#ifndef __DEBUG_DMACopyEngine_HH__
#define __DEBUG_DMACopyEngine_HH__

#include "base/compiler.hh" // For namespace deprecation
#include "base/debug.hh"
namespace gem5
{

namespace debug
{

namespace unions
{
inline union DMACopyEngine
{
    ~DMACopyEngine() {}
    SimpleFlag DMACopyEngine = {
        "DMACopyEngine", "", false
    };
} DMACopyEngine;
} // namespace unions

inline constexpr const auto& DMACopyEngine =
    ::gem5::debug::unions::DMACopyEngine.DMACopyEngine;

} // namespace debug
} // namespace gem5

#endif // __DEBUG_DMACopyEngine_HH__
