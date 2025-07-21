#include "allocator.h"

static int find_best_fit(int request_size) {
	int best_fit_addr = -1;
	int smallest_fit_size = MAX_N;
	int start_addr = 0;
	number_of_examined_holes++;

	while (memory[start_addr] >= 0) {
		number_of_examined_holes++;
		start_addr += memory[start_addr] + 4;
		if (start_addr >= n) return -1;
	}
	int first_hole_addr = start_addr;

	do {
		int hole_size = -memory[start_addr];
		number_of_examined_holes++;

		if (hole_size >= request_size + 4 && hole_size < smallest_fit_size) {
			best_fit_addr = start_addr;
			smallest_fit_size = hole_size;
		}
		start_addr = memory[start_addr + NEXT];
	} while (start_addr != 0 && start_addr != first_hole_addr);

	return best_fit_addr;
}

void simulate_best_fit() {
	memory[0] = -(n - 4);
	memory[n - 1] = -(n - 4);
	memory[1] = memory[2] = 0;

	int block_count = 0;
	int total_occupied_size = 0;
	double amu = 0;
	number_of_examined_holes = 0;
	int i = 0;

	for (int round = 0; round < x; round++) {
		while (1) {
			int hole_addr = find_best_fit(next_request[i]);
			if (hole_addr < 0) break;

			int block_size = next_request[i++];
			occupy(hole_addr, block_size, block_count++);
			total_occupied_size += block_size;
		}
		amu += (total_occupied_size / (double)n);

		int to_be_released_block;
		do {
			to_be_released_block = rand() % block_count;
		} while (blocks[to_be_released_block].released);

		release(to_be_released_block);
		total_occupied_size -= blocks[to_be_released_block].size;
	}

	printf("Best Fit:\n");
	printf("Average Memory Utilization: %.2f%%\n", 100 * amu / x);
	printf("Average Examined Holes per Round: %.2f\n", number_of_examined_holes / (double)x);
}