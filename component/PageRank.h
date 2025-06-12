#pragma once

#include <omp.h>  
#include <iostream>  
#include <fstream>  
#include <vector>  
#include <cmath>  
#include <string>
#include "../struct/Graph.h"

class PageRank {
private:
    int numThreads;             // 并行线程数
    const Graph& graph;         // 图对象引用
    std::string outputFile;          // 输出文件路径
    double d = 0.85;            // 阻尼因子  
    double tol = 1e-7;          // 收敛容忍度  
    int maxIter = 1000;         // 最大迭代次数  

    std::vector<double> r;           // PageRank向量
    std::vector<double> r2;          // 迭代中间向量

    double computeNormDiff();   // 计算向量差的范数

public:
    // 构造函数: 传入线程数、图对象和输出文件
    PageRank(int threads, const Graph& g, const std::string& output)
        : numThreads(threads), graph(g), outputFile(output) {
    }

    // 执行PageRank计算
    void run();
};