#pragma once

#include <omp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class PageRank {
private:
	string inputFile;
	string outputFile;
	int numThreads;
	int maxNode = 0;
	double d = 0.85;      // 阻尼因子
	double tol = 1e-7;    // 收敛容忍度
	int maxIter = 1000;    // 最大迭代次数

	vector<vector<double>> M;
	vector<double> r, r2;

	void loadGraph(const string& filename, bool directed = false);
	void buildTransitionMatrix(bool directed = false);
	double computeNormDiff();

public:
	PageRank(const string& input, const string& output, int threads)
		: inputFile(input), outputFile(output), numThreads(threads) {
	}

	void run();
};