#pragma once

#include <omp.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

using std::ifstream;
using std::istringstream;
using std::getline;
using std::string;
using std::vector;
using std::pair;

class Graph {
private:
    int maxNode = 0;                // ͼ�����ڵ�ID+1
    vector<int> row_ptr;            // CSR��ָ������
    vector<int> col_idx;            // CSR����������
    vector<double> outDegree;       // ÿ���ڵ�ĳ���

public:
    // ��ȡͼ�нڵ�����
    int getMaxNode() const { return maxNode; }

    // ��ȡCSR��ָ������
    const vector<int>& getRowPtr() const { return row_ptr; }

    // ��ȡCSR����������
    const vector<int>& getColIdx() const { return col_idx; }

    // ��ȡ�ڵ��������
    const vector<double>& getOutDegree() const { return outDegree; }

    // ���ļ�����ͼ���ݲ�����CSR��ʽ
    bool load(const string& filename, bool isDirected, int numThreads);
};