#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char ** argv){
	
	int i;
	int N = 1000;
	int hits = 0;
	double area = 0;
	struct drand48_data * rngs;

	rngs = (struct drand48_data 
*)malloc(sizeof(drand48_data)*omp_get_num_threads());

#pragma omp parallel
{
	srand48_r(omp_get_thread_num(),&(rngs[omp_get_thread_num()]))
}

#pragma omp parallel for reduction(+:hits)
	for(i=0;i<N;i++){
		int rank = omp_get_thread_num();
		double x; double y;
		drand48_r(&(rngs[rank]),&x);
		drand48_r(&(rngs[rank]),&y);
		if(x*x+y*y<1){
			hits += 1;
		}
	}
	printf("PI = %lf\n",((double)hits/(double)N*4.0));
}
