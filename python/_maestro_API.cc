#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "user-api/API_configuration.hpp"
#include "user-api/API_user-interface-v2.hpp"

namespace py = pybind11;
using namespace maestro;

class PyConfigurationV2 : public ConfigurationV2 {
   public:
    PyConfigurationV2(std::string dfsl_file_name, std::string hw_file_name,
                      std::vector<int> noc_bw, std::vector<int> noc_latency,
                      std::vector<bool> noc_multcast, int num_pes,
                      int simd_width, int top_noc_bw, int l1_sram_byte_size,
                      int l2_sram_byte_size, int offchip_bw) {
        dfsl_file_name_ = dfsl_file_name;
        hw_file_name_ = hw_file_name;
        simd_width_ = simd_width;
        l1_size_ = l1_sram_byte_size;
        l2_size_ = l2_sram_byte_size;
        noc_bw_ = std::make_shared<std::vector<int>>(std::move(noc_bw));
        noc_latency_ =
            std::make_shared<std::vector<int>>(std::move(noc_latency));
        noc_multcast_ =
            std::make_shared<std::vector<bool>>(std::move(noc_multcast));
    }
};

void init_API_configuration(py::module &m) {
    py::class_<PyConfigurationV2>(m, "Configuration")
        .def(py::init<std::string, std::string, std::vector<int>,
                      std::vector<int>, std::vector<bool>, int, int, int, int,
                      int, int>());
}

void init_API(py::module &m) {
    py::class_<APIV2>(m, "maestro")
        .def(py::init<std::shared_ptr<ConfigurationV2>>())
    .def("AnalyzeNeuralNetwork", [](APIV2 &api) {
        auto res = api.AnalyzeNeuralNetwork();
        std::vector<std::vector<std::shared_ptr<CA::CostAnalyisResults>>> result;
        for (auto const &dim1: *res) {
            result.emplace_back(*dim1);
        }

        return result;
});
}