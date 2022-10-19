import maestro
from maestro import SpatialMap, TemporalMap, Cluster, LayerDimension

subaccelerator_0 = maestro.SubAccelerator(256, 1, 50, 100, 50, [1000, 1000], [1, 1], [True, True])

dimensions = [LayerDimension('K', 64, 1, 1),
                LayerDimension('X', 224, 2, 1),
                LayerDimension('R', 7, 1, 1),
                LayerDimension('C', 3, 1, 1),
                LayerDimension('Y', 224, 2, 1),
                LayerDimension('S', 7, 1, 1),
                ]


maestro.LayerType.CONV
# layer
layer = maestro.ConvLayer("conv_1_1", dimensions)

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

layer.set_dataflow(directives)
cost = maestro.run(subaccelerator_0, layer)
assert int(cost['runtime']) == 9504801
assert int(cost['computation']) == 111776448
