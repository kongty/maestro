#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../maestro/cost-model/include/cost-analysis/CA_cost-analysis-results.hpp"

namespace py = pybind11;
using namespace maestro::CA;

void init_CA_cost_analysis_results(py::module &m) {
    py::class_<CostAnalysisResult>(m, "CostAnalysisResult")
        .def(py::init<>())
        .def(py::init<LayerType, int>())
        .def("GetRuntime", &CostAnalysisResult::GetRuntime);
}
