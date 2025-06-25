/**
 * @file splash.c
 * @brief A program to display a formatted splash screen for the "Magrathea" project.
 *
 * This program prompts the user for their name and the current date.
 * After a brief delay, it clears the screen and presents a splash screen
 * bordered with decorative elements, including asterisk triangles, as per
 * the project requirements. It is written using only the standard C libraries
 * stdio.h and stdlib.h.
 */

#include <stdio.h>
#include <stdlib.h> // Required for system()
#include <string.h> // Required for strlen()

// --- Helper Functions ---

/**
 * @brief Removes the trailing newline character from a string read by fgets.
 * @param str The string to modify.
 */
void remove_newline(char* str) {
    str[strcspn(str, "\n")] = '\0';
}

/**
 * @brief Clears the console screen in a cross-platform way.
 */
void clear_screen() {
// Use conditional compilation to issue the correct clear command for the OS.
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Linux, macOS, and other POSIX systems
#endif
}

/**
 * @brief Creates a simple, processor-dependent delay.
 *
 * NOTE: This is a "busy-wait" loop. It is used because the problem constraints
 * disallow platform-specific libraries like <unistd.h> (for sleep()) or
 * <windows.h> (for Sleep()), which are normally used for this purpose.
 * The loop's duration is approximate and will vary with CPU speed.
 */
void delay_seconds() {
    // A large loop to consume CPU time for roughly 3 seconds.
    for (volatile long i = 0; i < 4000000000; i++);
}


// --- Main Program Logic ---

int main(void) {
    char date[20];
    char name[50];

    // 1. Get User Input
    printf("[Please enter the current date in the \"yyyy-mm-dd\" format]: ");
    fgets(date, sizeof(date), stdin);
    remove_newline(date);

    printf("[Please enter your name]: ");
    fgets(name, sizeof(name), stdin);
    remove_newline(name);

    printf("**The input has been processed successfully.**\n");

    // 2. (Bonus) Delay and Clear Screen
    delay_seconds();
    clear_screen();

    // 3. Display the Splash Screen
    
    // An array of the main text lines, pre-padded with spaces to achieve
    // the exact alignment shown in the bonus example.
    const char* splash_lines[] = {
        "                                                                                                       ",
        "                                       [Magrathea ver 0.1]                                             ",
        "                  Magrathea, where a shining planet is created in a wasteland with no grass,           ",
        "          a place where unseen potential is discovered and gems are polished by the hands of experts,  ",
        "                                       Welcome to Magrathea.                                           "
    };
    
    // Top border
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // Print the main content row by row, including the bonus triangles.
    // This outer loop runs 5 times for the 5 rows of content.
    for (int i = 0; i < 5; i++) {
        
        // Print the left-side triangle using a nested for loop
        for (int j = 0; j < (i + 1); j++) {
            printf("*");
        }
        
        // Print the pre-formatted text line for the current row
        printf("%s", splash_lines[i]);
        
        // Print the right-side inverted triangle using another nested for loop
        for (int j = 0; j < (5 - i); j++) {
            printf("*");
        }
        printf("\n");
    }

    // Middle border
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // 4. Display the Footer
    
    // Print the user's name, then calculate and print the exact number of spaces
    // needed to align the date to the right edge.
    printf("[User]: %s", name);

    int total_width = 113; // The inner width of the footer line
    int left_len = strlen("[User]: ") + strlen(name);
    int right_len = strlen("[Execution Time]: ") + strlen(date);
    int spaces_needed = total_width - left_len - right_len;

    // Ensure at least one space if the name is very long
    if (spaces_needed < 1) {
        spaces_needed = 1;
    }

    for (int i = 0; i < spaces_needed; i++) {
        printf(" ");
    }
    
    printf("[Execution Time]: %s\n", date);

    // Bottom border
    printf("=============================================================================================================\n");

    return 0;
}
