#include "GraphVisitor.h"


// 并行BFS实现
void GraphVisitor::runBFS(int startNode) {
    reset();

    const auto& row_ptr = graph.getRowPtr();
    const auto& col_idx = graph.getColIdx();
    int maxNode = graph.getMaxNode();

    // 验证起始节点
    if (startNode < 0 || startNode >= maxNode) {
        startNode = 0;
    }

    // 使用两层队列实现并行BFS
    queue<int> globalQueue;
    globalQueue.push(startNode);
    visited[startNode].store(true, std::memory_order_relaxed);

    double startTime = omp_get_wtime();

    while (!globalQueue.empty()) {
        // 当前层节点容器
        vector<int> currentLevel;
        while (!globalQueue.empty()) {
            currentLevel.push_back(globalQueue.front());
            globalQueue.pop();
        }

        // 并行处理当前层的所有节点
#pragma omp parallel num_threads(numThreads)
        {
            // 每个线程有自己的私有队列
            vector<int> localQueue;

#pragma omp for schedule(dynamic, 100)
            for (int i = 0; i < currentLevel.size(); ++i) {
                int u = currentLevel[i];

                // 访问当前节点的所有邻居
                int start = row_ptr[u];
                int end = row_ptr[u + 1];
                for (int j = start; j < end; ++j) {
                    int v = col_idx[j];

                    // 原子操作检查并标记访问
                    bool expected = false;
                    if (visited[v].compare_exchange_strong(
                        expected, true,
                        std::memory_order_relaxed,
                        std::memory_order_relaxed)) {
                        localQueue.push_back(v);
                    }
                }
            }

            // 将本地队列合并到全局队列
#pragma omp critical
            {
                for (int node : localQueue) {
                    globalQueue.push(node);
                }
            }
        }
    }

    double endTime = omp_get_wtime();
    cout << "Parallel BFS completed in " << (endTime - startTime) << " seconds" << endl;
}

// 并行DFS实现
void GraphVisitor::runDFS(int startNode) {
    reset();

    int maxNode = graph.getMaxNode();

    // 验证起始节点
    if (startNode < 0 || startNode >= maxNode) {
        // 如果没有指定起始节点，使用第一个未访问节点
        for (int i = 0; i < maxNode; i++) {
            if (!visited[i].load(std::memory_order_relaxed)) {
                startNode = i;
                break;
            }
        }
    }

    double startTime = omp_get_wtime();

    // 创建任务列表
#pragma omp parallel num_threads(numThreads)
    {
#pragma omp single
        {
            // 从起始节点开始DFS
            parallelDFSUtil(startNode);

            // 处理其他未访问节点（多个连通分量）
            for (int i = 0; i < maxNode; i++) {
                if (!visited[i].load(std::memory_order_relaxed)) {
#pragma omp task
                    parallelDFSUtil(i);
                }
            }
        }
    }

    double endTime = omp_get_wtime();
    cout << "Parallel DFS completed in " << (endTime - startTime) << " seconds" << endl;
}

// 并行DFS辅助函数（非递归）
void GraphVisitor::parallelDFSUtil(int startNode) {
    stack<int> nodeStack;
    nodeStack.push(startNode);
    visited[startNode].store(true, std::memory_order_relaxed);

    const auto& row_ptr = graph.getRowPtr();
    const auto& col_idx = graph.getColIdx();

    while (!nodeStack.empty()) {
        int u = nodeStack.top();
        nodeStack.pop();

        // 访问当前节点的所有邻居
        int start = row_ptr[u];
        int end = row_ptr[u + 1];
        for (int j = start; j < end; ++j) {
            int v = col_idx[j];

            // 原子操作检查并标记访问
            bool expected = false;
            if (visited[v].compare_exchange_strong(
                expected, true,
                std::memory_order_relaxed,
                std::memory_order_relaxed)) {
                nodeStack.push(v);
            }
        }
    }
}