#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "user-api/API_configuration.hpp"
#include "user-api/API_user-interface-v2.hpp"

namespace py = pybind11;
using namespace maestro;

// pybind does not support bindings for function argument that is
// std::shared_ptr<std::vector<T>> This is a wrapper class. NOTE: it is a bad
// practice to have std::shared_ptr<std::vector<T>> as function inputs or
// return.
class PyConfigurationV2 : public ConfigurationV2 {
   public:
    PyConfigurationV2(std::string hw_file_name, std::string dfsl_file_name)
        : ConfigurationV2(dfsl_file_name, hw_file_name, std::make_shared<std::vector<int>>(std::vector<int>()),
                          std::make_shared<std::vector<int>>(std::vector<int>()),
                          std::make_shared<std::vector<bool>>(std::vector<bool>()), 7, 1, INT_MAX, INT_MAX, INT_MAX,
                          70000) {}
    PyConfigurationV2(std::vector<int> noc_bw, std::vector<int> noc_latency, std::vector<bool> noc_multcast,
                      int num_pes, int simd_width, int top_noc_bw, int l1_sram_byte_size, int l2_sram_byte_size,
                      int offchip_bw)
        : ConfigurationV2("", "", std::make_shared<std::vector<int>>(noc_bw),
                          std::make_shared<std::vector<int>>(noc_latency),
                          std::make_shared<std::vector<bool>>(noc_multcast), num_pes, simd_width, top_noc_bw,
                          l1_sram_byte_size, l2_sram_byte_size, offchip_bw) {}
};

void init_API_configuration(py::module &m) {
    py::class_<PyConfigurationV2, std::shared_ptr<PyConfigurationV2>>(m, "Configuration")
        .def(py::init<std::string, std::string>())
        .def(py::init<std::vector<int>, std::vector<int>, std::vector<bool>, int, int, int, int, int, int>());
}

class PyAPIV2 : public APIV2 {
   public:
    PyAPIV2(PyConfigurationV2 &config) : APIV2(std::make_shared<ConfigurationV2>(config)) {}
    PyAPIV2(PyConfigurationV2 &config, DFA::Layer &layer)
        : APIV2(std::make_shared<ConfigurationV2>(config), std::make_shared<DFA::Layer>(layer)) {}
};

void init_API(py::module &m) {
    py::class_<PyAPIV2, std::shared_ptr<PyAPIV2>>(m, "Analyzer")
        .def(py::init<PyConfigurationV2 &>())
        .def(py::init<PyConfigurationV2 &, DFA::Layer &>())
        .def("analyze_neural_network", [](PyAPIV2 &api) {
            auto res = api.AnalyzeNeuralNetwork();
            std::vector<std::vector<std::shared_ptr<CA::CostAnalyisResults>>> result;
            for (auto const &dim : *res) {
                result.emplace_back(*dim);
            }
            return result;
        });
}
