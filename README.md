# GraphSystem

本项目是一个支持稀疏图压缩存储与高效访问的并行图算法系统，支持命令行操作，内置并行 BFS、DFS 及 PageRank 等经典图算法的加速实现。

## 特性

- **CSR（压缩稀疏行）格式**存储稀疏图，节省内存并提升遍历效率
- 支持**并行 BFS/DFS**遍历
- 支持**并行 PageRank**计算
- 命令行参数灵活，支持多线程、起点、图类型等配置

## 构建要求

- C++20 编译器（推荐 MSVC 2022 或更高版本）
- CMake ≥ 3.8
- OpenMP 支持（用于多线程加速）

## 使用方法

```sh
GraphSystem <inputFile> <outputFile> <algorithm> [options]
```

- `<inputFile>`：输入图文件（每行一条边，格式：`u v`）
- `<outputFile>`：结果输出文件
- `<algorithm>`：`bfs` | `dfs` | `pagerank`
- `[options]` 可选参数：
  - `--threads <n>`：并行线程数（默认 20）
  - `--start <node>`：遍历起点（默认 0）
  - `--undirected`：无向图（默认有向图）

### 示例

1. 并行 BFS 遍历（从节点 0 开始，8 线程）：
```sh
GraphSystem web-Google.txt bfs_result.txt bfs --threads 8 --start 0
```

2. 并行 DFS 遍历（无向图）：
```sh
GraphSystem web-Google.txt dfs_result.txt dfs --undirected
```

3. 并行 PageRank 计算：
```sh
GraphSystem web-Google.txt pr_result.txt pagerank --threads 16
```

## 输入输出格式

- **输入文件**：每行一条边，空格分隔（如 `0 1` 表示 0 指向 1）
- **BFS/DFS 输出**：每行一个被访问到的节点编号
- **PageRank 输出**：每行 `节点编号,PageRank值`，如 `0,0.000123`

## 代码结构

- `GraphSystem.cpp`：主程序与命令行解析
- `struct/Graph.h`、`struct/Graph.cpp`：图的CSR存储与加载
- `component/GraphVisitor.h`、`component/GraphVisitor.cpp`：并行BFS/DFS
- `component/PageRank.h`、`component/PageRank.cpp`：并行PageRank

## 许可

本项目仅供学习与研究使用。