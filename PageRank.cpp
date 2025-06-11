#include "PageRank.h"

bool PageRank::loadGraph() {
	ifstream infile(inputFile);
	if (!infile.is_open()) {
		return false;
	}
	string line;
	vector<pair<int, int>> edges;

	while (getline(infile, line)) {
		istringstream iss(line);
		int u, v;
		if (iss >> u >> v) {
			edges.push_back({ u, v });
			if (!isDirected) {
				edges.push_back({ v, u });
			}
		}
	}

	// Find max node ID
	auto get_max_node = [](const pair<int, int>& p) {
		return max(p.first, p.second);
		};

#pragma omp parallel for reduction(max:maxNode)
	for (const auto& e : edges) {
		maxNode = max(maxNode, max(e.first, e.second));
	}
	maxNode += 1; // 节点编号从0开始

	// 初始化邻接矩阵
	M.assign(maxNode, vector<double>(maxNode, 0.0));
	r.assign(maxNode, 1.0 / maxNode);
	r2.assign(maxNode, 0.0);

	// 构建邻接矩阵
#pragma omp parallel for
	for (int i = 0; i < edges.size(); ++i) {
		int u = edges[i].first;
		int v = edges[i].second;
#pragma omp atomic
		M[u][v] += 1.0;
	}
	return true;
}

void PageRank::buildTransitionMatrix() {
	vector<double> outDegree(maxNode, 0.0);
#pragma omp parallel for
	for (int i = 0; i < maxNode; ++i) {
		double sum = 0.0;
		for (int j = 0; j < maxNode; ++j) {
			sum += M[i][j];
		}
		outDegree[i] = sum;
	}

	double teleport = (1.0 - d) / maxNode;
#pragma omp parallel for collapse(2)
	for (int i = 0; i < maxNode; ++i) {
		for (int j = 0; j < maxNode; ++j) {
			if (outDegree[j] > 0) {
				M[i][j] = d * (M[i][j] / outDegree[j]) + teleport;
			}
			else {
				M[i][j] = teleport;
			}
		}
	}
}

double PageRank::computeNormDiff() {
	double diff = 0.0;
#pragma omp parallel for reduction(+ : diff)
	for (int i = 0; i < maxNode; ++i) {
		diff += (r2[i] - r[i]) * (r2[i] - r[i]);
	}
	return sqrt(diff);
}


void PageRank::run() {
	omp_set_num_threads(numThreads);
	if (!loadGraph()) {
		cerr << "Error opening file: " << inputFile << endl;
		return;
	}
	buildTransitionMatrix();
	double startTime = omp_get_wtime();

	int iter;
	for (iter = 0; iter < maxIter; ++iter) {
#pragma omp parallel
		{
#pragma omp for
			for (int i = 0; i < maxNode; ++i) {
				r2[i] = 0.0;
			}

#pragma omp for
			for (int i = 0; i < maxNode; ++i) {
				for (int j = 0; j < maxNode; ++j) {
					r2[i] += M[i][j] * r[j];
				}
			}
		}

		double normDiff = computeNormDiff();
		r = r2;

		if (normDiff <= tol) {
			cout << "Converged after " << iter + 1 << " iterations." << endl;
			break;
		}
	}

	if (iter == maxIter) {
		cout << "Reached maximum iterations: " << maxIter << endl;
	}

	double endTime = omp_get_wtime();
	printf("并行时间：%lf 秒\n", endTime - startTime);

	ofstream outFile(outputFile);
	for (int i = 0; i < maxNode; ++i) {
		outFile << i << "," << r[i] << endl;
	}
	outFile.close();
	std::cout << "PageRank calcuate finished！" << std::endl;
}