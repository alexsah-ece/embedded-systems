#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

void end_program();
void init(char**);
void run();
useconds_t get_sample();
void end_program();

int samples, duration;
useconds_t* intervals;
useconds_t dt;
struct timeval start;


int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stdout, "Not enough arguments given\n");
        return 0;
    }
    signal(SIGALRM, end_program);
    init(argv);
    run();
}

void init(char **argv) {
    duration = atoi(argv[1]);
    float interval = atof(argv[2]);
    int count = (useconds_t) (duration/interval);
    // allocate memory for storing the interval values
    intervals = (useconds_t*) malloc(count * sizeof(useconds_t));
    dt = (useconds_t) (atof(argv[2]) * pow(10, 6));
}

void run() {
    // starting time
    gettimeofday(&start, NULL);
    samples = 0;
    // set alarm to know when to exit the sampling procedure
    alarm(duration);
    while (1){
        usleep(dt);
        intervals[samples++] = get_sample();        
    }
}

useconds_t get_sample() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    useconds_t delta = (tv.tv_sec - start.tv_sec) * pow(10, 6) + tv.tv_usec - start.tv_usec;
    start = tv;
    return delta;
}

void end_program() {
    printf("Writing results to file...\n");
    FILE *fptr;
    fptr = fopen("../log/sampling_a.csv","w");
    if (fptr == NULL){
     printf("Something went wrong\n");
     _exit(1);
    }
    fprintf(fptr, "sample,interval,duration\n");
    float sum = 0;
    for(int j = 0; j < samples; j++) {
        sum += intervals[j] / pow(10, 6);
        fprintf(fptr, "%d,%d,%f\n", j, intervals[j], sum);
    }
    free(intervals);
    fclose(fptr);
    exit(0);
}