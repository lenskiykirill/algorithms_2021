/* Ленский Кирилл, 126. https://codeforces.com/group/PVbQ8eK2T4/contest/374347/submission/151349444

В состав Галактической империи входит 𝑁 планет. Между большинством из них существуют гиперканалы. Новый император повелел, чтобы с любой планеты можно было попасть на любую другую, пройдя только через один гиперканал. Каналы устроены так, что позволяют путешествовать только в одну сторону. Единственный оставшийся прокладчик гиперканалов расположен на базе около планеты с номером 𝐴. К сожалению, он не может путешествовать по уже существующим каналам, он всегда прокладывает новый. А наличие двух каналов в одном направлении между двумя планетами существенно осложняет навигацию. Ваша задача — найти такой маршрут для прокладчика, чтобы все необходимые каналы были построены, и не было бы построено лишних. В конце своего маршрута прокладчик должен оказаться на своей родной базе, с которой он начал движение.
 */

/* Ну что тут вообще сказать? Ищем Эйлеров цикл.
 */

#include <iostream>
#include <vector>


namespace dst {
      class graph {
            public:
                  graph (size_t N) : graph_size (N), M(N), marked(N) {
                        for (size_t i = 0; i < N; ++i) {
                              M[i].resize(N);
                              marked[i].resize(N);
                              marked[i].resize(N);
                              marked[i][i] = true;
                        }
                  }

                  ~graph () = default;

                  void add_edge (size_t i, size_t j) { M[i][j] = 1; }

                  void euler (size_t node);
                  void print ();
            private:
                  size_t graph_size;
                  std::vector<std::vector<bool>> M;
                  std::vector<std::vector<bool>> marked;

                  std::vector<size_t> loop;
      };

      void graph::euler (size_t node) {
            size_t printed = true;

            for (size_t next = 0; next < graph_size; ++next) {
                  if (M[node][next]) {
                        if (!marked[node][next]) {
                              marked[node][next] = true;
                              
                              euler (next);
                        }
                  }
            }

            loop.push_back (node);
      }

      void graph::print () {
            for (size_t i = loop.size()-1; i != 0; --i)
                  std::cout << (loop[i]+1) << ' ' << (loop[i-1]+1) << '\n';
      }
}

int main () {
      size_t N, A;

      std::cin >> N >> A;

      dst::graph G(N);

      for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j) {
                  bool x;
                  std::cin >> x;
                  if (i != j)
                        if (!x)
                              G.add_edge (i, j);
            }

      G.euler (A-1);
      G.print();
}
