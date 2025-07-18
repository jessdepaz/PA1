#include "allocator.h"
static int find_first_fit(int request_size){
	int current_addr = 0;
	number_of_examined_holes++;
	//find the first hole!
	while(memory[current_addr] >= 0){
		number_of_examined_holes++;
		current_addr += (memory[current_addr]//size
						+ 4/*metadata*/); //next block/hole
		if(current_addr > n-1)//out of bound
			return -1;//request failed!
	}
	int first_hole_addr = current_addr;
	//find the first hole whose size is not less than request_size
	while(-memory[current_addr] < request_size + 5){
		//Leave at-least 5 bytes room for the remaining hole
		number_of_examined_holes++;
		current_addr = memory[current_addr+2];//next hole
		if(current_addr == first_hole_addr)//no hole is large enough
			return -1;//request failed!
	}
	return current_addr;//Address of the first hole whose size is appropriate!
}
static void print_all_blocks(int count){
	for(int i = 0; i < count;i++)
		printf("block[%d]: %s, %d, [%d, %d]\t",
			i, blocks[i].released?"released":"occupied", 
			blocks[i].size, 
			blocks[i].address,
			blocks[i].address + blocks[i].size + 3);
	putchar('\n');		
}
static void print_all_holes(){
	int hole = 0, i = 0;
	do{
		printf("hole#%d: %d, [%d, %d], P=%d, N=%d\t",
			i++, //index
			-memory[hole], //size
			hole, hole - memory[hole] + 3,//[from, to]
			memory[hole + PREV],
			memory[hole + NEXT]);
		hole = memory[hole + NEXT];
	}while(hole);
	putchar('\n');
}
void simulate_first_fit(){
	printf("Simulating first-fit!\n");
	memory[0] = -(n-4);
	memory[n-1] = -(n-4);
	memory[1] = memory[2] = 0;//prev and next pointers
	int block_count = 0;//total # of blocks (released/occupied)
	int total_occupied_size = 0;//sum of blocks that are currently occupied.
	double amu = 0;//average memory utilization
	number_of_examined_holes = 0;
	int i = 0;//index of next_request
	for(int round = 0; round < x; round++){
		while(1){
			printf("Trying to satisfy request(%d)...", next_request[i]);
			int hole_addr = find_first_fit(next_request[i]);
			printf("\thole address: %d\n", hole_addr);
			if(hole_addr < 0)//request failed!
				break;
			int block_size = next_request[i++];
			occupy(hole_addr, block_size, block_count++);
			total_occupied_size += block_size;
		}
		amu += (total_occupied_size/(double)n);
		printf("current AMU: %.2f%%.\n", 100*amu/(round+1));
		int to_be_released_block;
		do{//randomly choose an occupied block to release it
			to_be_released_block = rand() % block_count;
		}while(blocks[to_be_released_block].released);
		printf("block#%d will be released.\n", to_be_released_block);
		printf("Before	 release:\n");
		print_all_blocks(block_count);
		print_all_holes();
		release(to_be_released_block);
		total_occupied_size -= blocks[to_be_released_block].size;
		printf("After release:\n");
		print_all_blocks(block_count);
		print_all_holes();
	}
	printf("Average memory utilization for first fit is %.2f%%.\n", 
		100 * amu/x);
	printf("Avg. number of examined holes in the first fit algorithm is %.2f per round.\n", 
		number_of_examined_holes/(double)x);
}