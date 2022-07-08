import maestro
from maestro import SpatialMap, TemporalMap, Cluster, LayerDimension
import pytest


@pytest.mark.parametrize("hw_file,mapping_file", [('data/hw/accelerator_1.m', 'data/mapping/Resnet50_kcp_ws.m')])
def test_maestro_file(hw_file, mapping_file):
    config = maestro.Configuration(hw_file, mapping_file)
    analyzer = maestro.Analyzer(config)
    res = analyzer.analyze_neural_network()
    assert res[0][0].num_computation == 147


def test_maestro_object():
    # configuration
    config = maestro.Configuration([1000, 1000], [1, 1], [True, True], 256, 1, 50, 100, 3000, 50)

    # dimensions
    dimensions = [LayerDimension('K', 64, 1, 1),
                  LayerDimension('C', 3, 1, 1),
                  LayerDimension('R', 7, 1, 1),
                  LayerDimension('S', 7, 1, 1),
                  LayerDimension('Y', 224, 2, 1),
                  LayerDimension('X', 224, 2, 1),
                  ]

    # directives
    directives = maestro.DirectiveTable([SpatialMap(1, 1, "K"),
                                         TemporalMap(64, 64, "C"),
                                         TemporalMap(7, 7, "R"),
                                         TemporalMap(7, 7, "S"),
                                         TemporalMap(7, 1, "Y"),
                                         TemporalMap(7, 1, "X"),
                                         Cluster(64, maestro.ClusterType.Physical),
                                         SpatialMap(1, 1, "C"),
                                         TemporalMap(7, 1, "Y"),
                                         TemporalMap(7, 1, "X"),
                                         TemporalMap(7, 7, "R"),
                                         TemporalMap(7, 7, "S")])

    # layer
    layer = maestro.ConvLayer("conv_1_1", dimensions, directives)

    analyzer = maestro.Analyzer(config, layer)
    res = analyzer.analyze_neural_network()
    assert res[0][0].num_computation == 147
