#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
// compile using gcc -pthread -o term term.c

struct params{
    int* points_inside_ptr;
    int total_points;
    int radius;
};


void run_simulation(int , int);
int is_point_in_circle(float, float, int);
void* generate_points(void*);


int main() {
    int combinations = 4;
    int total_points[] = {20000, 100000, 1000000, 10000000};
    int threads[] = {2, 4, 6, 8};

    for(int i = 0; i < combinations; i++){
        run_simulation(total_points[i], threads[i]);
    }

    return 0;
}


void run_simulation(int total_points, int threads){
    clock_t begin = clock();

    int radius = 1;
    int a = 2*radius;

    // Create threads
    pthread_t* ids = malloc(threads * sizeof(pthread_t));
    int points_inside[threads];
    struct params all_params[threads];

    // Fill in the params
    for(int i = 0; i < threads; i++){
        all_params[i].points_inside_ptr = &points_inside[i];
        all_params[i].total_points = (int)total_points/threads;
        all_params[i].radius = radius;
        pthread_create(&ids[i], NULL, &generate_points, (void*)&all_params[i]); 
    }

    for(int i = 0; i < threads; i++){
        pthread_join(ids[i], NULL);
    }

    // Free memory
    free(ids);
    
    int inside = 0;
    for(int i = 0; i < threads; i++){
        inside += (points_inside[i]);
    }

    clock_t end = clock();
    double time_taken = (double)(end - begin) / CLOCKS_PER_SEC;

    // Approximate pi
    float pi = 4.0 * ((float)inside/(float)total_points);
    printf("Threads: %d\nTotal points: %d\nPoints inside: %d\nTime taken: %f\nPi approximation: %f\n\n", threads, total_points, inside, time_taken, pi);
}


void* generate_points(void* params){
    struct params p = (*(struct params*)params);
    int points_inside = 0;
    float x = 0.0;
    float y = 0.0;

    for(int i = 0; i < p.total_points; i++){
        x = ((float)rand()/(float)(RAND_MAX/2))-1.0;
        y = ((float)rand()/(float)(RAND_MAX/2))-1.0;
        if(is_point_in_circle(x, y, p.radius) == 1){
            points_inside++;
        }
    }
    
    *(p.points_inside_ptr) = points_inside;
}


int is_point_in_circle(float x, float y, int r){
    if ((x*x)+ (y*y) <= (r*r)){
        return 1;
    }
    return 0;
}