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

    // ��ʼ��PageRank���������ȷֲ���
    r.assign(maxNode, 1.0 / maxNode);
    r2.assign(maxNode, 0.0);

    double startTime = omp_get_wtime();
    int iter;

    // PageRank����
    for (iter = 0; iter < maxIter; ++iter) {
        // ����1: �������ҽڵ��ܹ���
        double hangSum = 0.0;
#pragma omp parallel for reduction(+:hangSum)
        for (int j = 0; j < maxNode; ++j) {
            if (outDegree[j] == 0) {
                hangSum += r[j]; // ���ҽڵ��PageRankֵ�ܺ�
            }
        }

        // ����2: ������PageRankֵ
#pragma omp parallel for
        for (int i = 0; i < maxNode; ++i) {
            double sum = 0.0;
            // ����Ŀ��ڵ�i��������ߣ�CSR��ʽ��
            int row_start = row_ptr[i];
            int row_end = row_ptr[i + 1];
            for (int idx = row_start; idx < row_end; ++idx) {
                int j = col_idx[idx]; // Դ�ڵ�j
                // ת�Ƹ��� = 1.0 / Դ�ڵ����
                sum += (1.0 / outDegree[j]) * r[j];
            }
            // PageRank��ʽ: 
            // r_i = d * (�������� + ���ҽڵ���ȷ���) + (1-d)/n
            r2[i] = d * (sum + hangSum / maxNode) + (1.0 - d) / maxNode;
        }

        // �������
        double normDiff = computeNormDiff();
        r.swap(r2); // ����rΪ�µ�PageRank����

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

    // ������
    ofstream outFile(outputFile);
    for (int i = 0; i < maxNode; ++i) {
        outFile << i << "," << r[i] << endl;
    }
    outFile.close();
    cout << "PageRank calculation finished!" << endl;
}