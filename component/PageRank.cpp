#include "PageRank.h"

double PageRank::computeNormDiff() {
    int maxNode = graph.getMaxNode();
    double diff = 0.0;
#pragma omp parallel for reduction(+ : diff)
    for (int i = 0; i < maxNode; ++i) {
        double delta = r2[i] - r[i];
        diff += delta * delta;
    }
    return sqrt(diff);
}

void PageRank::run() {
    int maxNode = graph.getMaxNode();
    const auto& row_ptr = graph.getRowPtr();
    const auto& col_idx = graph.getColIdx();
    const auto& outDegree = graph.getOutDegree();

    // 初始化PageRank向量（均匀分布）
    r.assign(maxNode, 1.0 / maxNode);
    r2.assign(maxNode, 0.0);

    double startTime = omp_get_wtime();
    int iter;

    // PageRank迭代
    for (iter = 0; iter < maxIter; ++iter) {
        // 步骤1: 计算悬挂节点总贡献
        double hangSum = 0.0;
#pragma omp parallel for reduction(+:hangSum)
        for (int j = 0; j < maxNode; ++j) {
            if (outDegree[j] == 0) {
                hangSum += r[j]; // 悬挂节点的PageRank值总和
            }
        }

        // 步骤2: 计算新PageRank值
#pragma omp parallel for
        for (int i = 0; i < maxNode; ++i) {
            double sum = 0.0;
            // 遍历目标节点i的所有入边（CSR格式）
            int row_start = row_ptr[i];
            int row_end = row_ptr[i + 1];
            for (int idx = row_start; idx < row_end; ++idx) {
                int j = col_idx[idx]; // 源节点j
                // 转移概率 = 1.0 / 源节点出度
                sum += (1.0 / outDegree[j]) * r[j];
            }
            // PageRank公式: 
            // r_i = d * (入链贡献 + 悬挂节点均匀分配) + (1-d)/n
            r2[i] = d * (sum + hangSum / maxNode) + (1.0 - d) / maxNode;
        }

        // 检查收敛
        double normDiff = computeNormDiff();
        r.swap(r2); // 更新r为新的PageRank向量

        if (normDiff <= tol) {
            cout << "Converged after " << iter + 1 << " iterations." << endl;
            break;
        }
    }

    if (iter == maxIter) {
        cout << "Reached maximum iterations: " << maxIter << endl;
    }

    double endTime = omp_get_wtime();
    cout << "Parallel time: " << (endTime - startTime) << " seconds" << endl;

    // 输出结果
    ofstream outFile(outputFile);
    for (int i = 0; i < maxNode; ++i) {
        outFile << i << "," << r[i] << endl;
    }
    outFile.close();
    cout << "PageRank calculation finished!" << endl;
}