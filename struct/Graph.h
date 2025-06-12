#pragma once

#include <omp.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

class Graph {
private:
    int maxNode = 0;                // ͼ�����ڵ�ID+1
    std::vector<int> row_ptr;            // CSR��ָ������
    std::vector<int> col_idx;            // CSR����������
    std::vector<double> outDegree;       // ÿ���ڵ�ĳ���

public:
    // ��ȡͼ�нڵ�����
    int getMaxNode() const { return maxNode; }

    // ��ȡCSR��ָ������
    const std::vector<int>& getRowPtr() const { return row_ptr; }

    // ��ȡCSR����������
    const std::vector<int>& getColIdx() const { return col_idx; }

    // ��ȡ�ڵ��������
    const std::vector<double>& getOutDegree() const { return outDegree; }

    // ���ļ�����ͼ���ݲ�����CSR��ʽ
    bool load(const std::string& filename, bool isDirected, int numThreads);
};