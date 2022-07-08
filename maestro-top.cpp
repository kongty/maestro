/******************************************************************************
Copyright (c) 2019 Georgia Instititue of Technology
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Author : Hyoukjun Kwon (hyoukjun@gatech.edu)
*******************************************************************************/

#include <boost/program_options.hpp>
#include <iostream>
#include <list>
#include <memory>
#include <vector>

#include "AHW_noc-model.hpp"
#include "API_configuration.hpp"
#include "API_user-interface-v2.hpp"
#include "BASE_base-objects.hpp"
#include "BASE_constants.hpp"
#include "CA_cost-analysis-results.hpp"
#include "DFA_tensor.hpp"
#include "DSE_cost-database.hpp"
#include "DSE_csv_writer.hpp"
#include "DSE_design_point.hpp"
#include "DSE_hardware_modules.hpp"
#include "option.hpp"

// To remove layer
#include "DFA_layer.hpp"

int main(int argc, char** argv) {
    /*
     * Hard coded part; will Fix it
     */

    using namespace maestro;
    Options option;
    bool success = option.parse(argc, argv);

    InitializeBaseObjects(option.message_print_lv);

    std::shared_ptr<std::vector<bool>> noc_multcast =
        std::make_shared<std::vector<bool>>();
    std::shared_ptr<std::vector<int>> noc_latency =
        std::make_shared<std::vector<int>>();
    std::shared_ptr<std::vector<int>> noc_bw =
        std::make_shared<std::vector<int>>();

    // felix

    // auto config = std::make_shared<ConfigurationV2>(
    //     option.dfsl_file_name, option.hw_file_name,
    //     noc_bw, noc_latency,
    //     noc_multcast, option.np, option.num_simd_lanes, option.bw,
    //     option.l1_size, option.l2_size, option.offchip_bw);
    auto config = std::make_shared<ConfigurationV2>(
        "data/mapping/Resnet50_kcp_ws.m", "data/hw/accelerator_1.m", noc_bw,
        noc_latency, noc_multcast, 7, 1, INT_MAX, INT_MAX, INT_MAX, 70000);

    std::shared_ptr<DFA::DirectiveTable> prev_directive_table = nullptr;
    std::shared_ptr<DFA::DirectiveTable> directive_table = nullptr;
    std::shared_ptr<DFA::Layer> layer = nullptr;
    std::shared_ptr<DFA::LayerDimension> curr_dim = nullptr;
    std::shared_ptr<std::vector<std::shared_ptr<DFA::LayerDimension>>> dim_vector = nullptr;
    std::shared_ptr<std::map<std::string, int>> stride_info = nullptr;
    DFA::directive::DirectiveClass curr_directive_class = DFA::directive::DirectiveClass::Invalid;
    std::shared_ptr<DFA::directive::Directive> curr_directive = nullptr;
    std::shared_ptr<std::map<std::string, int>> constant_map = std::make_shared<std::map<std::string, int>>();
    std::string stride_dim;

    // Layer
    layer = std::make_shared<DFA::ConvLayer>("dummy_layer");

    // LayerType
    layer->SetLayerType(LayerType::CONV);

    // Dimensions
    // TODO: In my code, i would use emplace_back
    dim_vector = std::make_shared<std::vector<std::shared_ptr<DFA::LayerDimension>>>();
    curr_dim = std::make_shared<DFA::LayerDimension> ("K", 64, 1, 1);
    dim_vector->push_back(curr_dim);
    curr_dim = std::make_shared<DFA::LayerDimension> ("C", 3, 1, 1);
    dim_vector->push_back(curr_dim);
    curr_dim = std::make_shared<DFA::LayerDimension> ("R", 7, 1, 1);
    dim_vector->push_back(curr_dim);
    curr_dim = std::make_shared<DFA::LayerDimension> ("S", 7, 1, 1);
    dim_vector->push_back(curr_dim);
    curr_dim = std::make_shared<DFA::LayerDimension> ("Y", 224, 2, 1);
    dim_vector->push_back(curr_dim);
    curr_dim = std::make_shared<DFA::LayerDimension> ("X", 224, 2, 1);
    dim_vector->push_back(curr_dim);
    layer->SetDimensions(dim_vector);

    // Dataflow
    directive_table = std::make_shared<DFA::DirectiveTable>();
    curr_directive = std::make_shared<DFA::directive::SpatialMap> (1, 1, "K");
    directive_table->AddDirective(curr_directive);
    curr_directive = std::make_shared<DFA::directive::TemporalMap> (64, 64, "C");
    directive_table->AddDirective(curr_directive);
    curr_directive = std::make_shared<DFA::directive::TemporalMap> (7, 7, "R");
    directive_table->AddDirective(curr_directive);
    curr_directive = std::make_shared<DFA::directive::TemporalMap> (7, 7, "S");
    directive_table->AddDirective(curr_directive);
    curr_directive = std::make_shared<DFA::directive::TemporalMap> (7, 1, "Y");
    directive_table->AddDirective(curr_directive);
    curr_directive = std::make_shared<DFA::directive::TemporalMap> (7, 1, "X");
    directive_table->AddDirective(curr_directive);
    curr_directive = std::make_shared<DFA::directive::Cluster>(64, DFA::directive::ClusterType::Physical);
    directive_table->AddDirective(curr_directive);
    curr_directive = std::make_shared<DFA::directive::SpatialMap> (1, 1, "C");
    directive_table->AddDirective(curr_directive);
    curr_directive = std::make_shared<DFA::directive::TemporalMap> (7, 1, "Y");
    directive_table->AddDirective(curr_directive);
    curr_directive = std::make_shared<DFA::directive::TemporalMap> (7, 1, "X");
    directive_table->AddDirective(curr_directive);
    curr_directive = std::make_shared<DFA::directive::TemporalMap> (7, 7, "R");
    directive_table->AddDirective(curr_directive);
    curr_directive = std::make_shared<DFA::directive::TemporalMap> (7, 7, "S");
    directive_table->AddDirective(curr_directive);
    layer->SetDataflow(directive_table);
    

    auto api = std::make_shared<APIV2>(config, layer);
    // auto api = std::make_shared<APIV2>(config);

    auto res = api->AnalyzeNeuralNetwork(true, true, true);

    return 0;
}
