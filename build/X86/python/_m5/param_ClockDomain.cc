/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/sim_object_param_struct_cc.py:297
 */

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include <type_traits>

#include "base/compiler.hh"
#include "params/ClockDomain.hh"
#include "sim/init.hh"
#include "sim/sim_object.hh"

#include "sim/clock_domain.hh"

namespace py = pybind11;

namespace gem5
{

static void
module_init(py::module_ &m_internal)
{
py::module_ m = m_internal.def_submodule("param_ClockDomain");
    py::class_<ClockDomainParams, SimObjectParams, std::unique_ptr<ClockDomainParams, py::nodelete>>(m, "ClockDomainParams")
        ;

    py::class_<gem5::ClockDomain, gem5::SimObject, std::unique_ptr<gem5::ClockDomain, py::nodelete>>(m, "gem5_COLONS_ClockDomain")
        ;

}

static EmbeddedPyBind embed_obj("ClockDomain", module_init, "SimObject");

} // namespace gem5
