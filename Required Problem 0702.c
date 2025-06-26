/**
 * @file main.c
 * @brief A self-contained version of the "Concept Research" game for online compilers.
 *
 * This program combines all necessary functions into a single file. It features
 * a game where users match K-POP groups to their correct concepts. The user can
 * choose to play the game using either a Stack or a Queue data structure, fulfilling
 * the bonus requirement. It includes a fallback mechanism to use hardcoded data
 * if the required input file is not found.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Constants and Data Structures ---
#define MAX_CONCEPTS 20

// Structure to hold one concept entry from the database
typedef struct {
    char group_name[100];
    char concept_name[100];
    char description[512];
} ConceptData;

// Node for the Linked List (used by both Stack and Queue)
typedef struct Node {
    char data[100];
    struct Node* next;
} Node;

// Stack structure
typedef struct {
    Node* top;
} Stack;

// Queue structure
typedef struct {
    Node* front;
    Node* rear;
} Queue;


// --- Global Data Storage ---
ConceptData* g_concept_db = NULL;
int g_concept_count = 0;


// --- Forward Declarations for All Functions ---
void clear_screen();
void cleanup_stage6_data();
void show_main_menu();
void show_training_menu();
void show_visual_menu();
void defineConcept();


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
        printf("   B. Concept Research\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');
        if (toupper(choice) == 'A') {
             printf("\n['Finding People' feature not shown in this combined file.]\n");
             printf("Press Enter to continue...");
             getchar();
        } else if (toupper(choice) == 'B') {
            defineConcept();
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

// Linked List Stack & Queue Operations
Stack* createStack() { Stack* s = (Stack*)malloc(sizeof(Stack)); s->top = NULL; return s; }
Queue* createQueue() { Queue* q = (Queue*)malloc(sizeof(Queue)); q->front = q->rear = NULL; return q; }

void push(Stack* s, const char* data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->data, data);
    newNode->next = s->top;
    s->top = newNode;
}

void enqueue(Queue* q, const char* data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->data, data);
    newNode->next = NULL;
    if(q->rear == NULL) q->front = q->rear = newNode;
    else { q->rear->next = newNode; q->rear = newNode; }
}

void destroyStack(Stack* s) {
    Node* current = s->top;
    while(current != NULL) { Node* temp = current; current = current->next; free(temp); }
    free(s);
}
void destroyQueue(Queue* q) {
    Node* current = q->front;
    while(current != NULL) { Node* temp = current; current = current->next; free(temp); }
    free(q);
}

void load_concepts() {
    if (g_concept_db != NULL) return;

    FILE* file = fopen("concept.txt", "r");
    if (!file) {
        // --- Fallback for Online Compilers ---
        printf("Notice: 'concept.txt' not found. Using hardcoded fallback data.\n");
        g_concept_db = (ConceptData*)malloc(7 * sizeof(ConceptData));
        if(!g_concept_db) return;
        g_concept_count = 7;
        strcpy(g_concept_db[0].group_name, "Wonder Girls"); strcpy(g_concept_db[0].concept_name, "Retro"); strcpy(g_concept_db[0].description, "A concept that reinterprets past trends in a modern way.");
        strcpy(g_concept_db[1].group_name, "2PM"); strcpy(g_concept_db[1].concept_name, "Beastly Idol"); strcpy(g_concept_db[1].description, "A concept emphasizing powerful and masculine performances.");
        strcpy(g_concept_db[2].group_name, "Crayon Pop"); strcpy(g_concept_db[2].concept_name, "Goofy/Novelty"); strcpy(g_concept_db[2].description, "A unique concept using helmets and quirky choreography.");
        strcpy(g_concept_db[3].group_name, "Girl's Day"); strcpy(g_concept_db[3].concept_name, "Sexy"); strcpy(g_concept_db[3].description, "A concept highlighting mature and alluring charms.");
        strcpy(g_concept_db[4].group_name, "BTS"); strcpy(g_concept_db[4].concept_name, "Social Commentary"); strcpy(g_concept_db[4].description, "A concept that includes messages about social issues and youth struggles.");
        strcpy(g_concept_db[5].group_name, "aespa"); strcpy(g_concept_db[5].concept_name, "Metaverse/AI"); strcpy(g_concept_db[5].description, "A futuristic concept involving virtual avatars and a digital world.");
        strcpy(g_concept_db[6].group_name, "SHINee"); strcpy(g_concept_db[6].concept_name, "Contemporary"); strcpy(g_concept_db[6].description, "A trend-setting concept that always presents a sophisticated and modern style.");
        return;
    }
    
    g_concept_db = (ConceptData*)malloc(MAX_CONCEPTS * sizeof(ConceptData));
    char line[1024];
    while(g_concept_count < MAX_CONCEPTS && fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        char* group = strtok(line, ";");
        char* concept = strtok(NULL, ";");
        char* desc = strtok(NULL, "");
        if(group && concept && desc) {
            strcpy(g_concept_db[g_concept_count].group_name, group);
            strcpy(g_concept_db[g_concept_count].concept_name, concept);
            strcpy(g_concept_db[g_concept_count].description, desc);
            g_concept_count++;
        }
    }
    fclose(file);
}

void playMatchingGame(char** groups, char** concepts, ConceptData* original_data, int count) {
    printf("\n--- Match the Group to its Correct Concept ---\n");
    printf("================================================\n");
    printf("| %-2s | %-20s || %-2s | %-20s |\n", "#", "Group", "#", "Concept");
    printf("------------------------------------------------\n");
    for(int i=0; i<count; i++) {
        printf("| %-2d | %-20s || %-2d | %-20s |\n", i+1, groups[i], i+1, concepts[i]);
    }
    printf("================================================\n");

    printf("\nEnter your matches as pairs (e.g., to match Group #1 with Concept #3, type '1 3').\n");
    
    int user_matches[count];
    for(int i=0; i<count; i++) {
        printf("Match for Group #%d (%s): ", i+1, groups[i]);
        scanf("%d", &user_matches[i]);
    }
    while(getchar() != '\n');
    
    int correct_count = 0;
    char incorrect_matches[count][256];
    int incorrect_count = 0;

    for(int i=0; i<count; i++) {
        const char* user_group = groups[i];
        const char* user_chosen_concept = concepts[user_matches[i]-1];
        
        char correct_concept[100] = "";
        for(int j=0; j<count; j++){
            if(strcmp(original_data[j].group_name, user_group) == 0) {
                strcpy(correct_concept, original_data[j].concept_name);
                break;
            }
        }
        
        if(strcmp(user_chosen_concept, correct_concept) == 0) {
            correct_count++;
        } else {
            sprintf(incorrect_matches[incorrect_count++], "Group '%s' -> Correct Concept was '%s'", user_group, correct_concept);
        }
    }
    
    printf("\n--- Results ---\n");
    printf("You got %d out of %d correct!\n", correct_count, count);
    if(incorrect_count > 0) {
        printf("\nIncorrect Matches:\n");
        for(int i=0; i<incorrect_count; i++) {
            printf(" - %s\n", incorrect_matches[i]);
        }
    }
}

/**
 * @brief The main function for the Concept Research feature.
 */
void defineConcept() {
    clear_screen();
    printf("========================================\n");
    printf("         B. Concept Research\n");
    printf("========================================\n");
    
    load_concepts();
    if(g_concept_count == 0) { printf("Failed to load concept data.\n"); return; }
    
    char buffer[10];
    int use_stack = 0;

    printf("Choose a data structure for the game:\n  1. Queue (FIFO)\n  2. Stack (LIFO) [Bonus]\nChoice: ");
    fgets(buffer, sizeof(buffer), stdin);
    if(buffer[0] == '2') use_stack = 1;
    
    int num_to_extract = 0;
    int max_extract = g_concept_count > 4 ? 4 : g_concept_count;
    printf("Enter number of data sets to use (max %d): ", max_extract);
    fgets(buffer, sizeof(buffer), stdin);
    num_to_extract = atoi(buffer);
    if(num_to_extract < 1 || num_to_extract > max_extract) {
        printf("Invalid number. Defaulting to %d.\n", max_extract);
        num_to_extract = max_extract;
    }
    
    ConceptData selected_data[num_to_extract];
    int indices[g_concept_count];
    for(int i=0; i<g_concept_count; i++) indices[i] = i;
    for(int i=g_concept_count-1; i>0; i--) { int j = rand()%(i+1); int tmp=indices[i]; indices[i]=indices[j]; indices[j]=tmp; }
    for(int i=0; i<num_to_extract; i++) selected_data[i] = g_concept_db[indices[i]];
    
    char* groups[num_to_extract];
    char* concepts[num_to_extract];
    for(int i=0; i<num_to_extract; i++) {
        groups[i] = selected_data[i].group_name;
        concepts[i] = selected_data[i].concept_name;
    }
    for(int i=num_to_extract-1; i>0; i--) { int j = rand()%(i+1); char* tmp=concepts[i]; concepts[i]=concepts[j]; concepts[j]=tmp; }

    if (use_stack) {
        printf("\nUsing STACK data structure for game.\n");
        Stack* group_s = createStack();
        Stack* concept_s = createStack();
        for(int i=0; i<num_to_extract; i++){ push(group_s, groups[i]); push(concept_s, concepts[i]); }
        playMatchingGame(groups, concepts, selected_data, num_to_extract);
        destroyStack(group_s); destroyStack(concept_s);
    } else {
        printf("\nUsing QUEUE data structure for game.\n");
        Queue* group_q = createQueue();
        Queue* concept_q = createQueue();
        for(int i=0; i<num_to_extract; i++){ enqueue(group_q, groups[i]); enqueue(concept_q, concepts[i]); }
        playMatchingGame(groups, concepts, selected_data, num_to_extract);
        destroyQueue(group_q); destroyQueue(concept_q);
    }
    
    printf("\nPress Enter to return to menu...");
    getchar();
}

/**
 * @brief Frees all dynamically allocated memory at program exit.
 */
void cleanup_stage6_data() {
    if (g_concept_db != NULL) {
        free(g_concept_db);
        g_concept_db = NULL;
    }
}
