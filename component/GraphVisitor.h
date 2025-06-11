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
    const Graph& graph;           // ͼ��������
    std::unique_ptr<std::atomic<bool>[]> visited;
    int numNodes;  // ����ڵ�����
    int numThreads;               // �����߳���

    // ����DFS�ĸ�������
    void parallelDFSUtil(int startNode);

public:
    // ���캯��������ͼ������߳���
    GraphVisitor(const Graph& g, int threads)
        : graph(g), numThreads(threads), numNodes(0) {
        int maxNode = graph.getMaxNode();
        numNodes = maxNode;
        visited = std::make_unique<std::atomic<bool>[]>(maxNode);
        reset();
    }

    // ִ�в���BFS
    void runBFS(int startNode);

    // ִ�в���DFS
    void runDFS(int startNode);

    // ��ȡ���ʱ������
    std::atomic<bool>* getVisited() const {
        return visited.get();
    }

    int getVisitedSize() const {
        return numNodes;
    }

    // ���÷��ʱ��
    void reset() {
#pragma omp parallel for
        for (int i = 0; i < numNodes; ++i) {
            visited[i].store(false, std::memory_order_relaxed);
        }
    }
};