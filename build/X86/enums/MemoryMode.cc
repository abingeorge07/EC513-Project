/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/enum_cc.py:169
 */

#include "base/compiler.hh"
#include "enums/MemoryMode.hh"

namespace gem5
{

namespace enums
{
    const char *MemoryModeStrings[Num_MemoryMode] =
    {
        "invalid",
        "atomic",
        "timing",
        "atomic_noncaching",
    };
} // namespace enums
} // namespace gem5
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include <sim/init.hh>

namespace py = pybind11;

namespace gem5
{

static void
module_init(py::module_ &m_internal)
{
    py::module_ m = m_internal.def_submodule("enum_MemoryMode");

py::enum_<enums::MemoryMode>(m, "enum_MemoryMode")
        .value("invalid", enums::invalid)
        .value("atomic", enums::atomic)
        .value("timing", enums::timing)
        .value("atomic_noncaching", enums::atomic_noncaching)
        .value("Num_MemoryMode", enums::Num_MemoryMode)
        .export_values()
        ;
    }
static EmbeddedPyBind embed_enum("enum_MemoryMode", module_init);

} // namespace gem5
    
