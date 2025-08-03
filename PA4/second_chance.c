#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define ALLOCATED_FRAMES 7
#define lrand() ((long)rand() * RAND_MAX + (long)rand())
#define STRING_LEN 1000000
// simulation parameters: bhuu
int e;	  // Small integer representing current locus to simulate spatial locality!
int m;	  // rate of motion - constant rate of moving locus forward!
int P;	  // The virtual memory of a process consists of P pages, numbered 0 through P −1.
double t; // probability of transitioning to a new locus

typedef struct
{
	int page_no;
	int second_chance;
} Frame;

int locate(Frame *frames, int size, int page_no)
{
	for (int i = 0; i < size; i++)
		if (frames[i].page_no == page_no)
			return i;
	return -1;
}

void ref_string_generator(int *ref_str)
{
	int locus_position = 0;
	int ref_count = 0;

	while (ref_count < STRING_LEN)
	{
		int next_ref = lrand() % e + locus_position;

		if (!ref_count || next_ref != ref_str[ref_count - 1]) // different from previous one
			ref_str[ref_count++] = next_ref;

		if (ref_count % m == 0)
		{ // let's displace the locus
			if (rand() < t * RAND_MAX) // jump
				locus_position = lrand() % (P - e + 1);
			else
				locus_position = (locus_position + 1) % (P - e + 1);
		}
	}
}

int handle_clas(int argc, char *argv[])
{
	while (*++argv)
	{
		//./main -P 1048576 -e 10 -m 200 -t 2000 (Scenario #1)
		if (**argv != '-')
			return 1;
		switch ((*argv)[1])
		{
		case 'P':
			P = atoi(*++argv);
			break;
		case 'e':
			e = atoi(*++argv);
			break;
		case 'm':
			m = atoi(*++argv);
			break;
		case 't': // inverse of transition probability
			t = 1.0 / atoi(*++argv);
			break;
		default: // error
			return 1;
		}
	}
	return 0;
}

int second_chance(Frame* frames, int size, int *ref_str)
{
	int page_faults = 0;
	int replacement_index = 0;

	for (int i = 0; i < size; i++)
	{
		frames[i].page_no = -1;
		frames[i].second_chance = 0; 
	}
	
	for (int cur = 0; cur < STRING_LEN; cur++)
	{
		int page_no = ref_str[cur];
		int found = locate(frames, size, page_no);

		if (found != -1)
		{
			frames[found].second_chance = 1; 
			continue;
		}

		page_faults++;

		while (1) {
            if (frames[replacement_index].page_no == -1) {
                // Empty frame — use immediately
                frames[replacement_index].page_no = page_no;
                frames[replacement_index].second_chance = 1;
                replacement_index = (replacement_index + 1) % size;
                break;
            } else if (frames[replacement_index].second_chance == 0) {
                // Replace this one
                frames[replacement_index].page_no = page_no;
                frames[replacement_index].second_chance = 1;
                replacement_index = (replacement_index + 1) % size;
                break;
            } else {
                // Give second chance and move on
                frames[replacement_index].second_chance = 0;
                replacement_index = (replacement_index + 1) % size;
            }
        }
	}
	return page_faults;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	if (handle_clas(argc, argv))
	{
		fprintf(stderr, "Fatal error: CLAs aren't passed correctly.\n");
		return 1;
	}
	else{
		fprintf(stderr, "CLAs are P = %d, locus size = %d, m = %d, transition prob. = %f\n", P, e, m, t);
	}

	int *ref_str = (int *)malloc(STRING_LEN * sizeof(int));
	ref_string_generator(ref_str);

	Frame frames[ALLOCATED_FRAMES];
	int faults = second_chance(frames, ALLOCATED_FRAMES, ref_str);

	printf("Second chance page replacement causes %d page faults\n", faults);
	
	return 0;
}