/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/sim_object_param_struct_cc.py:297
 */

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include <type_traits>

#include "base/compiler.hh"
#include "params/PciBarNone.hh"
#include "sim/init.hh"
#include "sim/sim_object.hh"

#include "dev/pci/device.hh"

namespace py = pybind11;

namespace gem5
{

static void
module_init(py::module_ &m_internal)
{
py::module_ m = m_internal.def_submodule("param_PciBarNone");
    py::class_<PciBarNoneParams, PciBarParams, std::unique_ptr<PciBarNoneParams, py::nodelete>>(m, "PciBarNoneParams")
        .def(py::init<>())
        .def("create", &PciBarNoneParams::create)
        ;

    py::class_<gem5::PciBarNone, gem5::PciBar, std::unique_ptr<gem5::PciBarNone, py::nodelete>>(m, "gem5_COLONS_PciBarNone")
        ;

}

static EmbeddedPyBind embed_obj("PciBarNone", module_init, "PciBar");

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
class DummyPciBarNoneParamsClass
{
  public:
    gem5::PciBarNone *create() const;
};

template <class CxxClass, class Enable=void>
class DummyPciBarNoneShunt;

/*
 * This version directs to the real Params struct and the
 * default behavior of create if there's an appropriate
 * constructor.
 */
template <class CxxClass>
class DummyPciBarNoneShunt<CxxClass, std::enable_if_t<
    std::is_constructible_v<CxxClass, const PciBarNoneParams &>>>
{
  public:
    using Params = PciBarNoneParams;
    static gem5::PciBarNone *
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
class DummyPciBarNoneShunt<CxxClass, std::enable_if_t<
    !std::is_constructible_v<CxxClass, const PciBarNoneParams &>>>
{
  public:
    using Params = DummyPciBarNoneParamsClass;
    static gem5::PciBarNone *
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
[[maybe_unused]] gem5::PciBarNone *
DummyPciBarNoneShunt<gem5::PciBarNone>::Params::create() const
{
    return DummyPciBarNoneShunt<gem5::PciBarNone>::create(*this);
}

} // namespace gem5
