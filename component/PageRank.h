#pragma once

#include <omp.h>  
#include <iostream>  
#include <fstream>  
#include <vector>  
#include <cmath>  
#include <string>
#include "../struct/Graph.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ofstream;
using std::sqrt;

class PageRank {
private:
    int numThreads;             // �����߳���
    const Graph& graph;         // ͼ��������
    string outputFile;          // ����ļ�·��
    double d = 0.85;            // ��������  
    double tol = 1e-7;          // �������̶�  
    int maxIter = 1000;         // ����������  

    vector<double> r;           // PageRank����
    vector<double> r2;          // �����м�����

    double computeNormDiff();   // ����������ķ���

public:
    // ���캯��: �����߳�����ͼ���������ļ�
    PageRank(int threads, const Graph& g, const string& output)
        : numThreads(threads), graph(g), outputFile(output) {
    }

    // ִ��PageRank����
    void run();
};