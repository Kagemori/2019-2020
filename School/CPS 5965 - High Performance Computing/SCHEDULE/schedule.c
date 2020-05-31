#include <stdio.h>
#include <omp.h>

int main(int argc, char ** argv) {

    int i;
    int n=64;

#pragma omp parallel for schedule(static)
    for(i=0;i<n;i++) {
        printf("%d %d\n",i,omp_get_thread_num());
    }
}
