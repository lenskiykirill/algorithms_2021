/* Ленский Кирилл, 126. https://codeforces.com/group/PVbQ8eK2T4/contest/371629/submission/149654538

Компания BrokenTiles планирует заняться выкладыванием во дворах у состоятельных клиентов узор из черных и белых плиток, каждая из которых имеет размер 1×1 метр. Известно, что дворы всех состоятельных людей имеют наиболее модную на сегодня форму прямоугольника MxN метров.

Однако при составлении финансового плана у директора этой организации появилось целых две серьезных проблемы: во первых, каждый новый клиент очевидно захочет, чтобы узор, выложенный у него во дворе, отличался от узоров всех остальных клиентов этой фирмы, а во вторых, этот узор должен быть симпатичным.

Как показало исследование, узор является симпатичным, если в нем нигде не встречается квадрата 2x2 метра, полностью покрытого плитками одного цвета.

Для составления финансового плана директору необходимо узнать, сколько клиентов он сможет обслужить, прежде чем симпатичные узоры данного размера закончатся. Помогите ему!

---------------------------------------------
* в этой задаче просто вводится маска на i-й слой, и матрица, которая добавляет число, соответствующее маске на i-м слое ко всем маскам i+1-го слоя, которые могут идти после i-го слоя. А потом матрица
* просто возводится в степень числа слоев и умножается на вектор из единиц и нулей, соотв. первому слою
*
*/


#include <climits>
#include <iostream>
 
class matrix {
      private:
            struct mul_struct;
      public:
            matrix (unsigned long long n, unsigned long long mod);
            matrix (const matrix& other);
            ~matrix ();
            matrix& operator= (const matrix& other);
            unsigned long long size () const {return N;}
 
            void power (unsigned long long p);
 
            unsigned long long* operator[] (unsigned long long idx) { return mat[idx]; }
            const unsigned long long* operator[] (unsigned long long idx) const { return mat[idx]; }
 
      private:
 
            unsigned long long N = 0;
            unsigned long long mod;
            unsigned long long** mat;
 
            void power (unsigned long long p, matrix& tmp1, matrix& tmp2);
            void multiply (const matrix& left, const matrix& right);
            static void swap (matrix& left, matrix& right);
};
 
matrix::matrix (unsigned long long n, unsigned long long mod) {
      this->mod = mod;
      N = n;
      mat = new unsigned long long* [N];
 
      for (unsigned long long i = 0; i < N; ++i) {
            mat[i] = new unsigned long long [N];
            for (unsigned long long j = 0; j < N; ++j)
                  mat[i][j] = (i != j);
      }
}
 
matrix::matrix (const matrix& other) {
      N = other.N;
      mod = other.mod;
 
      mat = new unsigned long long* [N];
 
      for (unsigned long long i = 0; i < N; ++i) {
            mat[i] = new unsigned long long [N];
            for (unsigned long long j = 0; j < N; ++j) {
                  mat[i][j] = other.mat[i][j];
            }
      }
}
 
matrix::~matrix () {
      for (unsigned long long i = 0; i < N; ++i)
            delete [] mat[i];
      delete [] mat;
}
 
matrix& matrix::operator= (const matrix& other) {
      matrix res (other);
      swap (*this, res);
 
      return *this;
}
 
void matrix::multiply (const matrix& left, const matrix& right) {
      for (unsigned long long i = 0; i < N; ++i)
            for (unsigned long long j = 0; j < N; ++j) {
                  unsigned long long r = 0;
 
                  for (unsigned long long k = 0; k < N; ++k) {
                        r += (left.mat[i][k] * right.mat[k][j]) % mod;
                        r %= mod;
                  }
                  mat[i][j] = r;
            }
}
 
void matrix::power (unsigned long long p, matrix& tmp1, matrix& tmp2) {
      if (p == 1)
            return;
      if (p % 2) {
            swap (*this, tmp1);
            tmp1.power (p-1, *this, tmp2);
            multiply (tmp1, tmp2);
            return;
      }
 
      swap (*this, tmp1);
      tmp1.power (p/2, *this, tmp2);
      multiply (tmp1, tmp1);
}
 
void matrix::power (unsigned long long p) {
      matrix tmp1 (N, mod);
      matrix tmp2 (*this);
 
      power (p, tmp1, tmp2);
}
 
void matrix::swap (matrix& left, matrix& right) {
      std::swap (left.N, right.N);
      std::swap (left.mat, right.mat);
      std::swap (left.mod, right.mod);
}

unsigned long long solve (unsigned long long n, unsigned int m, unsigned long long mod) {
      unsigned int D = 1 << m;

      if (n == 1)
            return D % mod;

      matrix M (D, mod);

      for (unsigned int d1 = 0; d1 < D; ++d1) {
            for (unsigned int d2 = 0; d2 < D; ++d2) {
                  unsigned int s1 = d1 & (d1 >> 1);
                  unsigned int s2 = d2 & (d2 >> 1);

                  if (s1 & s2) {
                        M[d1][d2] = 0;
                        continue;
                  }

                  s1 = d1 | (d1 >> 1);
                  s2 = d2 | (d2 >> 1);

                  s1 &= (D-1) >> 1;
                  s2 &= (D-1) >> 1;

                  s1 ^= (D-1) >> 1;
                  s2 ^= (D-1) >> 1;

                  if (s1 & s2) {
                        M[d1][d2] = 0;
                        continue;
                  }

                  M[d1][d2] = 1;
            }
      }

      M.power (n-1);
      
      unsigned long long res = 0;
      for (unsigned int i = 0; i < D; ++i)
            for (unsigned int j = 0; j < D; ++j) {
                  res += M[i][j];
                  res %= mod;
            }

      return res;
}

int main () {
      unsigned int N, M;

      std::cin >> N >> M;

      if (N < M)
            std::swap (N, M);

      std::cout << solve (N, M, ULLONG_MAX) << std::endl;
}
