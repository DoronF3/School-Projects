#include "osm.h"
#include <sys/time.h>
#include <iostream>

#define SECOND_TO_NANO 1000000000.0
#define MICRO_TO_NANO 1000.0


void empty_func()
{

}

/* Time measurement function for a simple arithmetic operation.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_operation_time(unsigned int iterations)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int a=0;
    for (unsigned int i = 0; i < iterations / 4; ++i)
    {
        a=a+1;
        a=a+1;
        a=a+1;
        a=a+1;
    }
    gettimeofday(&end, NULL);
    double result =
            ((end.tv_sec - start.tv_sec) * SECOND_TO_NANO + (end.tv_usec - start.tv_usec) * MICRO_TO_NANO) / iterations;
    return result;
}

double osm_function_time(unsigned int iterations)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (unsigned int i = 0; i < iterations / 4; ++i)
    {
        empty_func();
        empty_func();
        empty_func();
        empty_func();
    }
    gettimeofday(&end, NULL);
    double result =
            ((end.tv_sec - start.tv_sec) * SECOND_TO_NANO + (end.tv_usec - start.tv_usec) * MICRO_TO_NANO) / iterations;
    return result;

}

/* Time measurement function for an empty trap into the operating system.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_syscall_time(unsigned int iterations)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (unsigned int i = 0; i < iterations / 4; ++i)
    {
        OSM_NULLSYSCALL;
        OSM_NULLSYSCALL;
        OSM_NULLSYSCALL;
        OSM_NULLSYSCALL;
    }
    gettimeofday(&end, NULL);
    double result =
            ((end.tv_sec - start.tv_sec) * SECOND_TO_NANO + (end.tv_usec - start.tv_usec) * MICRO_TO_NANO) / iterations;
    return result;
}