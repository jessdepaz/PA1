
#include "allocator.h"

static int find_best_fit(int request_size) {
	int best_fit_addr = -1;
	int smallest_fit_size = MAX_N;
	int start_addr = 0;
	number_of_examined_holes++;

	// Locate the first hole
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

static void print_all_blocks(int count) {
	for (int i = 0; i < count; i++) {
		printf("block[%d]: %s, %d, [%d, %d]\t",
			i, blocks[i].released ? "released" : "occupied",
			blocks[i].size,
			blocks[i].address,
			blocks[i].address + blocks[i].size + 3);
	}
	putchar('\n');
}

static void print_all_holes() {
	int hole = 0, i = 0;
	do {
		printf("hole#%d: %d, [%d, %d], P=%d, N=%d\t",
			i++, -memory[hole], hole,
			hole - memory[hole] + 3,
			memory[hole + PREV],
			memory[hole + NEXT]);
		hole = memory[hole + NEXT];
	} while (hole);
	putchar('\n');
}

void simulate_best_fit() {
	printf("Simulating best-fit!\n");

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
			printf("Trying to satisfy request(%d)...", next_request[i]);
			int hole_addr = find_best_fit(next_request[i]);
			printf("\thole address: %d\n", hole_addr);
			if (hole_addr < 0) break;

			int block_size = next_request[i++];
			occupy(hole_addr, block_size, block_count++);
			total_occupied_size += block_size;
		}
		amu += (total_occupied_size / (double)n);
		printf("current AMU: %.2f%%.\n", 100 * amu / (round + 1));

		int to_be_released_block;
		do {
			to_be_released_block = rand() % block_count;
		} while (blocks[to_be_released_block].released);

		printf("block#%d will be released.\n", to_be_released_block);
		printf("Before release:\n");
		print_all_blocks(block_count);
		print_all_holes();

		release(to_be_released_block);
		total_occupied_size -= blocks[to_be_released_block].size;

		printf("After release:\n");
		print_all_blocks(block_count);
		print_all_holes();
	}
	printf("Average memory utilization for best fit is %.2f%%.\n",
		100 * amu / x);
	printf("Avg. number of examined holes in the best fit algorithm is %.2f per round.\n",
		number_of_examined_holes / (double)x);
}
