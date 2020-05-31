/* fire.c
 */
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "fire_serial.h"
#include <mpi.h>


int main(int argc, char ** argv) {
    // initial conditions and variable definitions
    int forest_size=200;
    double prob_spread;
    double prob_min=0.0;
    double prob_max=1.0;
    double prob_step;
    int **forest;
    double percent_burned;
    int i_trial;
    int n_trials=600;
    int i_prob;
    int n_probs=128;
    double * per_burns;
	double * per_burns_buffer;
	MPI_Status status;
	int size,rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
    // setup problem, allocate memory
    seed_by_time(0);
    forest=allocate_forest(forest_size);
    per_burns = (double *)malloc(sizeof(double)*n_probs+1);
	per_burns_buffer = (double *)malloc(sizeof(double)*n_probs+1);
	for (i_prob=0;i_prob<n_probs;i_prob++) per_burns[i_prob]=0.0;


    // for a number of probabilities, calculate
    // average burn and output
    prob_step = (prob_max-prob_min)/(double)(n_probs-1);
    for (i_prob = rank ; i_prob < n_probs; i_prob+=size) {
        //for a number of trials, calculate average
        //percent burn
        prob_spread = prob_min + (double)i_prob * prob_step;
        percent_burned=0.0;
        for (i_trial=0; i_trial < n_trials; i_trial++) {
            //burn until fire is gone
            burn_until_out(forest_size,forest,prob_spread,10,10);
            percent_burned+=get_percent_burned(forest_size,forest);
        }
        percent_burned/=n_trials;
        per_burns[i_prob]=percent_burned;

    }
	MPI_Reduce(per_burns,per_burns_buffer,n_trials,MPI_DOUBLE,
	    MPI_SUM,0,MPI_COMM_WORLD);

    // print output
    printf("Probability of fire spreading, Average percent burned\n");
    for (i_prob =0 ; i_prob<n_probs; i_prob++) {
        prob_spread = prob_min + (double)i_prob * prob_step;
        printf("%lf , %lf\n",prob_spread,per_burns_buffer[i_prob]);
    }

    // clean up
    delete_forest(forest_size,forest);
    free(per_burns);
	MPI_Finalize(); 
    return 0;

}

void seed_by_time(int offset) {
    time_t the_time;
    time(&the_time);
    srand((int)the_time+offset);
}

void burn_until_out(int forest_size,int ** forest, double prob_spread,
    int start_i, int start_j) {

    initialize_forest(forest_size,forest);
    light_tree(forest,start_i,start_j);

    // burn until fire is gone
    while(forest_is_burning(forest_size,forest)) {
        forest_burns(forest_size,forest,prob_spread);
    }
}

double get_percent_burned(int forest_size,int ** forest) {
    int i,j;
    int total = forest_size*forest_size-1;
    int sum=0;

    // calculate pecrent burned
    for (i=0;i<forest_size;i++) {
        for (j=0;j<forest_size;j++) {
            if (forest[i][j]==BURNT) {
                sum++;
            }
        }
    }

    // return percent burned;
    return ((double)(sum-1)/(double)total);
}


int ** allocate_forest(int forest_size) {
    int i;
    int ** forest;

    forest = (int **) malloc (sizeof(int*)*forest_size);
    for (i=0;i<forest_size;i++) {
        forest[i] = (int *) malloc (sizeof(int)*forest_size);
    }

    return forest;
}

void initialize_forest(int forest_size, int ** forest) {
    int i,j;

    for (i=0;i<forest_size;i++) {
        for (j=0;j<forest_size;j++) {
            forest[i][j]=UNBURNT;
        }
    }
}

void delete_forest(int forest_size, int ** forest) {
    int i;

    for (i=0;i<forest_size;i++) {
        free(forest[i]);
    }
    free(forest);
}

void light_tree(int ** forest, int i, int j) {
    forest[i][j]=SMOLDERING;
}

boolean fire_spreads(double prob_spread) {
    if ((double)rand()/(double)RAND_MAX < prob_spread) 
        return true;
    else
        return false;
}

void forest_burns(int forest_size, int **forest,double prob_spread) {
    int i,j;

    //burning trees burn down, smoldering trees ignite
    for (i=0; i<forest_size; i++) {
        for (j=0;j<forest_size;j++) {
            if (forest[i][j]==BURNING) forest[i][j]=BURNT;
            if (forest[i][j]==SMOLDERING) forest[i][j]=BURNING;
        }
    }

    //unburnt trees catch fire
    for (i=0; i<forest_size; i++) {
        for (j=0;j<forest_size;j++) {
            if (forest[i][j]==BURNING) {
                if (i!=0) { // North
                    if (fire_spreads(prob_spread)&&forest[i-1][j]==UNBURNT) {
                        forest[i-1][j]=SMOLDERING;
                    }
                }
                if (i!=forest_size-1) { //South
                    if (fire_spreads(prob_spread)&&forest[i+1][j]==UNBURNT) {
                        forest[i+1][j]=SMOLDERING;
                    }
                }
                if (j!=0) { // West
                    if (fire_spreads(prob_spread)&&forest[i][j-1]==UNBURNT) {
                        forest[i][j-1]=SMOLDERING;
                    }
                }
                if (j!=forest_size-1) { // East
                    if (fire_spreads(prob_spread)&&forest[i][j+1]==UNBURNT) {
                        forest[i][j+1]=SMOLDERING;
                    }
                }
            }
        }
    }
}

boolean forest_is_burning(int forest_size, int ** forest) {
    int i,j;

    for (i=0; i<forest_size; i++) {
        for (j=0; j<forest_size; j++) {
            if (forest[i][j]==SMOLDERING||forest[i][j]==BURNING) {
                return true;
            }
        }
    }
    return false;
}

void print_forest(int forest_size,int ** forest) {
    int i,j;

    for (i=0;i<forest_size;i++) {
        for (j=0;j<forest_size;j++) {
            if (forest[i][j]==BURNT) {
                printf(".");
            } else {
                printf("X");
            }
        }
        printf("\n");
    }
}
