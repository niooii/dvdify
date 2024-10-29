#include <stdio.h>
#include "util.h"
#include "vec2.h"
#include "physics_2d.h"
#include "ww.h"

// #define RAYGUI_IMPLEMENTATION
// #include "raygui.h"

#define ENABLE_DEBUG
#define MAX_WINDOW_TITLE_LEN 64
#define MAX_WINDOW_COUNT 128

#define SPEED_RANDOMNESS 30
#define BASE_SPEED 1200
#define MAX_MEMORY 1000000

typedef struct WindowInfo {
  HWND handle;
  char* window_name;
} WindowInfo;

// Globals
unsigned int found_win_count = 0;

// EnumWindows accepts a callback function to run on each window found:
BOOL window_callback(HWND hwnd, LPARAM lParam);
// Callback for window watcher lib
#define MAX_MESSAGES 100
static unsigned int msg_received[MAX_MESSAGES] = {0};
static int msg_count = 0;
void process_msg_fn(CWPSTRUCT* message_info)
{
    // MessageBoxA(NULL, "RAN PROCESS MESSAGE FN!!!", "YAY!!", MB_OK | MB_ICONERROR);
    if (msg_count < MAX_MESSAGES)
    {
        // Additional information about the message
        char name_buf[200];
        memset(name_buf, 0, 200);
        if (!IsWindowVisible(message_info->hwnd))
            return;
        GetWindowText(message_info->hwnd, name_buf, 200);
        if (strlen(name_buf) == 0)
            return;
        msg_received[msg_count++] = message_info->message;
        printf("Received message: 0x%08X (decimal: %u)\n", message_info->message, message_info->message);
        
        printf("  Window name: %s", name_buf);
        printf("  Window handle: 0x%p\n", (void*)message_info->hwnd);
        printf("  wParam: 0x%p\n", (void*)message_info->wParam);
        printf("  lParam: 0x%p\n", (void*)message_info->lParam);
    }
}

int main(int argc, char *argv[]) 
{
    // Grab some system info
    unsigned int screen_w = GetSystemMetrics(SM_CXSCREEN);
    unsigned int screen_h = GetSystemMetrics(SM_CYSCREEN);

    if (!InstallHook())
    {
        printf("Failed to install hook...\n");
        return 1;
    }
    else
    {
        printf("Hook has been set up.\n");
    }
    SetCallbackFn(process_msg_fn);

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
    
    ShowWindow(window->handle, SW_SHOWNORMAL);

    RECT window_rect;
    GetWindowRect(window->handle, &window_rect);

    // A LOT OF WINDOWS API SHIT
    double old_time = get_absolute_time();
    
    float speed_variance = rand() % SPEED_RANDOMNESS + BASE_SPEED;

    Physics2D sim;

    physics2d_init(&sim);
    physics2d_set_simulation_area(&sim, screen_rect);

    Vec2 vel = {
        // 20-40 random chance
        .x = speed_variance,
        .y = SPEED_RANDOMNESS - speed_variance
    };
    Vec2 a = {
        .x = 0,
        .y = 900.81
    };

    PhysicsObject2D* simulated_mouse = physics2d_add_object(&sim);
    PhysicsObject2D* simulated_win = physics2d_add_object(&sim);
    simulated_mouse->vel = vel;
    simulated_mouse->accel = a;
    simulated_win->vel = vel;
    // simulated_win->accel = a;
    simulated_win->collider = frect_from_winrect(window_rect);

    // MSG msg;
    // while (GetMessageW(&msg, NULL, 0, 0))
    // {
    //     TranslateMessage(&msg);
    //     DispatchMessageW(&msg);
    //     printf("got msg: %u\n", msg.message);
    // }

    while (1)
    {
        // printf("got msg: %u\n", msg_received);
        float dt = get_absolute_time() - old_time;
        old_time = get_absolute_time();

        physics2d_step(&sim, dt);

        SetWindowPos(
            window->handle, 
            NULL, 
            simulated_win->collider.left, 
            simulated_win->collider.top, 
            simulated_win->collider.right - simulated_win->collider.left, 
            simulated_win->collider.bottom - simulated_win->collider.top, 
            0
        );

        SetCursorPos(
            simulated_mouse->collider.left,
            simulated_mouse->collider.top
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