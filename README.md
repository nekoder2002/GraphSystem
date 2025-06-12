# GraphSystem

����Ŀ��һ��֧��ϡ��ͼѹ���洢���Ч���ʵĲ���ͼ�㷨ϵͳ��֧�������в��������ò��� BFS��DFS �� PageRank �Ⱦ���ͼ�㷨�ļ���ʵ�֡�

## ����

- **CSR��ѹ��ϡ���У���ʽ**�洢ϡ��ͼ����ʡ�ڴ沢��������Ч��
- ֧��**���� BFS/DFS**����
- ֧��**���� PageRank**����
- �����в�����֧�ֶ��̡߳���㡢ͼ���͵�����

## ����Ҫ��

- C++20 ���������Ƽ� MSVC 2022 ����߰汾��
- CMake �� 3.8
- OpenMP ֧�֣����ڶ��̼߳��٣�

## ʹ�÷���

```sh
GraphSystem <inputFile> <outputFile> <algorithm> [options]
```

- `<inputFile>`������ͼ�ļ���ÿ��һ���ߣ���ʽ��`u v`��
- `<outputFile>`���������ļ�
- `<algorithm>`��`bfs` | `dfs` | `pagerank`
- `[options]` ��ѡ������
  - `--threads <n>`�������߳�����Ĭ�� 20��
  - `--start <node>`��������㣨Ĭ�� 0��
  - `--undirected`������ͼ��Ĭ������ͼ��

### ʾ��

1. ���� BFS �������ӽڵ� 0 ��ʼ��8 �̣߳���
```sh
GraphSystem web-Google.txt bfs_result.txt bfs --threads 8 --start 0
```

2. ���� DFS ����������ͼ����
```sh
GraphSystem web-Google.txt dfs_result.txt dfs --undirected
```

3. ���� PageRank ���㣺
```sh
GraphSystem web-Google.txt pr_result.txt pagerank --threads 16
```

## ���������ʽ

- **�����ļ�**��ÿ��һ���ߣ��ո�ָ����� `0 1` ��ʾ 0 ָ�� 1��
- **BFS/DFS ���**��ÿ��һ�������ʵ��Ľڵ���
- **PageRank ���**��ÿ�� `�ڵ���,PageRankֵ`���� `0,0.000123`

## ����ṹ

- `GraphSystem.cpp`���������������н���
- `struct/Graph.h`��`struct/Graph.cpp`��ͼ��CSR�洢�����
- `component/GraphVisitor.h`��`component/GraphVisitor.cpp`������BFS/DFS
- `component/PageRank.h`��`component/PageRank.cpp`������PageRank

## ���

����Ŀ����ѧϰ���о�ʹ�á�