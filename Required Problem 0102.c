#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to introduce a delay in seconds.
void delay(int number_of_seconds) {
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

// Function to clear the console screen.
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    // ANSI escape code for clearing screen
    printf("\033[H\033[J");
#endif
}

int main() {
    char date[11];
    char name[100];

    // Prompt for and retrieve the current date.
    printf("[Please enter the current date in the \"yyyy-mm-dd\" format]: ");
    scanf("%10s", date);

    // Prompt for and retrieve the user's name.
    printf("[Please enter your name]: ");
    scanf(" %[^\n]s", name);

    printf("**The input has been processed successfully.**\n");

    // Bonus: Wait for 3 seconds.
    delay(3);

    // Bonus: Clear the screen.
    clear_screen();

    // Display the splash screen.
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("* *****\n");
    printf("** [Magrathea ver 0.1]                                            ****\n");
    printf("*** Magrathea, where a shining planet is created in a wasteland with no grass,          ***\n");
    printf("**** a place where unseen potential is discovered and gems are polished by the hands of experts, **\n");
    printf("***** Welcome to Magrathea.                                          * \n");
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("[User]: %-39s [Execution Time]: %s\n", name, date);
    printf("=============================================================================================================\n");

    return 0;
}
