#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define frand() (rand()/(double)RAND_MAX)
#define nrand() (sqrt(-2*log(frand()))*cos(2*M_PI*frand()))

int main()
{
    srand(time(NULL));
    double m, M,mu,sigma;
    printf("Enter the min and max values m,M:");
    scanf("%lf,%lf", &m, &M);
    //printf("Uniformly distributed random number between %.2f and %.2f is %.5f\n", m, M, m + (M-m)*frand());
    printf("Enter the mu and sigma values mu,sig:");
    scanf("%lf,%lf", &mu, &sigma);
    printf("Normally distributed random number of mean %.2f and std %.2f is %.5f\n", mu, sigma, nrand()*sigma + mu);
	int n = 20;
	//Sequence u is 	uniformly distributed from m to M
	double u[n];
	for(int i = 0; i < n;i++)
		u[i] = m + (M-m)*frand();
	
	//Sequence v is normally distributed with mean = mu and std = sigma
	double v[n];
	for(int i = 0; i < n;i++)
		v[i] = mu + sigma* nrand();
	
	//Sequence t is normally distributed with mean = mu and std = sigma, 
	//and it is truncated in the range [m,M)
	
	double t[n];
	for(int i = 0; i < n;i++){
		v[i] = mu + sigma* nrand();
		if(v[i] < m || v[i] >= M){
			i--;
			continue;
		}
	}
	
	
	//Sequence v/t
	double v_over_t[n];
	for(int i = 0; i < n;i++)
		v_over_t[i] = v[i] / t[i];
	
	
	
	
    return 0;
}