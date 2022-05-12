/* Ленский Кирилл, Б05-126. https://codeforces.com/group/PVbQ8eK2T4/contest/377095/submission/155205089

Дан взвешенный неориентированный граф. Требуется найти вес минимального пути между двумя вершинами.
______________________-
 * 
 * Ничего особенного, просто Дейкстра.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

class graph {
      public:
            graph (size_t N) : M(N), dist (N, -1) {}
            ~graph() = default;

            size_t find (size_t left, size_t right);
            void add_edge (size_t l, size_t r, size_t w);

      private:

            std::vector<std::vector<std::pair<size_t, size_t>>> M;
            std::vector<size_t> dist;
};

void graph::add_edge (size_t l, size_t r, size_t w) {
      M[l].push_back ({r, w});
      M[r].push_back ({l, w});
}

size_t graph::find (size_t left, size_t right) {
      
      auto cmp = [](std::pair<size_t, size_t> l, std::pair<size_t, size_t> r) {
            return l.second > r.second;
      };

      std::priority_queue<std::pair<size_t, size_t>,
                          std::vector<std::pair<size_t, size_t>>,
                          decltype (cmp)> q (cmp);
      q.push ({left, 0});

//      std::cout << "here" << std::endl;
//

      size_t iter = 1;

      while (!q.empty()) {
            
            //std::cout << iter++ << std::endl;

            size_t x = q.top().first;
            size_t w_dist = q.top().second;

            if (x == right)
                  return w_dist;

            q.pop ();

            if (dist[x] != size_t(-1))
                  continue;

            dist[x] = w_dist;

            for (auto [y, w] : M[x])
                  q.push ({y, w_dist+w});
      }

      return -1;
}

int main () {
      size_t N, M;
      size_t s, t;

      std::cin.tie (NULL);
      std::ios_base::sync_with_stdio (false);

      std::cin >> N >> M;
      std::cin >> s >> t;
      graph g(N);

      for (size_t i = 0; i < M; ++i) {
            size_t x, y, w;

            std::cin >> x >> y >> w;
            g.add_edge (x-1, y-1, w);
      }

      size_t r = g.find(s-1, t-1);

      if (r == size_t(-1))
            std::cout << "-1\n";
      else
            std::cout << r << "\n";
}
