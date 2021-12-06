// https://codeforces.com/group/PVbQ8eK2T4/contest/347382/submission/135512960
/*
Отсортируйте данную последовательность, используя алгоритм быстрой сортировки QuickSort.

Решения, использующие случайные числа, получат 1 балл.
Решения без использования случайных чисел получат 2 балла.

Входные данные

В первой строке дается число n (1≤n≤10^5)- количество элементов в массиве, затем даются n чисел (0≤ai≤10^9).

Выходные данные

В единственной строке выходного файла выведите последовательность в неубывающем порядке. 
*/

#include <iostream>

namespace dst {
template <class T>
	struct __quick_swp_default {
	       	
		void operator() (T& left, T& right) {
			T swp = left;
			left = right;
			right = swp;
		}
	};

template <class T>
	struct __quick_cmp_default {

		bool operator() (T& left, T& right) {
			return left < right;
		}
	};

template <class T, class C, class S>
	void __quick_partition (
				T* array, size_t size,
				size_t& middle, size_t& middle_size,
				C cmp_function, S swp_function
			       ) {
		
		swp_function (array[middle], array[size-1]);

		size_t i = 0;
		size_t j = size - 1;

		size_t middle_array = size - 1;

		while (true) {
			while ( (i < size - 1) && (middle_array > i) &&
			        !cmp_function (array[size-1], array[i]) ) {
				
				if (!cmp_function (array[i], array[size-1])) {
					
					if (j == middle_array)
						--j;
					
					swp_function (array[i], array[--middle_array]);
				} else {
					++i;
				}
			}

			while ( (j > 0) && // TODO: check this!
				!cmp_function (array[j-1], array[size-1]) ) {
				
				if (!cmp_function (array[size-1], array[j-1])) {
					swp_function (array[j-1], array[--middle_array]);
				} else {
					--j;
				}
			}

			if (i == j) {
				middle = j;
				middle_size = 0;

				while (middle_array < size) {
					swp_function (array[j++], array[middle_array++]);
					++middle_size;
				}

				return;
			}

			swp_function (array[i], array[j-1]);
		}
	}

template <class T, class C, class S>
	void __quick_s_sort (
			     T* array, size_t size,
			     C cmp_function, S swp_function
			    ) {
		
		for (size_t i = 0; i < size-1; ++i) {
			for (size_t j = i+1; j < size; j++) {
				if (cmp_function (array[j], array[i])) {
					swp_function (array[j], array[i]);
				}
			}
		}
	}

template <class T, class C, class S>
	size_t __quick_select (
			       T* array, size_t size,
			       size_t k,
			       C cmp_function, S swp_function
			      );

template <class T, class C, class S>
	size_t __quick_middle (
			       T* array, size_t size,
			       C cmp_function, S swp_function,
			       const unsigned int part_size = 5
			      ) {
		
		if (size < part_size) {
			return 0;
		}

		for (size_t i = 0; i < size / part_size; ++i) {
			__quick_s_sort (
				        &(array[part_size * i]),
				        part_size,
				        cmp_function, swp_function
			   	       );

			swp_function (
				      array[part_size*i + part_size / 2],
				      array[i]
				     );
		}

		return __quick_select (
				       array, size / part_size,
				       size / (2*part_size),
				       cmp_function, swp_function
				      );
	}

template <class T, class C, class S>
	size_t __quick_select (
			       T* array, size_t size,
			       size_t k,
			       C cmp_function, S swp_function
			      ) {
		if (size == 1) { return 0; }

		size_t middle = __quick_middle (
						array, size,
						cmp_function, swp_function
					       );
		size_t middle_size;

		__quick_partition (
				   array, size, middle, middle_size,
				   cmp_function, swp_function
				  );
		if (k < middle) {
			return __quick_select (
					       array, middle, k,
					       cmp_function, swp_function
					      );
		}

		if (k < middle + middle_size) {
			return middle;
		}

		return middle + middle_size +
		       __quick_select (
				       &(array[middle + middle_size]),
				       size - (middle + middle_size),
				       k - (middle + middle_size),
				       cmp_function, swp_function
				      );
	}

template <class T, class C = __quick_cmp_default<T>, class S = __quick_swp_default<T>>
	void quick_sort (
			 T* array, size_t size,
			 C cmp_function = __quick_cmp_default<T>(),
			 S swp_function = __quick_swp_default<T>()
			) {
		if (size <= 1) { return; }

		size_t middle = __quick_middle (
						array, size,
						cmp_function, swp_function
					       );
		size_t middle_size;

		__quick_partition (
				   array, size, middle, middle_size,
				   cmp_function, swp_function
				  );

		quick_sort (array, middle, cmp_function, swp_function);
		quick_sort (
			    &(array[middle + middle_size]),
			    size - (middle + middle_size),
			    cmp_function, swp_function
			   );
	}
}

#define SIZE 4

int main () {
size_t N;
std::cin >> N;

long long* array = new long long [N];

for (size_t i = 0; i < N; i++) {
	std::cin >> array[i];
}

dst::quick_sort (array, N);

for (size_t i = 0; i < N; i++) {
	std::cout << array[i] << ' ';
}

std::cout << '\n';

delete [] array;

return 0;
}
