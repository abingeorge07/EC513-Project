/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/sim_object_param_struct_cc.py:297
 */

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include <type_traits>

#include "base/compiler.hh"
#include "params/PowerDomain.hh"
#include "sim/init.hh"
#include "sim/sim_object.hh"

#include "sim/power_domain.hh"

namespace py = pybind11;

namespace gem5
{

static void
module_init(py::module_ &m_internal)
{
py::module_ m = m_internal.def_submodule("param_PowerDomain");
    py::class_<PowerDomainParams, PowerStateParams, std::unique_ptr<PowerDomainParams, py::nodelete>>(m, "PowerDomainParams")
        .def(py::init<>())
        .def("create", &PowerDomainParams::create)
        ;

    py::class_<gem5::PowerDomain, gem5::PowerState, std::unique_ptr<gem5::PowerDomain, py::nodelete>>(m, "gem5_COLONS_PowerDomain")
        ;

}

static EmbeddedPyBind embed_obj("PowerDomain", module_init, "PowerState");

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
class DummyPowerDomainParamsClass
{
  public:
    gem5::PowerDomain *create() const;
};

template <class CxxClass, class Enable=void>
class DummyPowerDomainShunt;

/*
 * This version directs to the real Params struct and the
 * default behavior of create if there's an appropriate
 * constructor.
 */
template <class CxxClass>
class DummyPowerDomainShunt<CxxClass, std::enable_if_t<
    std::is_constructible_v<CxxClass, const PowerDomainParams &>>>
{
  public:
    using Params = PowerDomainParams;
    static gem5::PowerDomain *
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
class DummyPowerDomainShunt<CxxClass, std::enable_if_t<
    !std::is_constructible_v<CxxClass, const PowerDomainParams &>>>
{
  public:
    using Params = DummyPowerDomainParamsClass;
    static gem5::PowerDomain *
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
[[maybe_unused]] gem5::PowerDomain *
DummyPowerDomainShunt<gem5::PowerDomain>::Params::create() const
{
    return DummyPowerDomainShunt<gem5::PowerDomain>::create(*this);
}

} // namespace gem5
