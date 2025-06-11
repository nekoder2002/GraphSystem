#include "GraphSystem.h"

int main() {
	// 设置并行线程数
	int numThreads = 20;

	// 图配置
	std::string inputFile = "D:\\Code\\Study\\GraphSystem\\dataset\\web-Google.txt";
	std::string outputFile = "D:\\Code\\Study\\GraphSystem\\result.txt";
	bool isDirected = true; // 有向图

	// 创建并加载图
	Graph graph;
	if (!graph.load(inputFile, isDirected, numThreads)) {
		std::cerr << "Failed to load graph from " << inputFile << std::endl;
		return 1;
	}

	// 创建图访问器
	int visitorThreads = 8;  // 使用不同的变量名避免冲突
	GraphVisitor visitor(graph, visitorThreads);

	// 执行并行BFS
	std::cout << "Running Parallel BFS from node 0..." << std::endl;
	visitor.runBFS(0);

	// 执行并行DFS
	std::cout << "\nRunning Parallel DFS from node 0..." << std::endl;
	visitor.runDFS(0);

	// 检查访问结果
	std::cout << "\nChecking visited nodes..." << std::endl;
	const auto* visited = visitor.getVisited();
	int maxNode = graph.getMaxNode();

	int visitedCount = 0;
	for (int i = 0; i < maxNode; ++i) {
		if (visited[i].load(std::memory_order_relaxed)) {
			visitedCount++;
		}
	}

	std::cout << "Visited " << visitedCount << " nodes out of "
		<< maxNode << std::endl;

	return 0;
}