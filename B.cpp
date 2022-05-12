/* Ленский Кирилл, 126. https://codeforces.com/group/PVbQ8eK2T4/contest/374347/submission/151366280

Напишите программу, которая для двух вершин дерева определяет, является ли одна из них предком другой.
 *
 * делаю топологическую сортировку, при которой запоминаю время входа и выхода в каждую вершину. Если в A вошли раньше, чем в B, а вышли позже, то A предок B */

#include <iostream>
#include <algorithm>
#include <vector>

namespace dst {
      class graph {
            public:
                  graph (size_t N): M(N), used(N, 0), tout (N), tin (N) {};
                  ~graph () = default;

                  void add_edge (size_t x, size_t y) { M[x].push_back (y); }

                  size_t sort ();

                  bool is_ancestor (size_t x, size_t y);
                  void init (size_t top);
            private:
                  std::vector<std::vector<size_t>> M;
                  std::vector<short> used;
                  std::vector<size_t> tout;
                  std::vector<size_t> tin;

                  size_t timer = 0;
                  size_t tin_timer = 0;

                  size_t sort_dfs (size_t node, size_t parent);
      };

      size_t graph::sort_dfs (size_t node, size_t parent) {

            used[node] = 1;

            tin[node] = tin_timer++;

            for (size_t i = 0; i < M[node].size(); ++i) {
                  size_t next = M[node][i];

                  if (used[next] == 0) {
                        size_t res = sort_dfs (next, node);

                        if (res == -1)
                              return -1;
                  }

                  if (used[next] == 1)
                        return -1;
            }
            
            used[node] = 2;
            tout[node] = timer++;

            return 0;
      }

      void graph::init (size_t top) {
            sort_dfs (top, -1);
      }

      size_t graph::sort () {
            for (size_t node = 0; node < M.size(); ++node) {
                  if (used[node] == 2)
                        continue;
                  
                  size_t res = sort_dfs (node, -1);
                  
                  if (res == -1) {
                        std::cout << "-1\n";
                        return -1;
                  }
            }

            std::vector<size_t> sorted (M.size());

            for (size_t i = 0; i < M.size(); ++i) {
                  sorted[M.size() - tout[i] - 1] = i;
            }

            for (auto x : sorted)
                  std::cout << (x+1) << ' ';
            std::cout << '\n';

            return 0;
      }

      bool graph::is_ancestor (size_t a, size_t b) {
            return ((tin[a] <= tin[b]) && (tout[a] >= tout[b]));
      }
}

int main () {
      size_t N, M;
      std::cin >> N;

      dst::graph G(N);

      size_t top;

      for (size_t y = 0; y < N; ++y) {
            size_t x;
            std::cin >> x;

            if (x == 0) {
                  top = y;
                  continue;
            }

            G.add_edge (x-1, y);
      }
      
      G.init (top);

      std::cin >> M;

      for (size_t i = 0; i < M; ++i) {
            size_t x, y;
            std::cin >> x >> y;

            std::cout << G.is_ancestor(x-1, y-1) << '\n';
      }
}
