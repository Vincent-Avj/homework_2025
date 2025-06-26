/**
 * @file main.c
 * @brief A self-contained version of the "Reading List Management" feature for online compilers.
 *
 * This program combines all necessary functions into a single file. The core
 * feature is a manager for a list of acting/stage theory subjects, implemented
 * as a Singly Linked List. It supports adding, removing, and re-ordering
 * list items. A fallback mechanism is included to use hardcoded data if the
 * required input file is not found, ensuring it can run in any environment.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For toupper()

// --- Data Structures and Global Declarations ---
// The structure for a single node in our linked list
typedef struct SubjectNode {
    int order;
    char subject_name[100];
    char details[1024];
    struct SubjectNode* next;
} SubjectNode;

// Global head pointer for the main list
SubjectNode* g_subject_list_head = NULL;
// (Bonus) Global head pointer for the completed list
SubjectNode* g_read_list_head = NULL;


// --- Forward Declarations for All Functions ---
void clear_screen();
void cleanupLists();
void show_main_menu();
void show_training_menu();
void show_acting_menu();
void doReadingList();
void PrintSubjectList(const SubjectNode* head, const char* list_name);
void UpdateSubjectInfo(const char* line_from_file);


// --- Main Entry Point ---
int main() {
    // Register the cleanup function to be called on normal program termination
    atexit(cleanupLists);

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

void show_training_menu() {
    char choice;
    do {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: Training\n");
        printf("----------------------------------------\n");
        printf("   7. Acting & Stage Performance\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');
        if (choice == '7') {
            show_acting_menu();
        }
    } while (choice != '0');
}

void show_acting_menu() {
    char choice;
    do {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: Acting & Stage Performance\n");
        printf("----------------------------------------\n");
        printf("   B. Reading List Management\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');
        if (toupper(choice) == 'B') {
            doReadingList();
        }
    } while(choice != '0');
}


// --- Feature-Specific Functions ---

// --- Linked List Operations ---
SubjectNode* createNode(int order, const char* title, const char* author) {
    SubjectNode* newNode = (SubjectNode*)malloc(sizeof(SubjectNode));
    if (!newNode) {
        perror("Failed to allocate memory for new node");
        return NULL;
    }
    newNode->order = order;
    strcpy(newNode->subject_name, title);
    strcpy(newNode->details, author);
    newNode->next = NULL;
    return newNode;
}

void renumberList(SubjectNode* head) {
    int current_order = 1;
    SubjectNode* current = head;
    while (current != NULL) {
        current->order = current_order++;
        current = current->next;
    }
}

void insertNodeAt(SubjectNode** p_head, SubjectNode* node_to_insert, int position) {
    if (position <= 1 || *p_head == NULL) {
        node_to_insert->next = *p_head;
        *p_head = node_to_insert;
    } else {
        SubjectNode* current = *p_head;
        int current_pos = 1;
        while (current->next != NULL && current_pos < position - 1) {
            current = current->next;
            current_pos++;
        }
        node_to_insert->next = current->next;
        current->next = node_to_insert;
    }
    renumberList(*p_head);
}

SubjectNode* removeNodeAt(SubjectNode** p_head, int position) {
    if (*p_head == NULL) return NULL;
    SubjectNode* node_to_remove = NULL;

    if (position <= 1) {
        node_to_remove = *p_head;
        *p_head = (*p_head)->next;
    } else {
        SubjectNode* current = *p_head;
        int current_pos = 1;
        while (current->next != NULL && current_pos < position - 1) {
            current = current->next;
            current_pos++;
        }
        if (current->next != NULL) {
            node_to_remove = current->next;
            current->next = node_to_remove->next;
        }
    }
    
    renumberList(*p_head);
    return node_to_remove;
}

// --- File I/O and Main Feature Logic ---
void UpdateSubjectInfo(const char* line_from_file) {
    for (SubjectNode* current = g_subject_list_head; current != NULL; current = current->next) {
        if (strstr(line_from_file, current->subject_name) != NULL) {
            strcpy(current->details, line_from_file);
            return;
        }
    }
}

void LoadContentFile() {
    FILE* file = fopen("perform_stage.txt", "r");
    if (!file) {
        printf("Notice: 'perform_stage.txt' not found. Loading hardcoded details.\n");
        // Fallback data for online compilers
        UpdateSubjectInfo("Acting Theory involves understanding character motivation, emotional range, and physical expression to create a believable performance.");
        UpdateSubjectInfo("Stage Theory focuses on the use of space and design. It is crucial for creating atmosphere.");
        UpdateSubjectInfo("Script Analysis is the deep reading of a text to understand subtext, themes, and character arcs.");
        UpdateSubjectInfo("Dialogue Interpretation and Emotional Expression focuses on how lines are delivered to convey emotion.");
        UpdateSubjectInfo("Character Development is the process of creating a three-dimensional character with a backstory and goals.");
        UpdateSubjectInfo("Storytelling Theory covers the structure of narrative, including plot points, pacing, and tension.");
        UpdateSubjectInfo("Stage Movement and Poses concerns how an actor uses their body to command a space non-verbally.");
        return;
    }
    
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        UpdateSubjectInfo(line);
    }
    fclose(file);
}

void initializeSubjectList() {
     if (g_subject_list_head != NULL) return;
     printf("Initializing subject list...\n");
     const char* subjects[] = {
        "Acting Theory", "Stage Theory", "Script Analysis",
        "Dialogue Interpretation and Emotional Expression", "Character Development",
        "Storytelling Theory", "Stage Movement and Poses"
     };
     for(int i=0; i < 7; i++){
        SubjectNode* newNode = createNode(i+1, subjects[i], "(No details loaded yet)");
        if(g_subject_list_head == NULL) {
            g_subject_list_head = newNode;
        } else {
            SubjectNode* current = g_subject_list_head;
            while(current->next != NULL) current = current->next;
            current->next = newNode;
        }
     }
     LoadContentFile();
}

/**
 * @brief Main function for the Reading List Management feature.
 */
void doReadingList() {
    initializeSubjectList();
    
    char buffer[100];
    int choice;
    while(1) {
        clear_screen();
        printf("========================================\n");
        printf("      Reading List Management\n");
        printf("========================================\n");
        PrintSubjectList(g_subject_list_head, "To-Read List");
        PrintSubjectList(g_read_list_head, "Completed Books (Bonus)");
        
        printf("\nChoose an action:\n");
        printf("  1. Add New Subject (Bonus)\n");
        printf("  2. Remove Subject (Bonus)\n");
        printf("  0. Back to previous menu\n");
        printf("Choice: ");
        
        fgets(buffer, sizeof(buffer), stdin);
        choice = atoi(buffer);

        if (choice == 0) break;
        
        int pos;
        char name[100], details[1024];

        switch(choice) {
            case 1:
                printf("Enter position to add at: ");
                fgets(buffer, sizeof(buffer), stdin); pos = atoi(buffer);
                printf("Enter new subject name: ");
                fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = 0;
                printf("Enter new subject details: ");
                fgets(details, sizeof(details), stdin); details[strcspn(details, "\n")] = 0;
                SubjectNode* new_node = createNode(0, name, details);
                insertNodeAt(&g_subject_list_head, new_node, pos);
                break;
            case 2:
                printf("Enter position of the subject to remove: ");
                fgets(buffer, sizeof(buffer), stdin); pos = atoi(buffer);
                SubjectNode* removed_node = removeNodeAt(&g_subject_list_head, pos);
                if (removed_node) {
                    printf("Removed: \"%s\"\n", removed_node->subject_name);
                    free(removed_node);
                } else {
                    printf("Invalid position.\n");
                }
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        if(choice >= 1 && choice <= 2) {
             printf("\nOperation complete. Press Enter to continue...");
             getchar();
        }
    }
}

// --- Cleanup ---
void cleanupLists() {
    SubjectNode *current, *next;
    current = g_subject_list_head;
    while(current != NULL) { next = current->next; free(current); current = next; }
    current = g_read_list_head;
    while(current != NULL) { next = current->next; free(current); current = next; }
}

// --- Print function with special formatting ---
void PrintSubjectList(const SubjectNode* head, const char* list_name) {
    if (head == NULL && strcmp(list_name, "To-Read List") == 0) {
        // Only show this detailed message if the main list is empty.
        printf("\n--- %s ---\n", list_name);
        printf("The list is empty.\n");
        printf("----------------------------------------\n");
        return;
    }
    // If the read list is empty, it's fine to just show that.
    if(head == NULL) return;

    printf("\n--- %s ---\n", list_name);
    
    int order = 1;
    const SubjectNode* current = head;
    while(current != NULL) {
        printf("\n%d. Subject: %s\n", order++, current->subject_name);
        printf("   Details: ");
        
        int len = strlen(current->details);
        for(int i=0; i<len; i++) {
            printf("%c", current->details[i]);
            if (current->details[i] == '.' && i < len - 1 && current->details[i+1] == ' ') {
                printf("\n            "); // Newline and indent after a period.
            }
        }
        printf("\n");
        current = current->next;
    }
    printf("----------------------------------------\n");
}
