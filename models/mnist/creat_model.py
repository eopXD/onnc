import numpy as np
import onnx
from onnx import numpy_helper
from onnx import helper
from onnx import AttributeProto, TensorProto, GraphProto



# Preprocessing: create a Numpy array
numpy_array = np.load('./quantized_weight/quant_Parameter5.npy')
# Convert the Numpy array to a TensorProto
tensor = numpy_helper.from_array(numpy_array)


Parameter6 = np.load('./quantized_weight/quant_Parameter6.npy')
Parameter6_tensor = numpy_helper.from_array(Parameter6)



# Create one input (ValueInfoProto)
Input3 = helper.make_tensor_value_info('Input3', TensorProto.FLOAT, [1,1,28,28])

# Create one output (ValueInfoProto)
Parameter5 = helper.make_tensor_value_info('Parameter5', TensorProto.FLOAT, [8,1,5,5])
#Y = helper.make_tensor_value_info('Y', TensorProto.FLOAT, [1,8,28,28])

Convolution28_Output_0 = helper.make_tensor_value_info('Convolution28_Output_0', TensorProto.FLOAT, [1,8,28,28])

# Create one output (ValueInfoProto)
Parameter6 = helper.make_tensor_value_info('Parameter6', TensorProto.FLOAT, [8,1,1])
addY = helper.make_tensor_value_info('addY', TensorProto.FLOAT, [1,8,28,28])



# Create a node (NodeProto)
node_def = helper.make_node(
    'Conv', # node name
    ['Input3','Parameter5'], # inputs
    ['Convolution28_Output_0'], # outputs
    'Convolution28', # node name
    
    #mode='kernel_shape', # attributes
    kernel_shape = [5,5],
    auto_pad  ="SAME_UPPER",
    strides=[1,1],
    group=1,
    dilations = [1,1],
    
)
Plus30_node_def = helper.make_node(
    'Add', # node name
    ['Convolution28_Output_0','Parameter6'], # inputs
    ['addY'], # outputs
    
    #mode='kernel_shape', # attributes
)
# Create the graph (GraphProto)
graph_def = helper.make_graph(
    [node_def],
    'test-model',
    [Input3,Parameter5],
    [Convolution28_Output_0],
    #[Plus30_node_def],
    #'test'
    #[Convolution28_Output_0,Parameter6],
    #[addY],
)

add_graph_def = helper.make_graph(
    [Plus30_node_def],
    'test-model',
    [Convolution28_Output_0,Parameter6],
    [addY],
)


graph_def.initializer.extend([tensor])
#graph_def.initializer[0].name = 'no-exist'

graph_def.initializer[0].name = 'Parameter5'

#graph_def.initializer.extend([Parameter6_tensor])
#graph_def.initializer[0].name = 'Parameter6'

# Create the model (ModelProto)
model_def = helper.make_model(graph_def, producer_name='onnx-example')

#print('The model is:\n{}'.format(model_def))
onnx.checker.check_model(model_def)
#print(model_def)
print('The model is checked!')
#print(graph_def.initializer)
onnx.save(model_def, './quantized.onnx')