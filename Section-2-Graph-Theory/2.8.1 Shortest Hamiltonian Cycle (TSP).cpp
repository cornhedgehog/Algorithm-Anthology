/*

Given a weighted, directed graph, determine a cycle of minimum total distance
which visits each node exactly once and returns to the starting node. This is
known as the traveling salesman problem (TSP). Since this implementation uses
bitmasks with 32-bit ints, the maximum number of nodes must be less than 32.

Time Complexity: O(2^n * n^2) on the number of nodes.
Space Complexity: O(2^n * n^2) auxiliary on the number of nodes.

*/

#include <algorithm>  // std::fill(), std::min()

const int MAXN = 20, INF = 0x3f3f3f3f;
int adj[MAXN][MAXN], dp[1 << MAXN][MAXN], order[MAXN];

int shortest_hamiltonian_cycle(int nodes) {
  int max_mask = (1 << nodes) - 1;
  for (int i = 0; i <= max_mask; i++) {
    std::fill(dp[i], dp[i] + nodes, INF);
  }
  dp[1][0] = 0;
  for (int mask = 1; mask <= max_mask; mask += 2) {
    for (int i = 1; i < nodes; i++) {
      if ((mask & 1 << i) != 0) {
        for (int j = 0; j < nodes; j++) {
          if ((mask & 1 << j) != 0) {
            dp[mask][i] = std::min(dp[mask][i],
                                   dp[mask ^ (1 << i)][j] + adj[j][i]);
          }
        }
      }
    }
  }
  int res = INF + INF;
  for (int i = 1; i < nodes; i++) {
    res = std::min(res, dp[max_mask][i] + adj[i][0]);
  }
  int cur = max_mask, old = 0;
  for (int i = nodes - 1; i >= 1; i--) {
    int bj = -1;
    for (int j = 1; j < nodes; j++) {
      if ((cur & 1 << j) != 0 &&
          (bj == -1 || dp[cur][bj] + adj[bj][old] > dp[cur][j] + adj[j][old])) {
        bj = j;
      }
    }
    order[i] = bj;
    cur ^= 1 << bj;
    old = bj;
  }
  return res;
}

void add_edge(int u, int v, int w) {
  adj[u][v] = w;
  adj[v][u] = w;  // Remove this line if the graph is directed.
}

/*** Example Usage and Output:

The shortest hamiltonian cycle has length 5.
Take the path: 0->3->2->4->1->0.

***/

#include <iostream>
using namespace std;

int main() {
  int nodes = 5;
  add_edge(0, 1, 1);
  add_edge(0, 2, 10);
  add_edge(0, 3, 1);
  add_edge(0, 4, 10);
  add_edge(1, 2, 10);
  add_edge(1, 3, 10);
  add_edge(1, 4, 1);
  add_edge(2, 3, 1);
  add_edge(2, 4, 1);
  add_edge(3, 4, 10);
  cout << "The shortest hamiltonian cycle has length "
       << shortest_hamiltonian_cycle(nodes) << "." << endl
       << "Take the path: ";
  for (int i = 0; i < nodes; i++) {
    cout << order[i] << "->";
  }
  cout << order[0] << "." << endl;
  return 0;
}
