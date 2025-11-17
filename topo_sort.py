def find_topo(node_list: List[Value]) -> List[Value]:
    visited = set()
    topo_order = []
    for node in node_list:  #node_list 的访问顺序会影响最后的结果
        topo_sort_for_single_node(node, visited, topo_order)
    return topo_order

def topo_sort_for_single_node(node, visited, topo_order):
    if node in visited:
        return
    visited.add(node)
    for input_node in node.inputs:
        topo_sort_dfs(input_node, visited, topo_order)
    topo_order.append(node) #这个放到最后是最关键的

#最后排出来可能顺序有多种，但都是拓扑排序


##再附一个梯度反向传播：

def backward(output_tensor, out_grad):
    #建立字典
    node_to_output_grad_list = defaultdict(list) #Dict[Tensor, List[Tensor]]
                                                 #需要import collections
                                                 #defaultdict 比起一般dict可以自动创建不存在的字典键，避免一次if判断
    node_to_output_grad_list[output_tensor] = [out_grad]#初始驱动
    reverse_topo_order = list(reversed(find_topo([output_tensor])))#reversed返回的是迭代器
    for node in reverse_topo_order:               #一次就做完了，不需要递归
        node.grad = sum_node_list(node_to_output_grad_list[node])
        for input_node in node.inputs:
            v_k_to_i = node.gradient(node.grad, input_node)
            node_to_output_grad_list[input_node].append(v_k_to_i)
