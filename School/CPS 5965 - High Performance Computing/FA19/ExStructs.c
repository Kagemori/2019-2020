#include <stdio.h>

typedef struct {
	double x;
	double y;
} Point;

double Point_Length(Point * p1){
	return sqrt(p1->x*p1->x+p1->y*p1->y);
}

int main(int argc, char ** argv){
	
	Point position;
	
	position.x = 1.0;
	position.y = 2.0;
	
	printf("Length = %lf\n",Point_Length(&position));
}