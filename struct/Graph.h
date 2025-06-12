#pragma once

#include <omp.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

class Graph {
private:
    int maxNode = 0;                // 图中最大节点ID+1
    std::vector<int> row_ptr;            // CSR行指针数组
    std::vector<int> col_idx;            // CSR列索引数组
    std::vector<double> outDegree;       // 每个节点的出度

public:
    // 获取图中节点总数
    int getMaxNode() const { return maxNode; }

    // 获取CSR行指针数组
    const std::vector<int>& getRowPtr() const { return row_ptr; }

    // 获取CSR列索引数组
    const std::vector<int>& getColIdx() const { return col_idx; }

    // 获取节点出度数组
    const std::vector<double>& getOutDegree() const { return outDegree; }

    // 从文件加载图数据并构建CSR格式
    bool load(const std::string& filename, bool isDirected, int numThreads);
};