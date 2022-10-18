#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "dataflow-analysis/DFA_layer.hpp"

namespace py = pybind11;
using namespace maestro;

void init_layer_type(py::module &m) {
    py::enum_<LayerType>(m, "LayerType")
        .value("CONV", LayerType::CONV)
        .value("DSCONV", LayerType::DSCONV)
        .value("POOL", LayerType::POOL)
        .value("TRCONV", LayerType::TRCONV)
        .value("NGCONV", LayerType::NGCONV)
        .value("LSTM", LayerType::LSTM)
        .value("GEMM", LayerType::GEMM);
}
class PyLayer : public DFA::Layer {
   public:
    PyLayer(std::string name, LayerType layer_type, const std::vector<std::shared_ptr<DFA::LayerDimension>> &dimension)
        : DFA::Layer(name, layer_type, std::make_shared<std::vector<std::shared_ptr<DFA::LayerDimension>>>(dimension)) {
    }
};

void init_layer(py::module &m) {
    py::class_<DFA::Layer, std::shared_ptr<DFA::Layer>>(m, "_Layer");
    py::class_<PyLayer, std::shared_ptr<PyLayer>, DFA::Layer>(m, "Layer")
        .def(py::init<std::string, LayerType, const std::vector<std::shared_ptr<DFA::LayerDimension>> &>())
        .def("set_dataflow", &DFA::Layer::SetDataflow);
}

void init_layer_dimension(py::module &m) {
    py::class_<DFA::LayerDimension, std::shared_ptr<DFA::LayerDimension>>(m, "LayerDimension")
        .def(py::init<std::string, int, int, int>());
}

// Directive Classes
void init_directive(py::module &m) {
    // Directive
    py::class_<DFA::directive::Directive, std::shared_ptr<DFA::directive::Directive>>(m, "Directive");
    // Map
    py::class_<DFA::directive::Map, std::shared_ptr<DFA::directive::Map>, DFA::directive::Directive>(m, "Map");
    // Temporal Map
    py::class_<DFA::directive::TemporalMap, std::shared_ptr<DFA::directive::TemporalMap>,
               //    DFA::directive::Map,
               DFA::directive::Directive>(m, "TemporalMap")
        .def(py::init<int, int, std::string>());
    // Spatial Map
    py::class_<DFA::directive::SpatialMap, std::shared_ptr<DFA::directive::SpatialMap>,
               //    DFA::directive::Map,
               DFA::directive::Directive>(m, "SpatialMap")
        .def(py::init<int, int, std::string>());
    // ClusterType
    py::enum_<DFA::directive::ClusterType>(m, "ClusterType")
        .value("Physical", DFA::directive::ClusterType::Physical)
        .value("Logical", DFA::directive::ClusterType::Logical)
        .value("Invalid", DFA::directive::ClusterType::Invalid);
    // Cluster
    py::class_<DFA::directive::Cluster, std::shared_ptr<DFA::directive::Cluster>, DFA::directive::Directive>(m,
                                                                                                             "Cluster")
        .def(py::init<int, DFA::directive::ClusterType>());
}

class PyDirectiveTable : public DFA::DirectiveTable {
   public:
    PyDirectiveTable() : DFA::DirectiveTable() {}
    PyDirectiveTable(const std::vector<std::shared_ptr<DFA::directive::Directive>> &directives)
        : DFA::DirectiveTable(std::make_shared<std::vector<std::shared_ptr<DFA::directive::Directive>>>(directives)) {}
};

// DirectiveTable class
void init_directive_table(py::module &m) {
    py::class_<DFA::DirectiveTable, std::shared_ptr<DFA::DirectiveTable>>(m, "_DirectiveTable");
    py::class_<PyDirectiveTable, std::shared_ptr<PyDirectiveTable>, DFA::DirectiveTable>(m, "DirectiveTable")
        .def(py::init<>())
        .def(py::init<const std::vector<std::shared_ptr<DFA::directive::Directive>> &>())
        .def("add_directive", &DFA::DirectiveTable::AddDirective);
}
