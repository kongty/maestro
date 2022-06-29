import maestro
import pytest


@pytest.mark.parametrize("hw_file,mapping_file", [('data/hw/accelerator_1.m', 'data/mapping/Resnet50_kcp_ws.m')])
def test_maestro(hw_file, mapping_file):
    config = maestro.Configuration(mapping_file, hw_file, [], [], [], 256, 1, 50, 50, 300, 300)
    api = maestro.maestro(config)
    res = api.AnalyzeNeuralNetwork()



#option 1
# hw file/ mapping file

# option 2
# hw options: np, num_simd, bw, l1_size, l2_size, offchip