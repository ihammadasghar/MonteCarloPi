// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct props{
    int radius;
    int total_points;
    int* points_inside;
};

int point_in_circle(float, float, int);
void* monte_carlo(void*);
void simulate(int, int, int);
float accuracy(float);

int main() {
    int radius = 1;
    int total_points[] = {20000, 100000, 1000000, 10000000};
    int threads[] = {2, 4, 6, 8};

    for(int i = 0; i < 4; i++){
        simulate(total_points[i], threads[i], radius);
    }

    
    
    return 0;
}

void simulate(int total_points, int threads, int radius){
    clock_t begin = clock();

    int a = 2*radius;

    pthread_t* ids = malloc(threads * sizeof(pthread_t));
    int points_inside[threads];
    struct props props[threads];


    for(int i = 0; i < threads; i++){
        props[i].points_inside = &points_inside[i];
        props[i].total_points = total_points/threads;
        props[i].radius = radius;
        pthread_create(&ids[i], NULL, &monte_carlo, (void*)&props[i]);
    }

    for(int i = 0; i < threads; i++){
        pthread_join(ids[i], NULL);
    }

    int sum = 0;
    for(int i = 0; i < threads; i++){
        sum += points_inside[i];
    }

    float pi = 4.0 * ((float)sum/(float)total_points);

    clock_t end = clock();
    float acc = accuracy(pi);
    double time_taken = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Total points: %d\nThreads: %d\nTime taken: %f\nApproximation of pi: %f with an accuracy of %f%%\n\n", total_points, threads, time_taken,pi, acc);
}

void* monte_carlo(void* props){
    struct props prop = *(struct props*)props;
    int total_points = prop.total_points;
    int r = prop.radius;
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
    *(prop.points_inside) = points_inside;
}

int point_in_circle(float x, float y, int r){
    if ((x*x)+ (y*y) <= (r*r)){
        return 1;
    }
    return 0;
}

float accuracy(float approx){
    float PI = 3.141592;
    float acc = (approx/PI)*100.00;
    if(acc > 100.00){
        float d = acc - (float)100;
        acc = (float)100 - d;
    }
    return acc;
}  