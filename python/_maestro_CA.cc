#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "cost-analysis/CA_cost-analysis-results.hpp"
#include "dataflow-analysis/DFA_layer.hpp"

namespace py = pybind11;
using namespace maestro;

void init_CA_cost_analysis_results(py::module &m) {
    py::class_<CA::CostAnalyisResults, std::shared_ptr<CA::CostAnalyisResults>>(m, "CostAnalysisResult")
        .def(py::init<>())
        .def(py::init<LayerType, int>())
        .def_property("num_computation", &CA::CostAnalyisResults::GetNumComputations,
                      &CA::CostAnalyisResults::UpdateNumComputations)
        .def_property(
            "runtime",
            [](CA::CostAnalyisResults &result) {
                auto res = result.GetRuntime(CA::EstimationType::Exact);
                return res;
            },
            [](CA::CostAnalyisResults &result, long new_runtime) {
                result.UpdateRuntime(new_runtime, CA::EstimationType::Exact);
            })
        .def("get_buffer_access_count", &CA::CostAnalyisResults::GetBufferAccessCount)
        .def("get_buffer_size_req", &CA::CostAnalyisResults::GetBufferSizeReq);
}
