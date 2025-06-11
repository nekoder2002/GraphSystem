#pragma once  

#include <omp.h>  
#include <iostream>  
#include <fstream>  
#include <sstream>  
#include <vector>  
#include <algorithm>  
#include <cmath>  

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::max;
using std::cout;
using std::cerr;
using std::endl;
using std::pair;

class PageRank {
private:
	string inputFile;
	string outputFile;
	int numThreads;
	bool isDirected;
	int maxNode = 0;
	double d = 0.85;      // 阻尼因子  
	double tol = 1e-7;    // 收敛容忍度  
	int maxIter = 1000;    // 最大迭代次数  

	vector<vector<double>> M;
	vector<double> r, r2;

	bool loadGraph();
	void buildTransitionMatrix();
	double computeNormDiff();

public:
	PageRank(const string& input, const string& output, bool directed, int threads) : inputFile(input), outputFile(output), isDirected(directed), numThreads(threads) {
	}

	void run();
};