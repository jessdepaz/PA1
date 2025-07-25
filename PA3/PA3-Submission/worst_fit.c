#include "allocator.h"

static int find_worst_fit(int request_size){
    int current_addr = 0;
    number_of_examined_holes++;
    //find the first hole!
    while(memory[current_addr] >= 0){
        number_of_examined_holes++;
        current_addr += (memory[current_addr] + 4); // Fixed: removed //size comment
        if(current_addr > n-1)//out of bound
            return -1;//request failed!
    }
    int first_hole_addr = current_addr;
    int worst_fit_addr = -1;
    int worst_fit_size = -1;
    int iterations = 0; // Safety counter to prevent infinite loops
    do{
        if(++iterations > n) break; // Safety check
        int hole_size = -memory[current_addr];
        if (hole_size >= request_size + 5 && hole_size > worst_fit_size) {
            worst_fit_size = hole_size;
            worst_fit_addr = current_addr;
        }
        number_of_examined_holes++;
        current_addr = memory[current_addr + NEXT];
    }
    while (current_addr != first_hole_addr && current_addr != 0);
    return worst_fit_addr;
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
    int iterations = 0; // Safety counter
    do{
        if(++iterations > n) break; // Safety check
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

void simulate_worst_fit(){
    //printf("Simulating worst-fit!\n");
    memory[0] = -(n-4);
    memory[n-1] = -(n-4);
    memory[1] = memory[2] = 0;//prev and next pointers
    int block_count = 0;//total # of blocks (released/occupied)
    int total_occupied_size = 0;//sum of blocks that are currently occupied.
    double amu = 0;//average memory utilization
    number_of_examined_holes = 0;
    int i = 0;//index of next_request
    for(int round = 0; round < x; round++){
        int requests_this_round = 0; // Safety counter
        while(1){
            if(++requests_this_round > n) break; // Safety check to prevent infinite allocation
            //printf("Trying to satisfy request(%d)...", next_request[i]);
            int hole_addr = find_worst_fit(next_request[i]);
            //printf("\thole address: %d\n", hole_addr);
            if(hole_addr < 0)//request failed!
                break;
            int block_size = next_request[i++];
            occupy(hole_addr, block_size, block_count++);
            total_occupied_size += block_size;
        }
        amu += (total_occupied_size/(double)n);
        //printf("current AMU: %.2f%%.\n", 100*amu/(round+1));
        int to_be_released_block;
        int attempts = 0; // Safety counter
        do{//randomly choose an occupied block to release it
            to_be_released_block = rand() % block_count;
            if(++attempts > block_count * 2) break; // Safety check
        }while(attempts <= block_count * 2 && blocks[to_be_released_block].released);
        //printf("block#%d will be released.\n", to_be_released_block);
        //printf("Before release:\n");
        //print_all_blocks(block_count);
        //print_all_holes();
        if(attempts <= block_count * 2) { // Only release if we found a valid block
            release(to_be_released_block);
            total_occupied_size -= blocks[to_be_released_block].size;
        }
        //printf("After release:\n");
        //print_all_blocks(block_count);
        //print_all_holes();
    }
    printf("Average memory utilization for worst fit is %.2f%%.\n",
        100 * amu/x);
    printf("Avg. number of examined holes in the worst fit algorithm is %.2f per round.\n",
        number_of_examined_holes/(double)x);
}