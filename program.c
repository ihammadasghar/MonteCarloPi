// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>

int point_in_circle(float, float, int);

int main() {
    int r = 1;
    int a = 2*r;
    int total_points = 10000000;
    int points_inside = 0;
    float x = 0.0;
    float y = 0.0;

    for(int i = 0; i < total_points; i++){
        x = ((float)rand()/(float)(RAND_MAX/2))-1.0;
        y = ((float)rand()/(float)(RAND_MAX/2))-1.0;
        if(point_in_circle(x, y, r) == 1){
            points_inside++;
        }
    }

    float pi = 4.0 * ((float)points_inside/(float)total_points);

    printf("Our pi approximation after %d is %f\n", total_points, pi);
    
    return 0;
}

int point_in_circle(float x, float y, int r){
    if ((x*x)+ (y*y) <= (r*r)){
        return 1;
    }
    return 0;
}