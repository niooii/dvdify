#include <stdio.h>
#include <Windows.h>
#define ENABLE_DEBUG

#define MAX_WINDOW_COUNT 128
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

unsigned short found_window_count = 0;
// EnumWindows accepts a callback function to run on each window found:
BOOL window_callback(HWND hwnd, LPARAM lParam)
{
    WindowInfo* window_info_arr = (WindowInfo*)lParam;
    if (found_window_count >= MAX_WINDOW_COUNT)
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
    window_info_arr[found_window_count].handle = hwnd;
    window_info_arr[found_window_count].window_name = window_name;
    ++found_window_count;
    return TRUE;
}

int main(int argc, char *argv[]) 
{
    printf("hi world\n");
    // List windows and shit
    // EnumWindows();
    // Accepts a format modifier (in this case %s is string, to accept integer do %d, accept float is %f etc) 
    // BUT..
    // where is the value stored?? u maybe asking,,,
    // we must allocate MEMORY for it first !
    // NO WTF
    // Max input size of 200
    int input;
    printf("input A NUMBER..: ");
    scanf("%d", &input);

#ifdef ENABLE_DEBUG
    printf("user inputted: %d", input);
#endif
    WindowInfo window_info_arr[MAX_WINDOW_COUNT]; 
    EnumWindows(window_callback, (LPARAM)window_info_arr);
    
    // Print options menu:
    // 0 - Mail
    // 1 - Settings
    // ....
    for

    return 0;
}