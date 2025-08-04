#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define ALLOCATED_FRAMES 7
#define lrand() ((long)rand()*RAND_MAX + (long)rand())
#define STRING_LEN 1000000
//simulation parameters:
int e;//Small integer representing current locus to simulate spatial locality!
int m;//rate of motion - constant rate of moving locus forward!
int P;//The virtual memory of a process consists of P pages, numbered 0 through P −1.
double t;//probability of transitioning to a new locus
int locate(int* frames, int size, int page_no){
	
	for(int i = 0; i < size;i++)
		if(frames[i] == page_no)
			return i;
	return -1;
}
void ref_string_generator(int* ref_str){
	//initially locus: 0...e-1
  int locus_position = 0;//s
  int ref_count = 0;
  while(ref_count <= STRING_LEN){
    int next_ref = lrand()%e + locus_position;
    if(!ref_count || next_ref != ref_str[ref_count-1])//different from previous one
      ref_str[ref_count++] = next_ref;
    else//try again
      continue;
    if(ref_count%m == 0){//let's displace the locus
      if(rand() < t * RAND_MAX)//jump
        locus_position = lrand()%(P-e+1);
      else
        locus_position = (locus_position + 1)%(P-e+1);
    }
  }
  
}
int handle_clas(int argc, char* argv[]){
	while(*++argv){
    //./main -P 1048576 -e 10 -m 200 -t 2000 (Scenario #1)
		if(**argv != '-')
		  return 1;
		switch((*argv)[1]){
		  case 'P':
		  P = atoi(*++argv);
		  break;
		  case 'e':
		  e = atoi(*++argv);
		  break;
		  case 'm':
		  m = atoi(*++argv);
		  break;
		  case 't'://inverse of transition probability
		  t = 1.0/atoi(*++argv);
		  break;
		  default: //error
		  return 1;
		}
	}
}
int optimal(int* ref_str, int size, int limit){
  //size is the # of frames allocated for the process
  //limit is the max # of cells that we look ahead in the RS to implement the optimal algorithm
  //initialize
	int page_faults = 0;
	int frames[size], i, cur, page_no, frame_no;
	for(i = 0; i < size;i++)
		frames[i] = -1;//empty
	//PURE demand paging means all frames are empty!
	//Let's fill them out with the first few numbers in the reference string!
	for(i = 0, cur = 0; i < size;i++,cur++){
		page_no = ref_str[cur];//get the next vpn from reference string
		frame_no = locate(frames, size, page_no);//check if it is in the memory!
		if(frame_no >= 0)//already exists in main memory, duplicate!
			i--;
		else{//page-fault, swap it in to the next available position!
			frames[i] = page_no;
			page_faults++;
		}
	}
	//main loop... now ALL frames are full - any page-fault casues page-replacement!
	for(;cur < STRING_LEN;cur++){
		page_no = ref_str[cur];
		frame_no = locate(frames, size, page_no);
		if(frame_no >= 0)//already exists, no page-fault, no need for replacement!
			continue;
		//look at ref_str[cur+1:cur+limit] to see which one has not been referred to the longest in the future
		page_faults++;
		//This is the very first line that is differnet in other replacement algorithms
		unsigned unused = (1<<size) - 1;//binary 1111111: all bits are one!
		int victim;//frame index that must be swapped out of memory!
		for(int k = 1; k <= limit && unused && cur + k < STRING_LEN;k++){
			victim = locate(frames, size, ref_str[cur + k]);
			if(victim >= 0)// in the memory
				unused &= ~(1<<victim);//set the unused bit for victim to zero
		}
		if(!unused)//ALL frames will be refered to in the future... 
			frames[victim] = page_no;//victim is the one that is referred the latest!
		else{//At-least one frame won't be referred to in the future!
			victim = 0;
			while(unused % 2 == 0)//find the non-zero bit to find out the useless frame!
				unused = unused >> 1, victim++;
			frames[victim] = page_no;//useless frame is the victim!
		}    
	} 
	return page_faults;
}
int fifo(int* ref_str, int size){
    //size is the # of frames allocated for the process
    //initialize
    int page_faults = 0;
    int frames[size], i, cur, page_no, frame_no;
    int fifo_index = 0; // Index for FIFO replacement
    for(i = 0; i < size;i++)
        frames[i] = -1;//empty
    //PURE demand paging means all frames are empty!
    //Let's fill them out with the first few numbers in the reference string!
    for(i = 0, cur = 0; i < size;i++,cur++){
        page_no = ref_str[cur];//get the next vpn from reference string
        frame_no = locate(frames, size, page_no);//check if it is in the memory!
        if(frame_no >= 0)//already exists in main memory, duplicate!
            i--;
        else{//page-fault, swap it in to the next available position!
            frames[i] = page_no;
            page_faults++;
        }
    }
    //main loop... now ALL frames are full - any page-fault casues page-replacement!
    for(;cur < STRING_LEN;cur++){
        page_no = ref_str[cur];
        frame_no = locate(frames, size, page_no);
        if(frame_no >= 0)//already exists, no page-fault, no need for replacement!
            continue;
        //FIFO replacement: replace the oldest frame
        page_faults++;
        frames[fifo_index] = page_no;
        fifo_index = (fifo_index + 1) % size; // Move to next frame in circular fashion
    }
    return page_faults;
}

typedef struct {
    int page_no;
    int second_chance;
} Frame;

int second_chance(int* ref_str, int size, int length) {
    int page_faults = 0;
    int replacement_index = 0;
    Frame frames[size];

    for (int i = 0; i < size; i++) {
        frames[i].page_no = -1;
        frames[i].second_chance = 0;
    }

    for (int cur = 0; cur < length; cur++) {
        int page_no = ref_str[cur];
        int found = -1;
        for (int i = 0; i < size; i++) {
            if (frames[i].page_no == page_no) {
                found = i;
                break;
            }
        }

        if (found != -1) {
            frames[found].second_chance = 1;
            continue;
        }

        page_faults++;

        while (1) {
            if (frames[replacement_index].page_no == -1) {
                frames[replacement_index].page_no = page_no;
                frames[replacement_index].second_chance = 1;
                replacement_index = (replacement_index + 1) % size;
                break;
            } else if (frames[replacement_index].second_chance == 0) {
                frames[replacement_index].page_no = page_no;
                frames[replacement_index].second_chance = 1;
                replacement_index = (replacement_index + 1) % size;
                break;
            } else {
                frames[replacement_index].second_chance = 0;
                replacement_index = (replacement_index + 1) % size;
            }
        }
    }
    return page_faults;
}


int lru(int* ref_str, int size) {
    int frames[size];
    int last_used[size];
    int i, cur, page_no, frame_no;
    int page_faults = 0;

    for (i = 0; i < size; i++) {
        frames[i] = -1;
        last_used[i] = -1;
    }

    for (cur = 0; cur < STRING_LEN; cur++) {
        page_no = ref_str[cur];
        frame_no = locate(frames, size, page_no);

        if (frame_no >= 0) {
            last_used[frame_no] = cur; // Update the last used time
        } else {
            page_faults++;
            // Find an empty frame or least recently used
            int lru_index = -1, lru_time = STRING_LEN + 1;
            for (i = 0; i < size; i++) {
                if (frames[i] == -1) {
                    lru_index = i;
                    break;
                }
                if (last_used[i] < lru_time) {
                    lru_time = last_used[i];
                    lru_index = i;
                }
            }
            frames[lru_index] = page_no;
            last_used[lru_index] = cur;
        }
    }
    return page_faults;
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	if(handle_clas(argc, argv)){
		return 1;
		fprintf(stderr, "Fatal error: CLAs aren't passed correctly.\n");
	}
	else
		fprintf(stderr, "CLAs are P = %d, locus size = %d, m = %d, transition prob. = %f\n", P, e, m, t);
	int*ref_str = (int*) malloc(STRING_LEN*sizeof(int));
	ref_string_generator(ref_str);
	int optimal_page_fault = optimal(ref_str, ALLOCATED_FRAMES, e * m);
	printf("Optimal page replacement causes %d page faults\n", optimal_page_fault);
		int fifo_page_fault = fifo(ref_str, ALLOCATED_FRAMES);
	printf("FIFO page replacement causes %d page faults\n", fifo_page_fault);
			int second_chance_page_fault = second_chance(ref_str, ALLOCATED_FRAMES, e * m);
	printf("Second chance page replacement causes %d page faults\n", second_chance_page_fault);
				int lru_page_fault = lru(ref_str, ALLOCATED_FRAMES, e * m);
	printf("LRU chance page replacement causes %d page faults\n", lru_page_fault);
	//do the same for other algorithms!
	return 0;
}