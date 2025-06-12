#include "Graph.h"

bool Graph::load(const std::string& filename, bool isDirected,int numThreads) {
    omp_set_num_threads(numThreads);
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        return false;
    }

    std::string line;
    std::vector<std::pair<int, int>> edges;

    // ��ȡ���б�
    while (getline(infile, line)) {
        std::istringstream iss(line);
        int u, v;
        if (iss >> u >> v) {
            edges.push_back({ u, v });
            if (!isDirected) {
                edges.push_back({ v, u }); // ����ͼ��ӷ����
            }
        }
    }

    // �ҵ����ڵ�ID
#pragma omp parallel for reduction(max:maxNode)
    for (size_t i = 0; i < edges.size(); ++i) {
        maxNode = std::max(maxNode, std::max(edges[i].first, edges[i].second));
    }
    maxNode += 1; // �ڵ�������0��maxNode-1��

    // ��ʼ����������
    outDegree.assign(maxNode, 0.0);
    // ͳ�Ƴ���
    for (const auto& e : edges) {
        outDegree[e.first] += 1.0; // Դ�ڵ��������
    }

    // ����CSR��ʽ����Ŀ��ڵ����
    std::vector<std::vector<int>> in_edges(maxNode); // �洢ÿ��Ŀ��ڵ��Դ�ڵ�
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        in_edges[v].push_back(u); // v��Ŀ��ڵ㣬u��Դ�ڵ�
    }

    // ����CSR����������
    row_ptr.resize(maxNode + 1, 0); // ��ָ�루maxNode+1��Ԫ�أ�
    // ����ÿ�еķ���Ԫ������Ŀ��ڵ�v���������
    for (int v = 0; v < maxNode; ++v) {
        row_ptr[v + 1] = row_ptr[v] + in_edges[v].size();
    }

    // ��ʼ������������
    col_idx.resize(row_ptr[maxNode]);

    // ���������
#pragma omp parallel for
    for (int v = 0; v < maxNode; ++v) {
        int start = row_ptr[v];
        int end = row_ptr[v + 1];
        const auto& in_list = in_edges[v];
        for (int idx = start; idx < end; ++idx) {
            col_idx[idx] = in_list[idx - start]; // Դ�ڵ�
        }
    }

    return true;
}