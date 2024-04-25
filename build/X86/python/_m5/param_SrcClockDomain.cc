/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/sim_object_param_struct_cc.py:297
 */

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include <type_traits>

#include "base/compiler.hh"
#include "params/SrcClockDomain.hh"
#include "sim/init.hh"
#include "sim/sim_object.hh"

#include "sim/clock_domain.hh"

#include <vector>
#include "base/types.hh"
#include "base/types.hh"
#include "base/types.hh"
#include "sim/voltage_domain.hh"
namespace py = pybind11;

namespace gem5
{

static void
module_init(py::module_ &m_internal)
{
py::module_ m = m_internal.def_submodule("param_SrcClockDomain");
    py::class_<SrcClockDomainParams, ClockDomainParams, std::unique_ptr<SrcClockDomainParams, py::nodelete>>(m, "SrcClockDomainParams")
        .def(py::init<>())
        .def("create", &SrcClockDomainParams::create)
        .def_readwrite("clock", &SrcClockDomainParams::clock)
        .def_readwrite("domain_id", &SrcClockDomainParams::domain_id)
        .def_readwrite("init_perf_level", &SrcClockDomainParams::init_perf_level)
        .def_readwrite("voltage_domain", &SrcClockDomainParams::voltage_domain)
        ;

    py::class_<gem5::SrcClockDomain, gem5::ClockDomain, std::unique_ptr<gem5::SrcClockDomain, py::nodelete>>(m, "gem5_COLONS_SrcClockDomain")
        ;

}

static EmbeddedPyBind embed_obj("SrcClockDomain", module_init, "ClockDomain");

} // namespace gem5
namespace gem5
{

namespace
{

/*
 * If we can't define a default create() method for this params
 * struct because the SimObject doesn't have the right
 * constructor, use template magic to make it so we're actually
 * defining a create method for this class instead.
 */
class DummySrcClockDomainParamsClass
{
  public:
    gem5::SrcClockDomain *create() const;
};

template <class CxxClass, class Enable=void>
class DummySrcClockDomainShunt;

/*
 * This version directs to the real Params struct and the
 * default behavior of create if there's an appropriate
 * constructor.
 */
template <class CxxClass>
class DummySrcClockDomainShunt<CxxClass, std::enable_if_t<
    std::is_constructible_v<CxxClass, const SrcClockDomainParams &>>>
{
  public:
    using Params = SrcClockDomainParams;
    static gem5::SrcClockDomain *
    create(const Params &p)
    {
        return new CxxClass(p);
    }
};

/*
 * This version diverts to the DummyParamsClass and a dummy
 * implementation of create if the appropriate constructor does
 * not exist.
 */
template <class CxxClass>
class DummySrcClockDomainShunt<CxxClass, std::enable_if_t<
    !std::is_constructible_v<CxxClass, const SrcClockDomainParams &>>>
{
  public:
    using Params = DummySrcClockDomainParamsClass;
    static gem5::SrcClockDomain *
    create(const Params &p)
    {
        return nullptr;
    }
};

} // anonymous namespace

/*
 * An implementation of either the real Params struct's create
 * method, or the Dummy one. Either an implementation is
 * mandantory since this was shunted off to the dummy class, or
 * one is optional which will override this weak version.
 */
[[maybe_unused]] gem5::SrcClockDomain *
DummySrcClockDomainShunt<gem5::SrcClockDomain>::Params::create() const
{
    return DummySrcClockDomainShunt<gem5::SrcClockDomain>::create(*this);
}

} // namespace gem5
