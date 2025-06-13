#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define BONUS_MATCHING 1

#define NUM_MAIN_MENU_ITEMS 3
#define NUM_TRAINING_STAGES 8
#define NUM_TRAINEES 4 // Milliways has 4 members
#define MAX_MENTORS 8
#define MAX_NAME_LEN 50


struct Mentor {
    int id;
    char name[MAX_NAME_LEN];
    int mentee_list_number; 
};

const char* MILLIWAYS_NICKNAMES[NUM_TRAINEES] = {"Ariel", "Simba", "Belle", "Aladdin"};

void display_main_menu();
void handle_training_menu();
void matchMentoring();
int parseIntMember(const char* nickname);
int getRandomAbility();
void initializeTrainees(int data[][2]);
void inputMentors(struct Mentor list[], int* mentor_count);
void randomMatching(int trainees[][2], struct Mentor mentors[], int num_mentors);
void displayMatches(const struct Mentor mentors[], int num_mentors, int trainees[][2]);

int main() {
    char input_buffer[100];
    int choice;

    srand(time(NULL));

    while (1) {
        display_main_menu();
        printf("> Select a menu (or 0, q to quit): ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;
        if (strcmp(input_buffer, "\n") == 0 || strcmp(input_buffer, "0\n") == 0 ||
            strcmp(input_buffer, "q\n") == 0 || strcmp(input_buffer, "Q\n") == 0) {
            printf("Terminating Magrathea System.\n");
            break;
        }
        choice = atoi(input_buffer);
        switch (choice) {
            case 1: printf("\n(Audition Management is not yet implemented.)\n\n"); break;
            case 2: handle_training_menu(); break;
            case 3: printf("\n(Debut is not yet implemented.)\n\n"); break;
            default: printf("\nInvalid selection. Please try again.\n\n");
        }
    }
    return 0;
}


void display_main_menu() {
    printf("####################################\n");
    printf("#        Magrathea Main Menu       #\n");
    printf("####################################\n");
    printf("I. Audition Management\n");
    printf("II. Training\n");
    printf("III. Debut\n");
    printf("------------------------------------\n");
}

void handle_training_menu() {
    char choice_char;
    printf("\n--- [II. Training] ---\n");
    printf("Selected Stage: 2. Self-Management & Teamwork\n");
    printf("--------------------------------------------\n");
    printf("A. Mentoring\n");
    printf("0. Return to Main Menu\n");
    printf("> Select an option: ");
    scanf(" %c", &choice_char);
    while (getchar() != '\n'); 

    if (choice_char == 'A' || choice_char == 'a') {
        matchMentoring();
    }
}

void matchMentoring() {
    int trainee_data[NUM_TRAINEES][2];
    struct Mentor mentor_list[MAX_MENTORS];
    int num_mentors = 0;

    printf("\n--- [A. Mentoring] ---\n");

    initializeTrainees(trainee_data);
    printf("Step 1: Trainee data has been initialized.\n");

    inputMentors(mentor_list, &num_mentors);
    printf("Step 2: %d mentors have been registered.\n", num_mentors);

    randomMatching(trainee_data, mentor_list, num_mentors);
    printf("Step 3: Mentor-mentee matching is complete.\n");


    displayMatches(mentor_list, num_mentors, trainee_data);
}


int parseIntMember(const char* nickname) {
    int sum = 0;
    for (int i = 0; nickname[i] != '\0'; i++) {
        sum += nickname[i];
    }
    return sum;
}


int getRandomAbility() {
    return 100 + rand() % 901; 
}

void initializeTrainees(int data[][2]) {
    for (int i = 0; i < NUM_TRAINEES; i++) {
        data[i][0] = parseIntMember(MILLIWAYS_NICKNAMES[i]);
        data[i][1] = getRandomAbility();
    }
}

void inputMentors(struct Mentor list[], int* mentor_count) {
    printf("\n--- Register Mentors ---\n");
    do {
        printf("How many mentors to register? (1-%d): ", BONUS_MATCHING ? NUM_TRAINEES : MAX_MENTORS);
        scanf("%d", mentor_count);
        while (getchar() != '\n');
        if (BONUS_MATCHING && *mentor_count != NUM_TRAINEES) {
            printf("Error: For 1:1 matching, mentor count must be exactly %d.\n", NUM_TRAINEES);
        }
    } while (*mentor_count <= 0 || (BONUS_MATCHING && *mentor_count != NUM_TRAINEES) || (!BONUS_MATCHING && *mentor_count > MAX_MENTORS));

    for (int i = 0; i < *mentor_count; i++) {
        list[i].id = i + 1;
        list[i].mentee_list_number = 0; 
        printf("Enter name for Mentor ID %d: ", list[i].id);
        fgets(list[i].name, MAX_NAME_LEN, stdin);
        list[i].name[strcspn(list[i].name, "\n")] = 0; 
    }
}


void randomMatching(int trainees[][2], struct Mentor mentors[], int num_mentors) {
#if BONUS_MATCHING == 1

    printf("Info: Using bonus 1:1 matching algorithm.\n");

    for (int i = NUM_TRAINEES - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp_id = trainees[i][0];
        int temp_ability = trainees[i][1];
        trainees[i][0] = trainees[j][0];
        trainees[i][1] = trainees[j][1];
        trainees[j][0] = temp_id;
        trainees[j][1] = temp_ability;
    }

    for (int i = 0; i < num_mentors; i++) {
        mentors[i].mentee_list_number = trainees[i][0];
    }
#else
    
    printf("Info: Using simple modulo matching algorithm.\n");
    for (int i = 0; i < NUM_TRAINEES; i++) {
        int trainee_id_num = trainees[i][0];
        int target_mentor_index = trainee_id_num % num_mentors;
        
        if (mentors[target_mentor_index].mentee_list_number != 0) {

            printf("Warning: Mentor '%s' (ID %d) is being assigned a new mentee, replacing a previous one.\n", 
                   mentors[target_mentor_index].name, mentors[target_mentor_index].id);
        }
        mentors[target_mentor_index].mentee_list_number = trainee_id_num;
    }
#endif
}

void displayMatches(const struct Mentor mentors[], int num_mentors, int trainees[][2]) {
    printf("\n--- Mentor-Mentee Matching Results ---\n");
    printf("=======================================\n");
    printf("%-10s | %-20s | %-20s\n", "Mentor ID", "Mentor Name", "Assigned Mentee");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < num_mentors; i++) {
        printf("%-10d | %-20s | ", mentors[i].id, mentors[i].name);
        if (mentors[i].mentee_list_number == 0) {
            printf("%-20s\n", "No mentee assigned");
        } else {

            char mentee_nickname[MAX_NAME_LEN] = "Unknown";
            for (int j = 0; j < NUM_TRAINEES; j++) {
                if (trainees[j][0] == mentors[i].mentee_list_number) {
                    strcpy(mentee_nickname, MILLIWAYS_NICKNAMES[j]);
                    break;
                }
            }
            printf("%-20s\n", mentee_nickname);
        }
    }
    printf("=======================================\n\n");
}
