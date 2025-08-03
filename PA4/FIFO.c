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
    int fifo_page_fault = fifo(ref_str, ALLOCATED_FRAMES);
    printf("FIFO page replacement causes %d page faults\n", fifo_page_fault);
    //do the same for other algorithms!
    return 0;
}