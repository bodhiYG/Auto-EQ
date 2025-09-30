#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "eq3band.hpp"

namespace py = pybind11;
using namespace autoeq;

PYBIND11_MODULE(autoeq_py, m) {
    m.doc() = "AutoEQ DSP bindings";

    py::class_<Eq3Band>(m, "Eq3Band")
        .def(py::init<int,int>())
        .def("set_gains_db", &Eq3Band::setGainsDb)
        .def("process_interleaved", [](Eq3Band& self, py::array_t<float, py::array::c_style | py::array::forcecast> frames) {
            if (frames.ndim() != 2) throw std::runtime_error("Expected 2D array [frames, channels]");
            auto buf = frames.request();
            const int channels = static_cast<int>(buf.shape[1]);
            auto ptr = static_cast<float*>(buf.ptr);
            self.processInterleaved(ptr, static_cast<size_t>(buf.shape[0]));
        });
}


