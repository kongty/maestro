#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void init_CA_cost_analysis_results(py::module &m);
void init_API_configuration(py::module &m);
void init_API(py::module &m);

PYBIND11_MODULE(maestro, m)
{
    m.doc() = R"pbdoc(
        .. currentmodule:: maestro
    )pbdoc";
    init_CA_cost_analysis_results(m);
    init_API_configuration(m);
    init_API(m);
}
