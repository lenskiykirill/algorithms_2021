/* Кирилл Ленский, 126. https://codeforces.com/group/PVbQ8eK2T4/contest/377095/submission/155210435

   На шахматной доске размером 8×8 заданы две клетки. Соедините эти клетки кратчайшим путем коня.

 * Просто строю граф и делаю по нему BFS.
 */


#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

namespace dst {
      class chess {
            public:
                  chess ();
                  ~chess() = default;
                  
                  void find (size_t x, size_t y);
                  std::vector<std::pair<size_t, size_t>> operator() (size_t x, size_t y);

            private:
                  std::vector<std::vector<int>> dist;
      };

      chess::chess () : dist (8) {
            for (auto& v : dist)
                  v.resize (8, -1);
      }
      
      void chess::find (size_t x, size_t y) {
            std::queue<std::pair<std::pair<size_t, size_t>, int>> q;
            q.push ({{x, y}, 0});

            while (!q.empty()) {
                  x = q.front().first.first;
                  y = q.front().first.second;

                  int d = q.front().second;

                  q.pop ();

                  if (dist[x][y] == -1)
                        dist[x][y] = d;
                  else
                        continue;

                  size_t _x, _y;

                  std::vector<std::pair<int, int>> next;

                  for (int _x : {-1, 1})
                        for (int _y : {-2, 2})
                              next.push_back ({x+_x, y+_y});

                  for (int _x : {-2, 2})
                        for (int _y : {-1, 1})
                              next.push_back ({x+_x, y+_y});

                  for (auto p : next) {
                        if (p.first < 0 || p.first > 7)
                              continue;
                        if (p.second < 0 || p.second > 7)
                              continue;

                        q.push ({p, d+1});
                  }
            }
      }

      std::vector<std::pair<size_t, size_t>> chess::operator() (size_t X, size_t Y) {
            int x = X, y = Y;
            int d = dist[x][y];

            std::vector<std::pair<size_t, size_t>> res;

            while (d > 0) {
                  res.push_back ({x, y});

                  decltype (res) next;
                  for (int _x : {-1, 1})
                        for (int _y : {-2, 2})
                              next.push_back ({x+_x, y+_y});

                  for (int _x : {-2, 2})
                        for (int _y : {-1, 1})
                              next.push_back ({x+_x, y+_y});


                  for (auto p : next) {
                        if (p.first < 0 || p.first > 7)
                              continue;
                        if (p.second < 0 || p.second > 7)
                              continue;

                        if (dist[p.first][p.second] == d-1) {
                              x = p.first;
                              y = p.second;
                              --d;

                              break;
                        }
                  }
            }
            
            res.push_back ({x, y});
            std::reverse (res.begin(), res.end());

            return res;
      }
}

int main () {
      dst::chess g;

      std::string s;
      std::cin >> s;

      char x = s[0];
      char y = s[1];

      g.find (x-'a', y-'1');

      std::cin >> s;
      x = s[0];
      y = s[1];

      auto res = g(x-'a', y-'1');

      for (auto p : res) {
            std::cout << char('a'+p.first) << char('1'+p.second) << std::endl;
      }
}
