// GraphSystem.cpp: 定义应用程序的入口点。
//

#include "GraphSystem.h"

using namespace std;

int main() {
    PageRank pagerank("D:\\Code\\Study\\GraphSystem\\facebook_combined.txt",
        "D:\\Code\\Study\\GraphSystem\\result.txt",
        16); // 使用 16 个线程

    pagerank.run();

    std::cout << "PageRank calcuate finished！" << std::endl;
    return 0;
}