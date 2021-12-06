/*
Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 300000. Отсортировать массив методом поразрядной сортировки LSD по байтам.

Входные данные

В первой строке вводится количество чисел в массиве N. На следующей строке через пробел вводятся N неотрицательных чисел.

Выходные данные

Выведите этот массив, отсортированный в порядке возрастания, в одну строчку через пробел.
*/

#include <iostream>
#include <bits/stdc++.h>

#define BYTE_SIZE (1 << 8)

namespace dst {
void __lsd_sort (unsigned long long* array, size_t size, size_t sorted_byte, size_t* ptr_array, unsigned long long* swap_array) {
	size_t first [BYTE_SIZE];
	size_t last  [BYTE_SIZE];

	for (size_t i = 0; i < BYTE_SIZE; i++) {
		first[i] = size;
		last[i] = size;
	}

	for (size_t i = 0; i < size; i++) {
		
		size_t byte = (array[i] >> 8*sorted_byte) & 0xff;

		if (first[byte] == size) {
			first[byte] = i;
			last[byte] = i;
		
		} else {
			ptr_array[last[byte]] = i;
			last[byte] = i;
		}
	}

	size_t count = 0;

	for (size_t byte = 0; byte < BYTE_SIZE; byte++) {

		size_t ptr = first[byte];

		if (first[byte] != size) {
		
			while (ptr != last[byte]) {
				swap_array[count++] = array[ptr];

				ptr = ptr_array[ptr];
			}
			
			swap_array[count++] = array[ptr];
		}
	}

	for (size_t i = 0; i < size; i++) {
		
		array[i] = swap_array[i];
	}
	
	return;
}

void lsd_sort (unsigned long long* array, size_t size) {

	unsigned long long* swap_array = new unsigned long long [size];
	size_t* ptr_array = new size_t [size];
	
	for (size_t byte=0; byte < sizeof (long long int); byte++) {

		__lsd_sort(array, size, byte, ptr_array, swap_array);
	}

	delete [] swap_array;
	delete [] ptr_array;

	return;
}
}

int main () {
std::ios_base::sync_with_stdio(false);
std::cin.tie(NULL);

size_t n;
std::cin >> n;

unsigned long long* array = new unsigned long long [n];

for (size_t i=0; i < n; i++) {
	std::cin >> array[i];
}

dst::lsd_sort (array, n);

for (size_t i=0; i < n; i++) {
	std::cout << array[i] << " ";
}
std::cout << "\n";

return 0;
}
