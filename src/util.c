#include "util.h"

void utils_init()
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    clock_freq = 1.0 / (double)freq.QuadPart;
    QueryPerformanceCounter(&start_time);
}

double get_absolute_time()
{
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (double)now_time.QuadPart * clock_freq;
}