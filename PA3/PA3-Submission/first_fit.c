#include "allocator.h"

static int find_first_fit(int request_size){
    int current_addr = 0;
    number_of_examined_holes++;
    
    // Find the first hole!
    while(current_addr < n && memory[current_addr] >= 0){
        number_of_examined_holes++;
        current_addr += (memory[current_addr] // size
                        + 4/*metadata*/); // next block/hole
        if(current_addr >= n) // out of bound (changed > to >=)
            return -1; // request failed!
    }
    
    if(current_addr >= n) // Check if we went out of bounds
        return -1;
        
    int first_hole_addr = current_addr;
    
    // Find the first hole whose size is not less than request_size
    while(-memory[current_addr] < request_size + 5){
        // Leave at-least 5 bytes room for the remaining hole
        number_of_examined_holes++;
        int next_hole = memory[current_addr + 2]; // next hole
        
        // Check for invalid next hole pointer
        if(next_hole < 0 || next_hole >= n) {
            return -1; // request failed!
        }
        
        current_addr = next_hole;
        
        if(current_addr == first_hole_addr) // no hole is large enough
            return -1; // request failed!
    }
    return current_addr; // Address of the first hole whose size is appropriate!
}

static void print_all_blocks(int count){
    for(int i = 0; i < count; i++)
        printf("block[%d]: %s, %d, [%d, %d]\t",
               i, blocks[i].released?"released":"occupied",
               blocks[i].size,
               blocks[i].address,
               blocks[i].address + blocks[i].size + 3);
    putchar('\n');
}

static void print_all_holes(){
    int hole = 0, i = 0;
    int visited_holes = 0; // Safety counter to prevent infinite loops
    
    do{
        printf("hole#%d: %d, [%d, %d], P=%d, N=%d\t",
               i++, // index
               -memory[hole], // size
               hole, hole - memory[hole] + 3, // [from, to]
               memory[hole + PREV],
               memory[hole + NEXT]);
        hole = memory[hole + NEXT];
        visited_holes++;
        
        // Safety check to prevent infinite loops
        if(visited_holes > n/4) { // reasonable upper bound
            printf("\n[Warning: Too many holes, breaking to prevent infinite loop]\n");
            break;
        }
    }while(hole != 0 && hole > 0 && hole < n); // Added bounds checking
    putchar('\n');
}

void simulate_first_fit(){
    //printf("Simulating first-fit!\n");
    memory[0] = -(n-4);
    memory[n-1] = -(n-4);
    memory[1] = memory[2] = 0; // prev and next pointers
    int block_count = 0; // total # of blocks (released/occupied)
    int total_occupied_size = 0; // sum of blocks that are currently occupied.
    double amu = 0; // average memory utilization
    number_of_examined_holes = 0;
    int i = 0; // index of next_request
    
    for(int round = 0; round < x; round++){
        int requests_in_round = 0; // Safety counter
        
        while(1){
            // Safety check to prevent infinite allocation attempts
            if(requests_in_round > 100) { // reasonable limit
                //printf("Too many allocation attempts in round %d, breaking\n", round);
                break;
            }
            
            // Check if we have more requests available
            if(i >= next_request[i]) { // You need to define the size
                //printf("No more requests available\n");
                break;
            }
            
            //printf("Trying to satisfy request(%d)...", next_request[i]);
            int hole_addr = find_first_fit(next_request[i]);
            //printf("\thole address: %d\n", hole_addr);
            
            if(hole_addr < 0) // request failed!
                break;
                
            int block_size = next_request[i++];
            occupy(hole_addr, block_size, block_count++);
            total_occupied_size += block_size;
            requests_in_round++;
        }
        
        amu += (total_occupied_size/(double)n);
        //printf("current AMU: %.2f%%.\n", 100*amu/(round+1));
        
        // Check if there are any occupied blocks to release
        int occupied_count = 0;
        for(int j = 0; j < block_count; j++) {
            if(!blocks[j].released) {
                occupied_count++;
            }
        }
        
        if(occupied_count == 0) {
            //printf("No occupied blocks to release in round %d\n", round);
            continue;
        }
        
        int to_be_released_block;
        int attempts = 0;
        do{ // randomly choose an occupied block to release it
            to_be_released_block = rand() % block_count;
            attempts++;
            if(attempts > block_count * 2) { // Safety check
                //printf("Cannot find occupied block to release\n");
                break;
            }
        }while(blocks[to_be_released_block].released && attempts <= block_count * 2);
        
        if(attempts <= block_count * 2 && !blocks[to_be_released_block].released) {
            // printf("block#%d will be released.\n", to_be_released_block);
            // printf("Before release:\n");
            // print_all_blocks(block_count);
            // print_all_holes();
            release(to_be_released_block);
            total_occupied_size -= blocks[to_be_released_block].size;
            // printf("After release:\n");
            // print_all_blocks(block_count);
            // print_all_holes();
        }
    }
    
    printf("Average memory utilization for first fit is %.2f%%.\n",
           100 * amu/x);
    printf("Avg. number of examined holes in the first fit algorithm is %.2f per round.\n",
           number_of_examined_holes/(double)x);
}