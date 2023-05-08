#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

void bfs_seq(vector<vector<int>>& graph, int start, vector<bool>& visited) {
    queue<int> q;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : graph[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
}

void bfs_par(vector<vector<int>>& graph, int start, vector<bool>& visited) {
    queue<int> q;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int size = q.size();

        #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            int u = q.front();
            q.pop();

            #pragma omp parallel for
            for (int v : graph[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    #pragma omp critical
                    q.push(v);
                }
            }
        }
    }
}

void dfs_seq(vector<vector<int>>& graph, int u, vector<bool>& visited) {
    visited[u] = true;

    for (int v : graph[u]) {
        if (!visited[v]) {
            dfs_seq(graph, v, visited);
        }
    }
}

void dfs_par(vector<vector<int>>& graph, int u, vector<bool>& visited) {
    visited[u] = true;

    #pragma omp parallel for
    for (int v : graph[u]) {
        if (!visited[v]) {
            dfs_par(graph, v, visited);
        }
    }
}

int main() {
    // Initialize graph
    int n =100;
    vector<vector<int>> graph(n);
    for (int i = 0; i < n; i++) {
        int m = rand() % 10;
        for (int j = 0; j < m; j++) {
            int v = rand() % n;
            graph[i].push_back(v);
        }
    }

    // Initialize start node and visited array
    int start = 0;
    vector<bool> visited(n, false);

    // Sequential BFS
    auto start_time = high_resolution_clock::now();
    bfs_seq(graph, start, visited);
    auto end_time = high_resolution_clock::now();
    auto time_seq_bfs = duration_cast<microseconds>(end_time - start_time).count();

    // Sequntial DFS
    start_time = high_resolution_clock::now();
    fill(visited.begin(), visited.end(), false);
    dfs_seq(graph, start, visited);
    end_time = high_resolution_clock::now();
    auto time_seq_dfs = duration_cast<microseconds>(end_time - start_time).count();

    // Parallel BFS
    start_time = high_resolution_clock::now();
    fill(visited.begin(), visited.end(), false);
    bfs_par(graph, start, visited);
    end_time = high_resolution_clock::now();
    auto time_par_bfs = duration_cast<microseconds>(end_time - start_time).count();

    // Parallel DFS
    start_time = high_resolution_clock::now();
    fill(visited.begin(), visited.end(), false);
    dfs_par(graph, start, visited);
    end_time = high_resolution_clock::now();
    auto time_par_dfs = duration_cast<microseconds>(end_time - start_time).count();

    // Print results
    cout << "Sequential BFS took " << time_seq_bfs << " ms" << endl;
    cout << "Sequential DFS took " << time_seq_dfs << " ms" << endl;
    cout << "Parallel BFS took " << time_par_bfs << " ms" << endl;
    cout << "Parallel DFS took " << time_par_dfs << " ms" << endl;

    // Calculate speedup
    double speedup_bfs = static_cast<double>(time_seq_bfs) / time_par_bfs;
    double speedup_dfs = static_cast<double>(time_seq_dfs) / time_par_dfs;
    cout << "Speedup BFS: " << speedup_bfs << endl;
    cout << "Speedup DFS: " << speedup_dfs << endl;

    return 0;
}


