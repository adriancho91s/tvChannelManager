#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

//Constants
#define UP 119
#define RIGHT 100
#define LEFT 97
#define DOWN 115
#define ESC 27
#define ENTER 10

/**
 * Struct to represent a TV program
 * @param prev The pointer to the previous program
 * @param timeSlot The time slot of the program
 * @param name The name of the program
 * @param duration The duration of the program
 * @param next The pointer to the next
*/
struct tvProgram {
    struct tvProgram *prev;
    char timeSlot[30];//This have the following format: DD:HH:MM
    char name[30];
    int duration;
    struct tvProgram *next;
};

/**
 * Struct to represent a TV channel
 * @param prev The pointer to the previous channel
 * @param name The name of the channel
 * @param number The number of the channel
 * @param firstProgram The pointer to the first program of the channel
 * @param next The pointer to the next channel
*/
struct tvChannel {
    struct tvChannel *prev;
    char name[30];
    int number;
    struct tvProgram *firstProgram;
    struct tvChannel *next;
};




// Function prototypes
struct tvChannel *findChannelAtPosition(struct tvChannel **first, int position);
void clearScreen();
void delay();
int counterChannels(struct tvChannel **first);
void addChannel(struct tvChannel **first, char name[30], int number);
void addProgram(struct tvChannel **first, int channelNumber, char *timeSlot, char *name, int duration);
void removeChannel(struct tvChannel **first, int number);
void deleteProgramList(struct tvProgram **first);
int getKey();
void navigateChannels(struct tvChannel **first);
struct tvChannel *searchChannel(struct tvChannel **first, char name[30], int number);
void printAllChannels(struct tvChannel **first, int printPrograms);
void printChannel(struct tvChannel *channel);
void printChannelWithPrograms(struct tvChannel *channel);
int verifyChannelNumber(struct tvChannel **first, int number);


// Implementation
int main() {
    struct tvChannel *first = NULL;

    char *options[] = {
            "Add a channel to the list of channels",
            "Add a program to a channel",
            "Query a information of a channel",
            "Delete a channel from the list of channels",
            "Delete all programs from a channel",
            "Enter to navigate mode",
            "Exit"
    };
    int numOptions = sizeof(options) / sizeof(options[0]);

    //Menu for the user
    int option;
    do {
        clearScreen();
        printf("\tWelcome to the linked list program\n\n");
        for (int i = 0; i < numOptions; i++) {
            printf("%d. %s\n", i + 1, options[i]);
        }
        fflush(stdin);
        printf("\nChoose an option: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                clearScreen();
                printf("You are going to add a channel\n");
                char name[30];
                int number;
                printf("Enter the number of the channel: ");
                scanf("%d", &number);
                if(verifyChannelNumber(&first, number)){
                    printf("The channel number already exists\n");
                    delay();
                    break;
                }
                fgetc(stdin);
                printf("Enter the name of the channel: ");
                fgets(name, 30, stdin);
                name[strcspn(name, "\n")] = 0;
                addChannel(&first, name, number);
                break;
            case 2:
                clearScreen();
                if (first == NULL) {
                    printf("There's no channels\n");
                    delay();
                    break;
                }
                printf("You are going to add a program to a channel\n");
                int channelNumber = 0;
                printAllChannels(&first, 0);
                printf("Enter the number of the channel: ");
                scanf("%d", &channelNumber);
                struct tvChannel *channel = searchChannel(&first, "", channelNumber);
                if (channel == NULL) {
                    printf("The channel was not found\n");
                    delay();
                    break;
                }
                char timeSlot[30];
                char nameProgram[30];
                int duration;
                printf("Enter the time slot of the program: ");
                scanf("%s", timeSlot);
                printf("Enter the name of the program: ");
                fgetc(stdin);
                fgets(nameProgram, 30, stdin);
                nameProgram[strcspn(nameProgram, "\n")] = 0;
                printf("Enter the duration of the program: ");
                scanf("%d", &duration);
                addProgram(&first, channelNumber, timeSlot, nameProgram, duration);
                break;
            case 3:
                clearScreen();
                if (first == NULL) {
                    printf("There's no channels\n");
                    delay();
                    break;
                }
                printf("You are going to query a channel\n");
                int channelToQuery = 0;
                printAllChannels(&first, 0);
                printf("Enter the number of the channel: ");
                scanf("%d", &channelToQuery);
                struct tvChannel *channelQuery = searchChannel(&first, "", channelToQuery);
                if (channelQuery == NULL) {
                    printf("The channel was not found\n");
                    delay();
                    break;
                }
                clearScreen();
                printChannel(channelQuery);
                printf("Do you want to see the programs of the channel? (y/n): ");
                char answer;
                scanf(" %c", &answer);
                if (answer == 'y') {
                    printChannelWithPrograms(channelQuery);
                }

                printf("Press enter to continue\n");
                fgetc(stdin);
                fgetc(stdin);
                break;
            case 4:
                clearScreen();
                if (first == NULL) {
                    printf("The linked list is empty\n");
                    delay();
                    break;
                }
                printf("\x1b[31m You are going to delete a channel \x1b[0m\n");
                delay();
                printAllChannels(&first, 0);
                int channelNumberToDelete = 0;
                printf("Enter the number of the channel to delete: ");
                scanf("%d", &channelNumberToDelete);
                removeChannel(&first, channelNumberToDelete);
                printf("Press enter to continue\n");
                fgetc(stdin);
                fgetc(stdin);
                break;
            case 5:
                clearScreen();
                if (first == NULL) {
                    printf("The linked list is empty\n");
                    delay();
                    break;
                }
                printf("\x1b[31m You are going to delete all programs from a channel \x1b[0m\n");
                delay();
                printAllChannels(&first, 1);
                int channelProgramToDelete = 0;
                printf("Enter the number of the channel to delete all programs: ");
                scanf("%d", &channelProgramToDelete);
                struct tvChannel *channelProgram = searchChannel(&first, "", channelProgramToDelete);
                if (channelProgram == NULL) {
                    printf("The channel was not found\n");
                    delay();
                    break;
                }
                deleteProgramList(&channelProgram->firstProgram);
                printf("Press enter to continue\n");
                fgetc(stdin);
                fgetc(stdin);
                break;
            case 6:
                clearScreen();
                if (first == NULL) {
                    printf("The linked list is empty\n");
                    delay();
                    break;
                }
                navigateChannels(&first);
                printf("Press enter to continue\n");
                fgetc(stdin);
                break;
            case 7:
                printf("Exiting the program\n");
                struct tvChannel *temp = first;
                do {
                    struct tvChannel *nextChannel = temp->next;
                    removeChannel(&first, temp->number);
                    temp = nextChannel;
                } while (temp != first);
                break;
            default:
                printf("Invalid option\n");
                break;
        }

    } while (option != numOptions);
    return 0;
}

/**
 * Add a program to a channel order by time slot
 * @param channelNumber The number of the channel
 * @param timeSlot The time slot of the program
 * @param name The name of the program
 * @param duration The duration of the program
 * @return void
 * */
void addProgram(struct tvChannel **first, int channelNumber, char *timeSlot, char *name, int duration){
    struct tvChannel *channel = searchChannel(first, "", channelNumber);
    if (channel == NULL) {
        printf("The channel was not found\n");
        delay();
        return;
    }
    struct tvProgram *newProgram = (struct tvProgram *) malloc(sizeof(struct tvProgram));
    if (newProgram == NULL) {
        printf("Memory allocation error\n");
        return;
    }
    strcpy(newProgram->timeSlot, timeSlot);
    strcpy(newProgram->name, name);
    newProgram->duration = duration;
    newProgram->next = NULL;
    newProgram->prev = NULL;
    if (channel->firstProgram == NULL) {
        channel->firstProgram = newProgram;
    } else {
        struct tvProgram *temp = channel->firstProgram;
        struct tvProgram *prev = NULL;
        while (temp != NULL && strcmp(temp->timeSlot, timeSlot) < 0) {
            prev = temp;
            temp = temp->next;
        }
        if (temp == NULL) {
            prev->next = newProgram;
            newProgram->prev = prev;
        } else if (prev == NULL) {
            newProgram->next = temp;
            temp->prev = newProgram;
            channel->firstProgram = newProgram;
        } else {
            newProgram->next = temp;
            newProgram->prev = prev;
            prev->next = newProgram;
            temp->prev = newProgram;
        }
    }
}

/**
 * Add a channel to the list of channels in the tv guide order by number
 * @param first The head pointer to the first channel
 * @param name The name of the channel
 * @param number The number of the channel
 * @return void
 * */
void addChannel(struct tvChannel **first, char name[30], int number) {
    struct tvChannel *newChannel = (struct tvChannel *) malloc(sizeof(struct tvChannel));
    if (newChannel == NULL) {
        printf("Memory allocation error\n");
        return;
    }
    strcpy(newChannel->name, name);
    newChannel->number = number;
    newChannel->next = NULL;
    newChannel->prev = NULL;
    if (*first == NULL) {
        *first = newChannel;
        newChannel->next = newChannel;
        newChannel->prev = newChannel;
    } else {
        struct tvChannel *temp = *first;
        while (temp->next != *first && temp->number < number) {
            temp = temp->next;
        }
        if (temp->number < number) {
            temp->next = newChannel;
            newChannel->prev = temp;
            newChannel->next = *first;
            (*first)->prev = newChannel;
        } else {
            newChannel->next = temp;
            newChannel->prev = temp->prev;
            temp->prev->next = newChannel;
            temp->prev = newChannel;
        }
    }
}

/**
 * Delete tv program list
 * @return void
 * */
void deleteProgramList(struct tvProgram **first) {
    struct tvProgram *temp = *first;
    while (temp != NULL) {
        struct tvProgram *aux = temp;
        temp = temp->next;
        free(aux);
    }
    *first = NULL;
}

/**
 * Get the key pressed by the user
 * @return int The key pressed by the user
 * */
int getKey() {
    struct termios oldTerm, newTerm;
    int option;

    tcgetattr(STDIN_FILENO, &oldTerm);
    newTerm = oldTerm;
    newTerm.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);

    option = getchar(); //Get Key

    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);

    return option;
}


/**
 * Navigate through the tv channels list in a circular way, but when user press left or right, it must go to programs list
 * @param head pointer to the first channel
 * @return void
 * */
void navigateChannels(struct tvChannel **first) {
    if (*first == NULL) {
        printf("There's no channels\n");
        delay();
        return;
    }
    struct tvChannel *current = *first;
    struct tvProgram *currentProgram = NULL;
    int position = 1;
    int option = 0;
    printf("Press ESC to exit from navigate channels mode\n");
    delay();
    fflush(stdin);
    do {
        clearScreen();
        if (currentProgram != NULL) {
            printf("╔═══════════╦══════════════════════════════╦══════════╗\n");
            printf("║ %-10s║%-30s║%-10s║\n", "Time slot", "Program name", "Duration");
            printf("╠═══════════╬══════════════════════════════╬══════════════════════════║\n");
            printf("║ %-10s║%-30s║%-10d║\n", currentProgram->timeSlot, currentProgram->name, currentProgram->duration);
            printf("╚═══════════╩══════════════════════════════╩══════════╝\n");
        } else {
            printf("╔════════╦════════════════════════════════╗\n");
            printf("║ %-7s║%-30s║\n", "Number", "Channel name");
            printf("╠════════╬════════════════════════════════║\n");
            printf("║ %-7d║%-30s║\n", current->number, current->name);
            printf("╚════════╩════════════════════════════════╝\n");
        }

        printf("\x1b[31m Press ESC to exit from navigate channels mode \x1b[0m\n");
        printf("\x1b[33m Press W to go to the next channel \x1b[0m\n");
        printf("\x1b[33m Press S to go to the previous channel \x1b[0m\n");
        printf("\x1b[33m Press A to go to the previous program \x1b[0m\n");
        printf("\x1b[33m Press D to go to the next program \x1b[0m\n");
        option = getKey();
        switch (option) {
            case UP:
                current = current->next != NULL ? current->next : *first;
                currentProgram = NULL;
                position++;
                break;
            case DOWN:
                current = current->prev != NULL ? current->prev : *first;
                currentProgram = NULL;
                position--;
                break;
            case LEFT:
                if (currentProgram != NULL && currentProgram->prev != NULL) {
                    currentProgram = currentProgram->prev;
                } else {
                    clearScreen();
                    printf("No more programs to the left\n");
                    currentProgram = NULL;
                    delay();
                }
                break;
            case RIGHT:
                if (currentProgram != NULL && currentProgram->next != NULL) {
                    currentProgram = currentProgram->next;
                } else {
                    clearScreen();
                    printf("End of programs\n");
                    delay();
                }
                break;
            case ESC:
                break;
            default:
                clearScreen();
                printf("Invalid key\n");
                delay();
                break;
        }

    } while (option != ESC);
}


/**
* Print channel's details in the console with their programs
* @param channel The channel to print
* @return void
*/
void printChannel(struct tvChannel *channel) {
    printf("╔════════╦═══════════════╗\n");
    printf("║ %-7s║%-15s║\n", "Number", "Channel name");
    printf("╠════════╬═══════════════║\n");
    printf("║ %-7d║%-15s║\n", channel->number, channel->name);
    printf("╚════════╩═══════════════╝\n");
}

/**
 * Print channel's details in the console with their programs
 * @param channel The channel to print
 * @return void
 * */
void printChannelWithPrograms(struct tvChannel *channel) {
    printf("╔════════╦══════════════╗\n");
    printf("║ %-7s║%-15s║\n", "Number", "Channel name");
    printf("╠════════╬══════════════║\n");
    printf("║ %-7d║%-15s║\n", channel->number, channel->name);
    printf("╚════════╩══════════════╝\n");

    struct tvProgram *temp = channel->firstProgram;
    if (temp == NULL) {
        printf("There's no programs\n");
        return;
    }
    printf("Programs:\n");
    printf("╔══════════╦═══════════════╦═════════╗\n");
    printf("║ %-9s║%-15s║%-9s║\n", "Time slot", "Program name", "Duration");
    printf("╠══════════╬═══════════════╬═════════╣\n");
    do {
        printf("║ %-9s║%-15s║%-9d║\n", temp->timeSlot, temp->name, temp->duration);
        temp = temp->next;
    } while (temp != NULL);
    printf("╚══════════╩═══════════════╩═════════╝\n");
}

/**
 * Search a channel in the circular doubly linked list
 * @param first The pointer to the first channel
 * @param name The name of the channel to find
 * @param number The number of the channel to find
 * @return struct tvChannel* The channel found or NULL if it was not found
 * */
struct tvChannel *searchChannel(struct tvChannel **first, char name[30], int number) {
    struct tvChannel *temp = *first;
    if (temp == NULL) {
        return NULL;
    }
    do {
        if ((strcmp(temp->name, name) == 0) || (temp->number == number)) {
            return temp;
        }
        temp = temp->next;
    } while (temp != *first);
    return NULL;
}

/**
 * Clear the screen
 * @return void
 * */
void clearScreen() {
#ifdef WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

/**
 * Delay the program
 * @return void
 * */
void delay() {
#ifdef WINDOWS
    system("timeout 1 > nul");
#else
    system("sleep 1");
#endif
}


/**
 * Find a channel at a given position in the guide
 * @param position The position of the channel to find
 * @return struct tvChannel* The channel at the given position
 * */
struct tvChannel *findChannelAtPosition(struct tvChannel **first, int position) {
    if (position < 0) {
        printf("Invalid position\n");
        return NULL;
    }
    struct tvChannel *temp = *first;
    int i = 1;
    while (temp != NULL && i < position) {
        temp = temp->next;
        i++;
    }
    return temp;
}

/**
 * Count the number of channels
 * @param first The pointer to the first channel
 * @return int The number of channels
 * */
int counterChannels(struct tvChannel **first) {
    struct tvChannel *temp = *first;
    int counter = 0;
    //Take into account that the linked list is circular and the start point is the head
    do {
        counter++;
        temp = temp->next;
    } while (temp != *first);
    return counter;
}


/**
 * Remove a channel from the list of channels
 * @param first The pointer to the first channel
 * @param number The number of the channel to remove
 * @return void
 * */
void removeChannel(struct tvChannel **first, int number) {
    struct tvChannel *channel = searchChannel(first, "", number);
    if (channel == NULL) {
        printf("The channel was not found\n");
        delay();
        return;
    }
    deleteProgramList(&channel->firstProgram);
    if (channel->next == channel) {
        free(channel);
        *first = NULL;
    } else {
        channel->prev->next = channel->next;
        channel->next->prev = channel->prev;
        if (channel == *first) {
            *first = channel->next;
        }
        free(channel);
    }
}

/**
 * Print all channels in the linked list
 * @param first The pointer to the first channel
 * @return void
 * */
void printAllChannels(struct tvChannel **first, int printPrograms) {
    struct tvChannel *temp = *first;
    if (temp == NULL) {
        printf("There's no channels\n");
        delay();
        return;
    }
    do {
        if (printPrograms) {
            printChannelWithPrograms(temp);
        } else {
            printChannel(temp);
        }
        temp = temp->next;
    } while (temp != *first);
}

/**
 * Verify if channel number exits 
 * @param first The pointer to the first channel
 * @param number The number of the channel to verify
*/
int verifyChannelNumber(struct tvChannel **first, int number){
    struct tvChannel *temp = *first;
    if (temp == NULL) {
        return 0;
    }
    do {
        if (temp->number == number) {
            return 1;
        }
        temp = temp->next;
    } while (temp != *first);
    return 0;
}