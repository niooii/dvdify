#include <stdio.h>
#include "util.h"
#include "vec2.h"
#include "physics_2d.h"
#define ENABLE_DEBUG

#define MAX_WINDOW_TITLE_LEN 64

#define SPEED_RANDOMNESS 30
#define BASE_SPEED 120

/*
 * geta list of windows
 * map each one to a number value
 * have user input number of window
 * then bounmce window around
*/
typedef struct WindowInfo {
  HWND handle;
  char* window_name;
} WindowInfo;

// Globals
unsigned int found_win_count = 0;
int max_win_count = 32;

// EnumWindows accepts a callback function to run on each window found:
BOOL window_callback(HWND hwnd, LPARAM lParam);

int main(int argc, char *argv[]) 
{
    unsigned int screen_w = GetSystemMetrics(SM_CXSCREEN);
    unsigned int screen_h = GetSystemMetrics(SM_CYSCREEN);
    FRect screen_rect = {
        .left = 0,
        .top = 0,
        .right = screen_w,
        .bottom = screen_h
    };
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

    // A LOT OF WINDOWS API SHIT
    // TODO! get current window pos and store in the vec2
    // What are we doing chat
    double old_time = get_absolute_time();
    
    float speed_variance = rand() % SPEED_RANDOMNESS + BASE_SPEED;

    Vec2 vel = {
        // 20-40 random chance
        .x = speed_variance,
        .y = SPEED_RANDOMNESS - speed_variance
    };
    PhysicsObject2D simulated_win = {
        .collider = frect_from_winrect(window_rect),
        .vel = vel,
        // .accel = {.x = 0, .y = -99}
    };

    Physics2D sim;

    physics2d_set_simulation_area(&sim, screen_rect);
    physics2d_add_object(&sim, &simulated_win);

#ifdef ENABLE_DEBUG
    // printf("\nCurrent window coordinates [x:%f] [y:%f]", win_rect.left, win_rect.top);
#endif

    while (1)
    {
        float dt = get_absolute_time() - old_time;
        old_time = get_absolute_time();

        physics2d_step(&sim, dt);

        SetWindowPos(
            window->handle, 
            NULL, 
            simulated_win.collider.left, 
            simulated_win.collider.top, 
            simulated_win.collider.right - simulated_win.collider.left, 
            simulated_win.collider.bottom - simulated_win.collider.top, 
            0
        );
    }

    return 0;
}

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