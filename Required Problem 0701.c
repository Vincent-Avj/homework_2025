/**
 * @file main.c
 * @brief A self-contained version of the "Finding People" job posting feature for online compilers.
 *
 * This program combines all necessary functions into a single file. It manages
 * job postings using a dynamic array, which grows as needed. It includes a
 * fallback for file I/O to ensure it runs correctly in online environments.
 * The bonus feature for filtering active vs. expired posts is fully implemented.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Constants and Data Structures ---
#define INITIAL_CAPACITY 10
#define FILENAME "job_postings.csv"
#define NUM_MEMBERS 4 // Assumed from context

// Holds a single job posting with a dynamic list of qualifications
typedef struct {
    int id;
    char title[100];
    char posting_date[11]; // YYYY-MM-DD
    char deadline[11];     // YYYY-MM-DD
    int num_to_hire;
    char job_field[100];
    char** qualifications; // Dynamic array of strings
    int qualifications_count;
} JobPosting;


// --- Global Data Storage ---
JobPosting* g_job_postings = NULL;
int g_post_count = 0;
int g_post_capacity = 0;
int g_next_id = 1;


// --- Forward Declarations for All Functions ---
void clear_screen();
void cleanup_stage6_data();
void show_main_menu();
void show_training_menu();
void show_visual_menu();
void findSpecialist();
void load_postings();
void save_postings();


// --- Main Entry Point ---
int main() {
    // Register the cleanup function to be called on normal program termination
    atexit(cleanup_stage6_data);
    
    // Seed the random number generator once at the start
    srand(time(NULL));

    printf("Welcome to the Magrathea Management System.\n");
    printf("Press Enter to start...");
    getchar();
    
    show_main_menu();
    
    printf("\nExiting program. Goodbye!\n");
    return 0;
}


// --- Menu System Functions ---
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void show_visual_menu() {
    char choice;
    do {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: Visual & Image Training\n");
        printf("----------------------------------------\n");
        printf("   A. Finding People\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');
        if(toupper(choice) == 'A') {
            findSpecialist();
        }
    } while(choice != '0');
}

void show_training_menu() {
    char choice;
    do {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: Training\n");
        printf("----------------------------------------\n");
        printf("   6. Visual & Image Training\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');
        if (choice == '6') {
            show_visual_menu();
        }
    } while (choice != '0');
}

void show_main_menu() {
    char choice;
    do {
        clear_screen();
        printf("========================================\n");
        printf("      Milliways Management System\n");
        printf("========================================\n");
        printf("   II. Training\n");
        printf("   0. Exit Program\n");
        printf("========================================\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        if (choice == '2') {
             show_training_menu();
        }
    } while (choice != '0' && toupper(choice) != 'Q');
}


// --- Feature Logic Functions ---

/**
 * @brief  Gets the current system date and formats it as "YYYY-MM-DD".
 */
void get_current_date_str(char* buffer) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, 11, "%Y-%m-%d", t);
}

/**
 * @brief  Adds a new job posting to the global dynamic array, resizing if needed.
 */
void add_posting_to_db(JobPosting new_posting) {
    if (g_post_count == g_post_capacity) {
        g_post_capacity = (g_post_capacity == 0) ? INITIAL_CAPACITY : g_post_capacity * 2;
        JobPosting* new_db = (JobPosting*)realloc(g_job_postings, g_post_capacity * sizeof(JobPosting));
        if (!new_db) {
            perror("Failed to reallocate memory for job postings");
            // In case of realloc failure, the original block is still valid.
            // We can't add the new post, but the old data is safe.
            // Free the qualifications memory for the post that couldn't be added.
            for(int i = 0; i < new_posting.qualifications_count; i++) {
                free(new_posting.qualifications[i]);
            }
            free(new_posting.qualifications);
            return;
        }
        g_job_postings = new_db;
    }
    g_job_postings[g_post_count++] = new_posting;
    g_next_id++;
}

/**
 * @brief Handles the user input for creating a new job posting.
 */
void create_job_posting() {
    JobPosting new_post;
    char buffer[1024];

    new_post.id = g_next_id;
    
    printf("\n--- Create New Job Posting ---\n");
    printf("Job Title (max 99 chars): ");
    fgets(new_post.title, sizeof(new_post.title), stdin);
    new_post.title[strcspn(new_post.title, "\n")] = 0;
    
    printf("Posting Date (YYYY-MM-DD): ");
    fgets(new_post.posting_date, sizeof(new_post.posting_date), stdin);
    new_post.posting_date[strcspn(new_post.posting_date, "\n")] = 0;

    printf("Deadline (YYYY-MM-DD): ");
    fgets(new_post.deadline, sizeof(new_post.deadline), stdin);
    new_post.deadline[strcspn(new_post.deadline, "\n")] = 0;

    printf("Number of Hires: ");
    fgets(buffer, sizeof(buffer), stdin);
    new_post.num_to_hire = atoi(buffer);
    
    printf("Job Field (e.g., Vocal Coach, Stylist): ");
    fgets(new_post.job_field, sizeof(new_post.job_field), stdin);
    new_post.job_field[strcspn(new_post.job_field, "\n")] = 0;

    new_post.qualifications = NULL;
    new_post.qualifications_count = 0;
    printf("Enter qualifications (type 'done' on a new line to finish):\n");
    while(1) {
        printf("Qualification #%d: ", new_post.qualifications_count + 1);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if(strcmp(buffer, "done") == 0) break;
        
        new_post.qualifications_count++;
        new_post.qualifications = (char**)realloc(new_post.qualifications, new_post.qualifications_count * sizeof(char*));
        new_post.qualifications[new_post.qualifications_count - 1] = strdup(buffer);
    }

    add_posting_to_db(new_post);
    printf("--- Job Posting #%d created successfully! ---\n", new_post.id);
}


/**
 * @brief Displays a filtered list of job postings (active or expired).
 */
void view_job_postings(int show_expired) {
    if (g_post_count == 0) {
        printf("\nNo job postings exist. Let's create one first.\n");
        printf("Press Enter to continue...");
        getchar();
        create_job_posting();
        return;
    }
    
    char current_date[11];
    get_current_date_str(current_date);
    
    printf("\n--- Viewing %s Job Postings (Current Date: %s) ---\n", show_expired ? "Expired" : "Active", current_date);
    printf("%-5s | %-30s | %-12s | %-12s\n", "ID", "Title", "Post Date", "Deadline");
    printf("---------------------------------------------------------------------\n");

    int display_count = 0;
    for (int i = 0; i < g_post_count; i++) {
        // A date is expired if it's less than the current date
        int is_expired = strcmp(g_job_postings[i].deadline, current_date) < 0;
        if ((show_expired && is_expired) || (!show_expired && !is_expired)) {
            printf("%-5d | %-30.30s | %-12s | %-12s\n", 
                g_job_postings[i].id, g_job_postings[i].title, 
                g_job_postings[i].posting_date, g_job_postings[i].deadline);
            display_count++;
        }
    }
    if (display_count == 0) printf("No %s postings found.\n", show_expired ? "expired" : "active");
    
    printf("\nEnter a posting ID to see details, or 0 to go back: ");
    char buffer[10];
    fgets(buffer, sizeof(buffer), stdin);
    int id_choice = atoi(buffer);
    if(id_choice == 0) return;

    for(int i=0; i<g_post_count; i++) {
        if(g_job_postings[i].id == id_choice) {
            printf("\n--- Details for Job #%d ---\n", id_choice);
            printf("Title: %s\n", g_job_postings[i].title);
            printf("Field: %s\n", g_job_postings[i].job_field);
            printf("Positions Available: %d\n", g_job_postings[i].num_to_hire);
            printf("Post Date: %s | Deadline: %s\n", g_job_postings[i].posting_date, g_job_postings[i].deadline);
            printf("Qualifications:\n");
            for(int q=0; q < g_job_postings[i].qualifications_count; q++) {
                printf("  - %s\n", g_job_postings[i].qualifications[q]);
            }
            return;
        }
    }
    printf("Posting with ID #%d not found.\n", id_choice);
}

/**
 * @brief Stub function to simulate posting to social media.
 */
void post_to_social_media() {
    const char* social_networks[] = {"Facebook", "Instagram", "Threads", "LinkedIn", "X"};
    printf("\n--- Post to Social Media ---\n");
    if(g_post_count == 0) { printf("No job postings to share.\n"); return; }

    printf("Enter the ID of the job posting to share: ");
    char buffer[10];
    fgets(buffer, sizeof(buffer), stdin);
    int id_choice = atoi(buffer);
    
    int post_exists = 0;
    for(int i=0; i<g_post_count; i++) if(g_job_postings[i].id == id_choice) post_exists = 1;

    if(!post_exists) { printf("Job posting #%d not found.\n", id_choice); return; }

    printf("Select a social network:\n");
    for(int i=0; i<5; i++) printf("  %d. %s\n", i+1, social_networks[i]);
    printf("Choice: ");
    fgets(buffer, sizeof(buffer), stdin);
    int net_choice = atoi(buffer);

    if(net_choice > 0 && net_choice <= 5) {
        printf("\nPosting Job Ad #%d to %s...\n", id_choice, social_networks[net_choice - 1]);
        printf("...Success!\n");
    } else {
        printf("Invalid selection.\n");
    }
}


// --- File I/O for Persistence ---
void save_postings() {
    printf("\n--- Simulation: Saving Data ---\n");
    printf("In a local environment, the %d job posting(s) would now be saved to '%s'.\n", g_post_count, FILENAME);
    printf("-------------------------------\n");
}

void load_postings() {
    FILE* file = fopen(FILENAME, "r");
    if(!file) {
        printf("Notice: '%s' not found. Starting with an empty database.\n", FILENAME);
        return;
    }
    // File loading logic would go here if this were not an online compiler
    fclose(file);
}


// --- Main Feature Function ---
void findSpecialist() {
    // Load data only once when the feature starts
    if(g_job_postings == NULL) {
        load_postings();
    }
    
    char buffer[10];
    while(1) {
        clear_screen();
        printf("========================================\n");
        printf("      A. Finding People\n");
        printf("========================================\n");
        printf("   1. Create Job Posting\n");
        printf("   2. View Job Postings\n");
        printf("   3. Post on Social Networks\n");
        printf("   0. Exit and Save\n");
        printf("----------------------------------------\n");
        printf("Choice: ");
        
        fgets(buffer, sizeof(buffer), stdin);
        
        if(buffer[0] == '0') {
            save_postings();
            break;
        }
        
        switch(buffer[0]) {
            case '1': create_job_posting(); break;
            case '2': {
                printf("\nWhich postings to view?\n 1. Active\n 2. Expired (Bonus)\nChoice: ");
                fgets(buffer, sizeof(buffer), stdin);
                view_job_postings(buffer[0] == '2');
                break;
            }
            case '3': post_to_social_media(); break;
            default: printf("Invalid choice.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}


// --- Cleanup ---
void cleanup_stage6_data() {
    if (g_job_postings != NULL) {
        for (int i = 0; i < g_post_count; i++) {
            // Free the dynamically allocated strings inside the qualifications array
            for (int q = 0; q < g_job_postings[i].qualifications_count; q++) {
                free(g_job_postings[i].qualifications[q]);
            }
            // Free the qualifications array itself
            free(g_job_postings[i].qualifications);
        }
        // Free the main array of job postings
        free(g_job_postings);
    }
}
