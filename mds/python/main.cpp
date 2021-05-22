#include <pybind11/pybind11.h>
#include "mds/base/except.h"
#include "mds/strlist/reader.h"
#include "mds/strlist/dumper.h"
namespace py = pybind11;

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace mds::python {

PYBIND11_MODULE(mmapds, m) {
  m.doc() = R"pbdoc(
        Python mem-map data structures.
    )pbdoc";

  py::register_exception<base::BaseException>(m, "Error", PyExc_RuntimeError);
  auto strlistModule = m.def_submodule("strlist", "Str/Bytes List ");
  py::class_<strlist::Dumper>(strlistModule, "Dumper").def(py::init<const char *>())
      .def("append", [](strlist::Dumper &self, const py::bytes &bytes) {
        auto sv = bytes.cast<std::string_view>();
        self.Append(sv);
      }).def("complete", &strlist::Dumper::Complete);
  py::class_<strlist::Reader>(strlistModule, "Reader").def(py::init<const char *>())
      .def("__len__", &strlist::Reader::size).def("__getitem__",
                                                  [](strlist::Reader &self, size_t i) -> py::bytes {
                                                    auto sv = self[i];
                                                    return py::bytes(sv.data(), sv.size());
                                                  });

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}

}