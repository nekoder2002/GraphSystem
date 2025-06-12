#include "Graph.h"

bool Graph::load(const std::string& filename, bool isDirected,int numThreads) {
    omp_set_num_threads(numThreads);
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        return false;
    }

    std::string line;
    std::vector<std::pair<int, int>> edges;

    // 读取所有边
    while (getline(infile, line)) {
        std::istringstream iss(line);
        int u, v;
        if (iss >> u >> v) {
            edges.push_back({ u, v });
            if (!isDirected) {
                edges.push_back({ v, u }); // 无向图添加反向边
            }
        }
    }

    // 找到最大节点ID
#pragma omp parallel for reduction(max:maxNode)
    for (size_t i = 0; i < edges.size(); ++i) {
        maxNode = std::max(maxNode, std::max(edges[i].first, edges[i].second));
    }
    maxNode += 1; // 节点总数（0到maxNode-1）

    // 初始化出度数组
    outDegree.assign(maxNode, 0.0);
    // 统计出度
    for (const auto& e : edges) {
        outDegree[e.first] += 1.0; // 源节点出度增加
    }

    // 构建CSR格式：按目标节点分组
    std::vector<std::vector<int>> in_edges(maxNode); // 存储每个目标节点的源节点
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        in_edges[v].push_back(u); // v是目标节点，u是源节点
    }

    // 构建CSR的三个数组
    row_ptr.resize(maxNode + 1, 0); // 行指针（maxNode+1个元素）
    // 计算每行的非零元素数（目标节点v的入边数）
    for (int v = 0; v < maxNode; ++v) {
        row_ptr[v + 1] = row_ptr[v] + in_edges[v].size();
    }

    // 初始化列索引数组
    col_idx.resize(row_ptr[maxNode]);

    // 填充列索引
#pragma omp parallel for
    for (int v = 0; v < maxNode; ++v) {
        int start = row_ptr[v];
        int end = row_ptr[v + 1];
        const auto& in_list = in_edges[v];
        for (int idx = start; idx < end; ++idx) {
            col_idx[idx] = in_list[idx - start]; // 源节点
        }
    }

    return true;
}