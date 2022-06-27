#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "cost-analysis/CA_cost-analysis-results.hpp"
#include "dataflow-analysis/DFA_layer.hpp"

namespace py = pybind11;
using namespace maestro;

void init_CA_cost_analysis_results(py::module &m)
{
    py::class_<CA::CostAnalyisResults>(m, "CostAnalyisResult")
        .def(py::init<>())
        .def(py::init<LayerType, int>())
        .def_property("num_computation", &CA::CostAnalyisResults::GetNumComputations, &CA::CostAnalyisResults::UpdateNumComputations);
}
