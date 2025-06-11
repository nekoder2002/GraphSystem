#include "GraphVisitor.h"


// ����BFSʵ��
void GraphVisitor::runBFS(int startNode) {
    reset();

    const auto& row_ptr = graph.getRowPtr();
    const auto& col_idx = graph.getColIdx();
    int maxNode = graph.getMaxNode();

    // ��֤��ʼ�ڵ�
    if (startNode < 0 || startNode >= maxNode) {
        startNode = 0;
    }

    // ʹ���������ʵ�ֲ���BFS
    queue<int> globalQueue;
    globalQueue.push(startNode);
    visited[startNode].store(true, std::memory_order_relaxed);

    double startTime = omp_get_wtime();

    while (!globalQueue.empty()) {
        // ��ǰ��ڵ�����
        vector<int> currentLevel;
        while (!globalQueue.empty()) {
            currentLevel.push_back(globalQueue.front());
            globalQueue.pop();
        }

        // ���д���ǰ������нڵ�
#pragma omp parallel num_threads(numThreads)
        {
            // ÿ���߳����Լ���˽�ж���
            vector<int> localQueue;

#pragma omp for schedule(dynamic, 100)
            for (int i = 0; i < currentLevel.size(); ++i) {
                int u = currentLevel[i];

                // ���ʵ�ǰ�ڵ�������ھ�
                int start = row_ptr[u];
                int end = row_ptr[u + 1];
                for (int j = start; j < end; ++j) {
                    int v = col_idx[j];

                    // ԭ�Ӳ�����鲢��Ƿ���
                    bool expected = false;
                    if (visited[v].compare_exchange_strong(
                        expected, true,
                        std::memory_order_relaxed,
                        std::memory_order_relaxed)) {
                        localQueue.push_back(v);
                    }
                }
            }

            // �����ض��кϲ���ȫ�ֶ���
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

// ����DFSʵ��
void GraphVisitor::runDFS(int startNode) {
    reset();

    int maxNode = graph.getMaxNode();

    // ��֤��ʼ�ڵ�
    if (startNode < 0 || startNode >= maxNode) {
        // ���û��ָ����ʼ�ڵ㣬ʹ�õ�һ��δ���ʽڵ�
        for (int i = 0; i < maxNode; i++) {
            if (!visited[i].load(std::memory_order_relaxed)) {
                startNode = i;
                break;
            }
        }
    }

    double startTime = omp_get_wtime();

    // ���������б�
#pragma omp parallel num_threads(numThreads)
    {
#pragma omp single
        {
            // ����ʼ�ڵ㿪ʼDFS
            parallelDFSUtil(startNode);

            // ��������δ���ʽڵ㣨�����ͨ������
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

// ����DFS�����������ǵݹ飩
void GraphVisitor::parallelDFSUtil(int startNode) {
    stack<int> nodeStack;
    nodeStack.push(startNode);
    visited[startNode].store(true, std::memory_order_relaxed);

    const auto& row_ptr = graph.getRowPtr();
    const auto& col_idx = graph.getColIdx();

    while (!nodeStack.empty()) {
        int u = nodeStack.top();
        nodeStack.pop();

        // ���ʵ�ǰ�ڵ�������ھ�
        int start = row_ptr[u];
        int end = row_ptr[u + 1];
        for (int j = start; j < end; ++j) {
            int v = col_idx[j];

            // ԭ�Ӳ�����鲢��Ƿ���
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