#include <stdio.h>
#include "util.h"
#include "vec2.h"
#include "physics_2d.h"

// #define RAYGUI_IMPLEMENTATION
// #include "raygui.h"

#define ENABLE_DEBUG

#define MAX_WINDOW_TITLE_LEN 64
#define MAX_WINDOW_COUNT 128

#define SPEED_RANDOMNESS 30
#define BASE_SPEED 1200
#define MAX_MEMORY 1000000

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

// EnumWindows accepts a callback function to run on each window found:
BOOL window_callback(HWND hwnd, LPARAM lParam);

int main(int argc, char *argv[]) 
{
    // Grab some system info
    unsigned int screen_w = GetSystemMetrics(SM_CXSCREEN);
    unsigned int screen_h = GetSystemMetrics(SM_CYSCREEN);

    // Init gui

    FRect screen_rect = {
        .left = 0,
        .top = 0,
        .right = screen_w,
        .bottom = screen_h
    };
    utils_init();

    WindowInfo window_arr[MAX_WINDOW_COUNT]; 
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
        scanf_s("%u", &win_selection);

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
    printf("bluh\n");
    ShowWindow(window->handle, SW_SHOWNORMAL);

    RECT window_rect;
    GetWindowRect(window->handle, &window_rect);

    // A LOT OF WINDOWS API SHIT
    // TODO! get current window pos and store in the vec2
    // What are we doing chat
    printf("pluh pluh\n");
    double old_time = get_absolute_time();
    printf("awflawkjfla pluh pluh\n");
    
    float speed_variance = rand() % SPEED_RANDOMNESS + BASE_SPEED;

    Vec2 vel = {
        // 20-40 random chance
        .x = speed_variance,
        .y = SPEED_RANDOMNESS - speed_variance
    };
    PhysicsObject2D simulated_win = {
        .collider = frect_from_winrect(window_rect),
        .vel = vel,
        // .accel = {.x = 999, .y = 999}
    };

    Physics2D sim;

    physics2d_init(&sim);
    printf("yuh\n");
    physics2d_set_simulation_area(&sim, screen_rect);
    printf("spluh\n");
    physics2d_add_object(&sim, &simulated_win);
    printf("cuh\n");

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
    if (found_win_count >= MAX_WINDOW_COUNT)
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