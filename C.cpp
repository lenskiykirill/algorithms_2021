/* Ленский Кирилл, 126. https://codeforces.com/group/PVbQ8eK2T4/contest/374347/submission/151364809

Дан ориентированный невзвешенный граф без петель и кратных рёбер. Необходимо определить есть ли в нём циклы, и если есть, то вывести любой из них.
-------------
Ну, я завожу dfs и храню родителя. Когда посещаю уже посещенную вершину, вывожу, что цикл есть и по родителям его восстанавливаю
*/


#include <iostream>
#include <vector>
#include <algorithm>

namespace dst {
      class graph {
            public:
                  graph (size_t N) : M(N), P(N), used(N, 0) {}
                  ~graph () = default;

                  void add_edge (size_t a, size_t b) {
                        M[a].push_back(b);
                  }

                  void get_cycle ();
            private:
                  std::vector<std::vector<size_t>> M;
                  std::vector<size_t> P;
                  std::vector<short> used;

                  size_t x;
                  size_t y;

                  size_t dfs (size_t node, size_t parent);
      };

      size_t graph::dfs (size_t node, size_t parent) {
            used[node] = 1;
            P[node] = parent;

            for (size_t i = 0; i < M[node].size(); ++i) {
                  size_t next = M[node][i];

                  if (used[next] == 1) {
                        x = next;
                        y = node;
                        return -1;
                  }

                  size_t res = dfs (next, node);

                  if (res == -1)
                        return -1;
            }

            used[node] = 2;

            return 0;
      }

      void graph::get_cycle () {
            for (size_t i = 0; i < M.size(); ++i) {
                  if (used[i])
                        continue;
                  
                  size_t res = dfs(i, -1);

                  if (res == -1) {
                        size_t ret = x;

                        std::cout << "YES\n";

                        std::vector<size_t> ans;

                        ans.push_back (x+1);
                        x = y;

                        while (x != ret) {
                              ans.push_back(x+1);
                              x = P[x];
                        }

                        std::reverse (ans.begin(), ans.end());

                        for (size_t i = 0; i < ans.size(); ++i)
                              std::cout << ans[i] << ' ';

                        std::cout << "\n";

                        return;
                  }
            }

            std::cout << "NO\n";
      }
}

int main () {
      size_t N, M;
      std::cin >> N >> M;

      dst::graph G(N);

      for (size_t i = 0; i < M; ++i) {
            size_t x, y;
            std::cin >> x >> y;

            G.add_edge (x-1, y-1);
      }

      G.get_cycle ();
}
