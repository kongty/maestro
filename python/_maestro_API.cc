#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "user-api/API_configuration.hpp"
#include "user-api/API_user-interface-v2.hpp"

namespace py = pybind11;
using namespace maestro;
const double DEFAULT_L1_ENERGY_MULTIPLIER = 1.68;
const double DEFAULT_L2_ENERGY_MULTIPLIER = 18.61;

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
                      int num_pes, int simd_width, int l1_sram_byte_size, int l2_sram_byte_size, int offchip_bw)
        : ConfigurationV2("", "", std::make_shared<std::vector<int>>(noc_bw),
                          std::make_shared<std::vector<int>>(noc_latency),
                          std::make_shared<std::vector<bool>>(noc_multcast), num_pes, simd_width, 0, l1_sram_byte_size,
                          l2_sram_byte_size, offchip_bw) {}
};

void init_API_configuration(py::module &m) {
    py::class_<PyConfigurationV2, std::shared_ptr<PyConfigurationV2>>(m, "Configuration")
        .def(py::init<std::string, std::string>())
        .def(py::init<std::vector<int>, std::vector<int>, std::vector<bool>, int, int, int, int, int>());
}

class PyAPIV2 : public APIV2 {
   public:
    PyAPIV2(PyConfigurationV2 &config) : APIV2(std::make_shared<ConfigurationV2>(config)) {}
    PyAPIV2(PyConfigurationV2 &config, DFA::Layer &layer)
        : APIV2(std::make_shared<ConfigurationV2>(config), std::make_shared<DFA::Layer>(layer)) {}
};

void init_API(py::module &m) {
    py::class_<PyAPIV2, std::shared_ptr<PyAPIV2>>(m, "API")
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

struct SubAccelerator {
   public:
    SubAccelerator(int num_pes, int simd_width, int l1_size, int l2_size, int offchip_bw, std::vector<int> noc_bw,
                   std::vector<int> noc_latency, std::vector<bool> noc_multcast)
        : num_pes(num_pes),
          simd_width(simd_width),
          l1_size(l1_size),
          l2_size(l2_size),
          offchip_bw(offchip_bw),
          noc_bw(noc_bw),
          noc_latency(noc_latency),
          noc_multcast(noc_multcast) {}

    int num_pes;
    int simd_width;
    int l1_size;
    int l2_size;
    int offchip_bw;
    std::vector<int> noc_bw;
    std::vector<int> noc_latency;
    std::vector<bool> noc_multcast;
};

void init_subaccelerator(py::module &m) {
    py::class_<SubAccelerator, std::shared_ptr<SubAccelerator>>(m, "SubAccelerator")
        .def(py::init<int, int, int, int, int, std::vector<int>, std::vector<int>, std::vector<bool>>());
}

std::map<std::string, long double> run(SubAccelerator &hardware, DFA::Layer &layer,
                                    std::vector<double> energy_multiplier) {
    PyConfigurationV2 configuration =
        PyConfigurationV2(hardware.noc_bw, hardware.noc_latency, hardware.noc_multcast, hardware.num_pes,
                          hardware.simd_width, hardware.l1_size, hardware.l2_size, hardware.offchip_bw);
    PyAPIV2 api = PyAPIV2(configuration, layer);
    std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::shared_ptr<CA::CostAnalyisResults>>>>> nn_result =
        api.AnalyzeNeuralNetwork();

    long computation;
    long runtime;
    long double throughput;
    long double abs_throughput;
    long double l2_write_energy;
    long double l2_read_energy;
    long double l1_write_energy;
    long double l1_read_energy;

    long double peak_l1_bw;
    long double avg_l1_bw;

    long double peak_l2_bw;
    long double avg_l2_bw;
    long num_utilized_pes;

    std::map<std::string, long double> cost_map;
    cost_map = api.CustomOutputResults(nn_result).front();
    cost_map.emplace("l1_rd_energy", cost_map["l1_rd_count"] * energy_multiplier[0]);
    cost_map.emplace("l1_wr_energy", cost_map["l1_wr_count"] * energy_multiplier[0]);
    cost_map.emplace("l2_rd_energy", cost_map["l1_rd_count"] * energy_multiplier[1]);
    cost_map.emplace("l2_wr_energy", cost_map["l2_wr_count"] * energy_multiplier[1]);

    return cost_map;
}

std::map<std::string, long double> run(SubAccelerator &hardware, DFA::Layer &layer) {
    return run(hardware, layer, {DEFAULT_L1_ENERGY_MULTIPLIER, DEFAULT_L2_ENERGY_MULTIPLIER});
}

void init_run(py::module &m) {
    m.def("run", py::overload_cast<SubAccelerator &, DFA::Layer &, std::vector<double>>(&run),
          "Run maestro with custom energy table");
    m.def("run", py::overload_cast<SubAccelerator &, DFA::Layer &>(&run), "Run maestro with default energy table");
}