#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "base/BASE_base-objects.hpp"

namespace py = pybind11;

void init_CA_cost_analysis_results(py::module &m);
void init_API_configuration(py::module &m);
void init_API(py::module &m);
void init_layer(py::module &m);
void init_layer_dimension(py::module &m);
void init_directive(py::module &m);
void init_directive_table(py::module &m);

PYBIND11_MODULE(maestro, m) {
    m.doc() = R"pbdoc(
        .. currentmodule:: maestro
    )pbdoc";
    maestro::InitializeBaseObjects();

    init_CA_cost_analysis_results(m);
    init_API_configuration(m);
    init_API(m);
    init_layer(m);
    init_layer_dimension(m);
    init_directive(m);
    init_directive_table(m);
}
