#include <stdio.h>
#include "FRect.h"
#include "util.h"
#define ENABLE_DEBUG

#define MAX_WINDOW_TITLE_LEN 64
/*
 * my plan is right
 * geta list of windows
 * map each one to a number value
 * have user input number of window
 * then validate selection so no segfault
 * then bounmce window around
*/
typedef struct WindowInfo {
  HWND handle;
  char* window_name;
} WindowInfo;

// Globals
unsigned int found_win_count = 0;
int max_win_count = 32;

long long get_milliseconds() {
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  LARGE_INTEGER counter_now;
  QueryPerformanceCounter(&counter_now);
  return (1000LL * counter_now.QuadPart) / freq.QuadPart;
}

// EnumWindows accepts a callback function to run on each window found:
BOOL window_callback(HWND hwnd, LPARAM lParam)
{
    WindowInfo* window_info_arr = (WindowInfo*)lParam;
    if (found_win_count >= max_win_count)
    {
        printf("TOO MANY WINDOWS \n");
        return FALSE;
    }

    if (!IsWindowVisible(hwnd))
    {
        return TRUE;
    }

    char* window_name = malloc(MAX_WINDOW_TITLE_LEN); 
    GetWindowText(hwnd, window_name, MAX_WINDOW_TITLE_LEN);
    // Rule 1. Expect the unexpected.
    if (strlen(window_name) <= 0){
        free(window_name);
        return TRUE;
    }

    // Set the name of the window in the struct
    // Increment found_window_count
    window_info_arr[found_win_count].handle = hwnd;
    window_info_arr[found_win_count].window_name = window_name;
    ++found_win_count;
    return TRUE;
}

int main(int argc, char *argv[]) 
{
    utils_init();
    printf("Enter window scan limit: ");
    scanf("%d", &max_win_count);

#ifdef ENABLE_DEBUG
    printf("Max Windows Limit: %d", max_win_count);
#endif
    WindowInfo window_arr[max_win_count]; 
    EnumWindows(window_callback, (LPARAM)window_arr);
    
    // Print options menu:
    // 0 - Mail
    // 1 - Settings
    // ....
    

    unsigned int win_selection = 0;
    printf("\nPlease attempt to select one of the windows from the following list of window options listed" 
    " below in the accompanying input prompt using your keyboard by entering the number on the left of the" 
    " hyphen that corresponds to the window title:");
    
    for(int window = 0; window < found_win_count; window++)
    {
        printf("\n%d - %s", window, window_arr[window].window_name);
    }

    //Get Window Selection Number
    while(TRUE)
    {
        printf("\nPlease enter a window selection: ");
        scanf("%u", &win_selection);

        if(win_selection < found_win_count)
        {
            break;
        }
    }
    
    // Store window in window for easy access
    WindowInfo* window = &window_arr[win_selection];
#ifdef ENABLE_DEBUG
    printf("Window selected: %s", window->window_name);
#endif
// STAGE 4
    ShowWindow(window->handle, SW_SHOWNORMAL);

    RECT window_rect;
    GetWindowRect(window->handle, &window_rect);

#ifdef ENABLE_DEBUG
    printf("Current window coordinates [x:%f] [y:%f]", window_rect.left, window_rect.top);
#endif

    // A LOT OF WINDOWS API SHIT
    // TODO! get current window pos and store in the vec2
    // What are we doing chat
    double old_time = get_absolute_time();
    
    Vec2 velocity = {
        .x = rand() % 20,
        .y = rand() % 20
    };
    while (1)
    {
        // K YOURSELF NOW !! 
        float dt = (get_absolute_time() - old_time)*9.81; // why tho its in seconds already
        old_time = get_absolute_time();
        // printf("deltatime: %lf\n", dt);
        
        // move window places
        
        // kinematicm oment
        float dx = velocity.x * dt;
        float dy = velocity.y * dt;
        // ur gonna lose a shit ton of presicion if u cast all ur immediate calculations to int btw watch otu
        // bc dx and dy will always be near 0 whe nu cast it it becomes 0 and ur adding NOTHING !
        // ideal solution is to store the position of window as a float for x and y then cast it when u set the window position
        window_rect.left += dx;
        window_rect.top += dy;
        #def

        // CHECK IF lower bound of window is greater than resolution
        
        SetWindowPos(window->handle, NULL, window_rect.left, window_rect.top, 0, 0, 0);
        // FOR DEBUG
    }

    return 0;
}