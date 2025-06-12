#include "GraphSystem.h"

void printUsage() {
    std::cout << "用法: GraphSystem <inputFile> <outputFile> <algorithm> [options]\n"
        << "算法: bfs | dfs | pagerank\n"
        << "示例: GraphSystem web-Google.txt result.txt bfs\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printUsage();
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string algorithm = argv[3];
    int numThreads = 20;
    int startNode = 0;
    bool isDirected = true;

    // 可选参数解析
    for (int i = 4; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--threads" && i + 1 < argc) {
            numThreads = std::stoi(argv[++i]);
        }
        else if (arg == "--start" && i + 1 < argc) {
            startNode = std::stoi(argv[++i]);
        }
        else if (arg == "--undirected") {
            isDirected = false;
        }
    }

    // 加载图（CSR压缩存储）
    Graph graph;
    if (!graph.load(inputFile, isDirected, numThreads)) {
        std::cerr << "Failed to load graph from " << inputFile << std::endl;
        return 1;
    }

    if (algorithm == "bfs" || algorithm == "dfs") {
        GraphVisitor visitor(graph, numThreads);
        if (algorithm == "bfs") {
            std::cout << "Running Parallel BFS from node " << startNode << "..." << std::endl;
            visitor.runBFS(startNode);
        }
        else {
            std::cout << "Running Parallel DFS from node " << startNode << "..." << std::endl;
            visitor.runDFS(startNode);
        }
        // 可选：输出访问结果
        std::ofstream out(outputFile);
        auto* visited = visitor.getVisited();
        int n = visitor.getVisitedSize();
        for (int i = 0; i < n; ++i) {
            if (visited[i].load(std::memory_order_relaxed)) {
                out << i << "\n";
            }
        }
        out.close();
    }
    else if (algorithm == "pagerank") {
        PageRank pr(numThreads, graph, outputFile);
        pr.run();
    }
    else {
        printUsage();
        return 1;
    }

    return 0;
}