#include <stdio.h>
#include "frect.h"
#include "util.h"
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
unsigned int screen_w;
unsigned int screen_h;
int max_win_count = 32;

// EnumWindows accepts a callback function to run on each window found:
BOOL window_callback(HWND hwnd, LPARAM lParam);

int main(int argc, char *argv[]) 
{
    screen_w = GetSystemMetrics(SM_CXSCREEN);
    screen_h = GetSystemMetrics(SM_CYSCREEN);
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
    FRect win_rect = frect_from_winrect(window_rect);

#ifdef ENABLE_DEBUG
    printf("\nCurrent window coordinates [x:%f] [y:%f]", win_rect.top_left.x, win_rect.top_left.y);
    printf("\nWidth and Height [w:%f] [h:%f]", win_rect.w, win_rect.h);
#endif

    while (1)
    {
        // K YOURSELF NOW !! 
        float dt = get_absolute_time() - old_time; // why tho its in seconds already
        old_time = get_absolute_time();
        // printf("deltatime: %lf\n", dt);
        
        // move window places
        float new_speed_variance = rand() % SPEED_RANDOMNESS + BASE_SPEED;

        // kinematicm oment
        float dx = vel.x * dt;
        float dy = vel.y * dt;
        win_rect.top_left.x += dx;
        win_rect.top_left.y += dy;

        if (win_rect.top_left.x <= 0)
        {
            win_rect.top_left.x = 0;

            vel.x *= -1;
        }

        if (win_rect.top_left.x + win_rect.w >= screen_w){
            win_rect.top_left.x = screen_w - win_rect.w;

            vel.x *= -1;
        }

        if (win_rect.top_left.y <= 0)
        {
            win_rect.top_left.y = 0;

            vel.y *= -1;
        }

        if (win_rect.top_left.y + win_rect.h >= screen_h){
            win_rect.top_left.y = screen_h - win_rect.h;

            vel.y *= -1;
        }

        // TODO! CHECK IF bounds of window are greater or equal to resolution
        // if (win_rect.top_left.x <= 0 ||                         // Checks if the top left of the screen does not go past the left
        //     win_rect.top_left.x + win_rect.w >= screen_w)     // Checks if the top right goes past the screen width
        // {
            
        //     #ifdef ENABLE_DEBUG
        //             printf("CONTACT Y AXIS");
        //     #endif

        //     

        // }

        // if (win_rect.top_left.y <= 0 ||
        //     win_rect.top_left.y + win_rect.h >= screen_h)
        // {
            
        //     #ifdef ENABLE_DEBUG
        //             printf("CONTACT Y AXIS");
        //     #endif
            
        //     vel.y *= -1;

        // }

        SetWindowPos(window->handle, NULL, win_rect.top_left.x, win_rect.top_left.y, win_rect.w, win_rect.h, 0);
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