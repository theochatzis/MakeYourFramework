// The python module "myf._myf": the C++ classes python needs to configure and run a job.
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "myf/Looper.h"
#include "myf/ModuleFactory.h"

namespace py = pybind11;

PYBIND11_MODULE(_myf, m) {
  m.doc() = "C++ core of myf";

  py::class_<myf::Sample>(m, "Sample")
      .def(py::init<>())
      .def_readwrite("name", &myf::Sample::name)
      .def_readwrite("isData", &myf::Sample::isData)
      .def_readwrite("xsec", &myf::Sample::xsec)
      .def_readwrite("files", &myf::Sample::files);

  py::class_<myf::Looper>(m, "Looper")
      .def(py::init<const myf::Sample&, const std::string&>(), py::arg("sample"), py::arg("output"))
      .def(
          "add",
          [](myf::Looper& looper, const std::string& type, const std::string& label,
             const std::map<std::string, myf::ParameterSet::Value>& params) {
            looper.add(type, label, myf::ParameterSet(params));
          },
          py::arg("type"), py::arg("label"), py::arg("params"))
      .def("set_config", &myf::Looper::setConfig)
      .def("run", &myf::Looper::run, py::arg("max_events") = -1);

  m.def("available_modules", [] { return myf::ModuleFactory::instance().available(); });
}
