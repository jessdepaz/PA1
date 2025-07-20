#include "allocator.h"

static int last_allocated_addr = 0; // Static variable to remember last allocation

static int find_next_fit(int request_size){
    int current_addr = last_allocated_addr; // Start from last allocated position
    int start_addr = current_addr; // Remember where we started to detect loop
    number_of_examined_holes++;

    // Find the first hole starting from last_allocated_addr
    while(memory[current_addr] >= 0){
        number_of_examined_holes++;
        current_addr += (memory[current_addr]
            + 4);
        if(current_addr > n-1) { // wrap around to beginning
            current_addr = 0;
            if(current_addr == start_addr) // full loop without finding hole
                return -1; 
        }
        if(current_addr == start_addr) // back to starting point
            return -1;
    }

    int first_hole_addr = current_addr;
    // Find the first hole whose size is not less than request_size
    while(-memory[current_addr] < request_size + 5){
        // Leave at-least 5 bytes room for the remaining hole
        number_of_examined_holes++;
        current_addr = memory[current_addr+2];
        if(current_addr == first_hole_addr) 
            return -1; 
    }

    last_allocated_addr = current_addr; // Update last allocated position
    return current_addr; // Address of the first hole whose size is appropriate
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
            i++,
            -memory[hole],
            hole, hole - memory[hole] + 3,
            memory[hole + PREV],
            memory[hole + NEXT]);
            hole = memory[hole + NEXT];
    }while(hole);
    putchar('\n');
}

void simulate_next_fit(){
    printf("Simulating next-fit!\n");
    memory[0] = -(n-4);
    memory[n-1] = -(n-4);
    memory[1] = memory[2] = 0;
    last_allocated_addr = 0;
    int block_count = 0;
    int total_occupied_size = 0;
    double amu = 0;
    number_of_examined_holes = 0;
    int i = 0;
    for(int round = 0; round < x; round++){
        while(1){
            printf("Trying to satisfy request(%d)...", next_request[i]);
            int hole_addr = find_next_fit(next_request[i]);
            printf("\thole address: %d\n", hole_addr);
            if(hole_addr < 0)
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
        }
        while(blocks[to_be_released_block].released);
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
    printf("Average memory utilization for next fit is %.2f%%.\n",
        100 * amu/x);
    printf("Avg. number of examined holes in the next fit algorithm is %.2f per round.\n",
        number_of_examined_holes/(double)x);
}