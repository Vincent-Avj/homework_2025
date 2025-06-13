#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define NUM_MAIN_MENU_ITEMS 3
#define NUM_TRAINING_STAGES 8
#define NUM_MEMBERS 4 // Same as NUM_TRAINEES in the previous problem
#define NUM_QUESTIONS 5
#define QUESTIONS_PER_SESSION 3
#define MAX_RESPONSES (NUM_MEMBERS * QUESTIONS_PER_SESSION)
#define MAX_LEN 256
#define MAX_RESPONSE_LEN 101 // 100 chars + null terminator


struct TraumaInfo {
    char nickname[MAX_LEN];
    int age;
    char trauma_desc[MAX_LEN];
};

struct Question {
    int id;
    const char* content; 
};

struct Response {
    char nickname[MAX_LEN];
    char question[MAX_LEN];
    char answer[MAX_RESPONSE_LEN];
};


const char* MILLIWAYS_NICKNAMES[NUM_MEMBERS] = {"Ariel", "Simba", "Belle", "Aladdin"};
const int MILLIWAYS_AGES[NUM_MEMBERS] = {19, 20, 18, 19}; 

const struct Question COUNSELING_QUESTIONS[NUM_QUESTIONS] = {
    {1, "In what situations have you experienced this trauma?"},
    {2, "How has this situation affected your daily life and emotions?"},
    {3, "How have you tried to overcome this trauma?"},
    {4, "What emotions do you associate with this trauma?"},
    {5, "What kind of support do you think is necessary to overcome this trauma?"}
};


void display_main_menu();
void handle_training_menu(struct TraumaInfo traumas[], struct Response responses[], int* response_count);
void overcomeTrauma(struct TraumaInfo traumas[], struct Response responses[], int* response_count);
void initializeTraumas(struct TraumaInfo traumas[]);
int find_member_by_nickname(const char* nickname, const struct TraumaInfo traumas[]);
void inputTraumaDescription(struct TraumaInfo traumas[]);
void startCounselingSession(struct TraumaInfo traumas[], struct Response responses[], int* response_count);
void viewCounselingResults(const struct TraumaInfo traumas[], const struct Response responses[], int response_count);


int main() {

    struct TraumaInfo member_traumas[NUM_MEMBERS];
    struct Response session_responses[MAX_RESPONSES];
    int response_count = 0;
    char input_buffer[100];
    int choice;

    srand(time(NULL));
    initializeTraumas(member_traumas);

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
            case 2: handle_training_menu(member_traumas, session_responses, &response_count); break;
            case 3: printf("\n(Debut is not yet implemented.)\n\n"); break;
            default: printf("\nInvalid selection. Please try again.\n\n");
        }
    }
    return 0;
}


void display_main_menu() {}

void handle_training_menu(struct TraumaInfo traumas[], struct Response responses[], int* response_count) {
    char choice_char;
    printf("\n--- [II. Training] --- \n");
    printf("1. Self-Management and Teamwork (Mentoring - Not Implemented in this file)\n");
    printf("2. Self-Management and Teamwork (Trauma Management)\n");
    printf("0. Return to Main Menu\n");
    printf("> Select an option: ");
    scanf(" %c", &choice_char);
    while (getchar() != '\n');

    if (choice_char == '2') {
        overcomeTrauma(traumas, responses, response_count);
    }
}


void overcomeTrauma(struct TraumaInfo traumas[], struct Response responses[], int* response_count) {
    char choice_char;
    while(1) {
        printf("\n--- [B. Trauma Management] ---\n");
        printf("1. Enter/Update Trauma Description\n");
        printf("2. Start/Resume Counseling Session\n");
        printf("3. View Counseling Results\n");
        printf("0. Return to Training Menu\n");
        printf("> Select an option: ");
        scanf(" %c", &choice_char);
        while (getchar() != '\n');

        if (choice_char == '0') break;

        switch(choice_char) {
            case '1': inputTraumaDescription(traumas); break;
            case '2': startCounselingSession(traumas, responses, response_count); break;
            case '3': viewCounselingResults(traumas, responses, *response_count); break;
            default: printf("Invalid option. Please try again.\n");
        }
    }
}

void initializeTraumas(struct TraumaInfo traumas[]) {
    for (int i = 0; i < NUM_MEMBERS; i++) {
        strcpy(traumas[i].nickname, MILLIWAYS_NICKNAMES[i]);
        traumas[i].age = MILLIWAYS_AGES[i];
        strcpy(traumas[i].trauma_desc, ""); 
    }
}

int find_member_by_nickname(const char* nickname, const struct TraumaInfo traumas[]) {
    for (int i = 0; i < NUM_MEMBERS; i++) {
        if (strcmp(nickname, traumas[i].nickname) == 0) return i;
    }
    return -1; 
}

void inputTraumaDescription(struct TraumaInfo traumas[]) {
    char nickname_buffer[MAX_LEN];
    while (1) {
        printf("\nEnter nickname to input trauma description for (or 0 to return): ");
        fgets(nickname_buffer, sizeof(nickname_buffer), stdin);
        nickname_buffer[strcspn(nickname_buffer, "\n")] = 0;
        if (strcmp(nickname_buffer, "0") == 0) break;

        int index = find_member_by_nickname(nickname_buffer, traumas);
        

        (index != -1)
            ? printf("Enter trauma description for %s: ", nickname_buffer)
            : printf("Error: Nickname not found. Please try again.\n");

        if (index != -1) {
            fgets(traumas[index].trauma_desc, MAX_LEN, stdin);
            traumas[index].trauma_desc[strcspn(traumas[index].trauma_desc, "\n")] = 0;
            printf("Trauma description for %s has been saved.\n", nickname_buffer);
        }
    }
}

void startCounselingSession(struct TraumaInfo traumas[], struct Response responses[], int* response_count) {
    char nickname_buffer[MAX_LEN];
    printf("\n--- Start Counseling Session ---\n");
    printf("Members with trauma entries:\n");
    for (int i = 0; i < NUM_MEMBERS; i++) {
        if (strlen(traumas[i].trauma_desc) > 0) printf("- %s\n", traumas[i].nickname);
    }
    printf("Enter nickname to start session with (or 0 to return): ");
    fgets(nickname_buffer, sizeof(nickname_buffer), stdin);
    nickname_buffer[strcspn(nickname_buffer, "\n")] = 0;
    if (strcmp(nickname_buffer, "0") == 0) return;

    int member_idx = find_member_by_nickname(nickname_buffer, traumas);
    if (member_idx == -1 || strlen(traumas[member_idx].trauma_desc) == 0) {
        printf("Error: Invalid nickname or no trauma description found.\n");
        return;
    }
    

    int answered_q_ids[QUESTIONS_PER_SESSION] = {0};
    int completed_count = 0;
    for(int i = 0; i < *response_count; i++) {
        if(strcmp(responses[i].nickname, nickname_buffer) == 0) {
            for(int j=0; j<NUM_QUESTIONS; j++) {
                if(strcmp(responses[i].question, COUNSELING_QUESTIONS[j].content) == 0){
                    answered_q_ids[completed_count++] = COUNSELING_QUESTIONS[j].id;
                }
            }
        }
    }

    if(completed_count >= QUESTIONS_PER_SESSION) {
        printf("\n%s has already completed their counseling session.\n", nickname_buffer);
        return;
    }

    printf("\nStarting session for %s. (%d/%d questions already answered)\n", nickname_buffer, completed_count, QUESTIONS_PER_SESSION);
    
    int questions_to_ask[NUM_QUESTIONS];
    int unasked_count = 0;
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        int already_answered = 0;
        for(int j=0; j<completed_count; j++){
            if(COUNSELING_QUESTIONS[i].id == answered_q_ids[j]){
                already_answered = 1;
                break;
            }
        }
        if(!already_answered) {
            questions_to_ask[unasked_count++] = i;
        }
    }
    

    for (int i = unasked_count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = questions_to_ask[i];
        questions_to_ask[i] = questions_to_ask[j];
        questions_to_ask[j] = temp;
    }

    int questions_needed = QUESTIONS_PER_SESSION - completed_count;
    for (int i = 0; i < questions_needed; i++) {
        int q_index = questions_to_ask[i];
        printf("\nQuestion %d of %d:\n%s\n", completed_count + i + 1, QUESTIONS_PER_SESSION, COUNSELING_QUESTIONS[q_index].content);
        
        do {
            printf("Your response (max 100 characters): ");
            fgets(responses[*response_count].answer, MAX_RESPONSE_LEN, stdin);
            responses[*response_count].answer[strcspn(responses[*response_count].answer, "\n")] = 0;

            if (strlen(responses[*response_count].answer) == 0) {
                printf("Error: Response cannot be empty. Please answer the question.\n");
            } else if (strlen(responses[*response_count].answer) > 100) {
                printf("Error: Response exceeds 100 characters. Please shorten your answer.\n");
            }
        } while (strlen(responses[*response_count].answer) == 0 || strlen(responses[*response_count].answer) > 100);

        strcpy(responses[*response_count].nickname, nickname_buffer);
        strcpy(responses[*response_count].question, COUNSELING_QUESTIONS[q_index].content);
        (*response_count)++;
    }
    printf("\nCounseling session for %s is now complete.\n", nickname_buffer);
}


void viewCounselingResults(const struct TraumaInfo traumas[], const struct Response responses[], int response_count) {
    char nickname_buffer[MAX_LEN];
    printf("\n--- View Counseling Results ---\n");
    printf("Members who have completed counseling:\n");

    char completed_members[NUM_MEMBERS][MAX_LEN];
    int completed_member_count = 0;
    for (int i = 0; i < response_count; i++) {
        int already_listed = 0;
        for (int j = 0; j < completed_member_count; j++) {
            if (strcmp(responses[i].nickname, completed_members[j]) == 0) {
                already_listed = 1;
                break;
            }
        }
        if (!already_listed) {
            strcpy(completed_members[completed_member_count++], responses[i].nickname);
        }
    }

    for(int i = 0; i < completed_member_count; i++) printf("- %s\n", completed_members[i]);
    
    printf("Enter nickname to view results (or 0 to return): ");
    fgets(nickname_buffer, sizeof(nickname_buffer), stdin);
    nickname_buffer[strcspn(nickname_buffer, "\n")] = 0;
    if (strcmp(nickname_buffer, "0") == 0) return;

    int member_idx = find_member_by_nickname(nickname_buffer, traumas);
    if(member_idx == -1) {
        printf("Error: Member not found.\n");
        return;
    }

    printf("\n--- Counseling Report for %s ---\n", traumas[member_idx].nickname);
    printf("Nickname: %s\n", traumas[member_idx].nickname);
    printf("Trauma: %s\n", traumas[member_idx].trauma_desc);
    printf("--------------------------------------\n");
    printf("Session Q&A:\n");

    for (int i = 0; i < response_count; i++) {
        if (strcmp(responses[i].nickname, nickname_buffer) == 0) {
            printf("\nQ: %s\n", responses[i].question);
            printf("A: %s\n", responses[i].answer);
        }
    }
    printf("--------------------------------------\n");
}
