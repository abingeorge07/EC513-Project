/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   build_tools/sim_object_param_struct_cc.py:297
 */

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include <type_traits>

#include "base/compiler.hh"
#include "params/SimpleUart.hh"
#include "sim/init.hh"
#include "sim/sim_object.hh"

#include "dev/serial/simple.hh"

#include "enums/ByteOrder.hh"
#include "base/types.hh"
namespace py = pybind11;

namespace gem5
{

static void
module_init(py::module_ &m_internal)
{
py::module_ m = m_internal.def_submodule("param_SimpleUart");
    py::class_<SimpleUartParams, UartParams, std::unique_ptr<SimpleUartParams, py::nodelete>>(m, "SimpleUartParams")
        .def(py::init<>())
        .def("create", &SimpleUartParams::create)
        .def_readwrite("byte_order", &SimpleUartParams::byte_order)
        .def_readwrite("end_on_eot", &SimpleUartParams::end_on_eot)
        .def_readwrite("pio_size", &SimpleUartParams::pio_size)
        ;

    py::class_<gem5::SimpleUart, gem5::Uart, std::unique_ptr<gem5::SimpleUart, py::nodelete>>(m, "gem5_COLONS_SimpleUart")
        ;

}

static EmbeddedPyBind embed_obj("SimpleUart", module_init, "Uart");

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
class DummySimpleUartParamsClass
{
  public:
    gem5::SimpleUart *create() const;
};

template <class CxxClass, class Enable=void>
class DummySimpleUartShunt;

/*
 * This version directs to the real Params struct and the
 * default behavior of create if there's an appropriate
 * constructor.
 */
template <class CxxClass>
class DummySimpleUartShunt<CxxClass, std::enable_if_t<
    std::is_constructible_v<CxxClass, const SimpleUartParams &>>>
{
  public:
    using Params = SimpleUartParams;
    static gem5::SimpleUart *
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
class DummySimpleUartShunt<CxxClass, std::enable_if_t<
    !std::is_constructible_v<CxxClass, const SimpleUartParams &>>>
{
  public:
    using Params = DummySimpleUartParamsClass;
    static gem5::SimpleUart *
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
[[maybe_unused]] gem5::SimpleUart *
DummySimpleUartShunt<gem5::SimpleUart>::Params::create() const
{
    return DummySimpleUartShunt<gem5::SimpleUart>::create(*this);
}

} // namespace gem5
