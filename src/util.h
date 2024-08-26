#include <Windows.h>

static double clock_freq;
static LARGE_INTEGER start_time;

void utils_init();

double get_absolute_time();