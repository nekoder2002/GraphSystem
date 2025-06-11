#pragma once

#include "../struct/Graph.h"
#include <vector>
#include <queue>
#include <stack>
#include <atomic>
#include <memory>
#include <omp.h>
#include <iostream>

using std::queue;
using std::stack;
using std::cout;
using std::endl;
using std::vector;
using std::unique_ptr;

class GraphVisitor {
private:
    const Graph& graph;           // 图对象引用
    std::unique_ptr<std::atomic<bool>[]> visited;
    int numNodes;  // 保存节点数量
    int numThreads;               // 并行线程数

    // 并行DFS的辅助函数
    void parallelDFSUtil(int startNode);

public:
    // 构造函数：传入图对象和线程数
    GraphVisitor(const Graph& g, int threads)
        : graph(g), numThreads(threads), numNodes(0) {
        int maxNode = graph.getMaxNode();
        numNodes = maxNode;
        visited = std::make_unique<std::atomic<bool>[]>(maxNode);
        reset();
    }

    // 执行并行BFS
    void runBFS(int startNode);

    // 执行并行DFS
    void runDFS(int startNode);

    // 获取访问标记数组
    std::atomic<bool>* getVisited() const {
        return visited.get();
    }

    int getVisitedSize() const {
        return numNodes;
    }

    // 重置访问标记
    void reset() {
#pragma omp parallel for
        for (int i = 0; i < numNodes; ++i) {
            visited[i].store(false, std::memory_order_relaxed);
        }
    }
};