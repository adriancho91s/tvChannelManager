#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define UP 119
#define RIGHT 100
#define LEFT 97
#define DOWN 115
#define ESC 27
#define ENTER 10

//We´re going to convert this program to a channel manager
// it must have the following fields:
// Tv Channel Name, number, and it must have to order the channels by number
// the list of channels will be a circular doubly linked list with a pointer to the first channel
// each channel must have a list of programs that will be a doubly linked list
// with the following fields: time slot, program name, duration, and a pointer to the next program

struct tvProgram {
    struct tvProgram *prev;
    char timeSlot[30];
    struct tvProgram *next;
};


struct tvChannel {
    struct tvChannel *prev;
    char name[30];
    int number;
    struct tvPrograms *firstProgram;
    struct tvChannel *next;
};




// Function prototypes
struct node *findNodeAtPosition(struct node **first, int position);
void print(struct node **first, int order);
void clearScreen();
void delay();
int counterChannels(struct node **first);
void addChannel(struct tvChannel **first, char name[30], int number);
void deleteProgramList(struct tvProgram **first);
int getKey();
void navigateChannels(struct tvChannel **first);
struct tvChannel *searchChannel(struct tvChannel **first, char name[30], int number);

// Implementation
int main() {
    struct tvChannel *first = NULL;

    char *options[] = {
            "Add a channel to the list of channels",
            "Add a program to a channel",
            "Query a information of a channel",
            "Delete a channel from the list of channels",
            "Delete all programs from a channel",
            "Show the channels guide",
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
                //Add a program to a channel
                break;
            case 3:
                clearScreen();
                if (first == NULL) {
                    printf("There's no channels\n");
                    delay();
                    break;
                }
                //Query a information of a channel
                struct node *temp = binarySearch(&first, name, surname);
                if (temp == NULL) {
                    printf("The contact was not found\n");
                    delay();
                    break;
                }
                editContact(&temp);
                printf("Press enter to continue\n");
                fgetc(stdin);
                free(nameToFind);
                free(surnameToFind);
                break;
            case 4:
                clearScreen();
                if (first == NULL) {
                    printf("The linked list is empty\n");
                    delay();
                    break;
                }
                int order = 0;
                printf("Choose the order (0 ASC, 1 DESC): ");
                scanf("%d", &order);
                print(&first, order);
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
                print(&first, 0);
                printf("Press enter to continue\n");
                fgetc(stdin);
                fgetc(stdin);
                break;
            case 6:
                deleteList(&first);
                break;
            default:
                printf("Invalid option\n");
                break;
        }

    } while (option != numOptions);
    return 0;
}

/*
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

/*
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

/*
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


/*
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
    int position = 1;
    int option = 0;
    printf("Press ESC to exit from navigate channels mode\n");
    delay();
    fflush(stdin);
    do {
        clearScreen();
        printf("╔═══════╦═══════════════╗\n");
        printf("║ %-7s║%-15s║\n", "Number", "Channel name");
        printf("╠════╬═══════════════║\n");
        printf("║ %-7d║%-15s║\n", current->number, current->name);
        printf("╚═══════╩═══════════════╝\n");


        printf("\x1b[31m Press ESC to exit from navigate channels mode \x1b[0m\n");
        printf("\x1b[33m Press W to go to the next channel \x1b[0m\n");
        printf("\x1b[33m Press S to go to the previus channel \x1b[0m\n");
        printf("\x1b[33m Press A to go to the previus program \x1b[0m\n");
        printf("\x1b[33m Press D to go to the next program \x1b[0m\n");
        option = getKey();
        switch (option) {
            case UP:
                if (current->next != NULL) {
                    current = current->next;
                    position++;
                } else {
                    clearScreen();
                    printf("This is the end of the database\n");
                    delay();
                }
                break;
            case LEFT:
                //Navigate to the previus program
            case DOWN:
                if (current->prev != NULL) {
                    current = current->prev;
                    position--;
                }
            case RIGHT:
                //Navigate to the next program
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


/*
 * Print the linked list
 * @param first The pointer to the pointer of the first node in the linked list
 * @param to print the doubly linked list ASC or DESC if 0 ASC, 1 DESC
 * @return void
 * */
void print(struct node **first, int order) {
    struct node *temp = *first;
    int position = 1;
    if (temp == NULL) {
        printf("The linked list is empty\n");
        return;
    }
    if (order == 0) {
        printf("Printing the linked list in ASC order\n");

        printf("╔════╦═══════════════╦═══════════════╦═══════════════╦═══════════════╦══════════════════════════════╦══════════╗\n");
        printf("║ %-3s║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", "Pos", "Name", "Surname", "Phone", "WhatsApp", "Email", "Status");
        printf("╠════╬═══════════════╬═══════════════╬═══════════════╬═══════════════╬══════════════════════════════╬══════════║\n");
        while (temp != NULL) {
            if (temp->next == NULL) {
                printf("║ %-3d║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", position, temp->name, temp->surname, temp->phone, temp->whatsApp, temp->email, temp->status);
                printf("╚════╩═══════════════╩═══════════════╩═══════════════╩═══════════════╩══════════════════════════════╩══════════╝\n");
            } else {
                printf("║ %-3d║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", position, temp->name, temp->surname, temp->phone, temp->whatsApp, temp->email, temp->status);
            }
            temp = temp->next;
            position++;
        }
    } else {
        temp = findNodeAtPosition(first, counterChannels(first));
        printf("Printing the linked list in DESC order\n");
        printf("╔════╦═══════════════╦═══════════════╦═══════════════╦═══════════════╦══════════════════════════════╦══════════╗\n");
        printf("║ %-3s║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", "Pos", "Name", "Surname", "Phone", "WhatsApp", "Email", "Status");
        printf("╠════╬═══════════════╬═══════════════╬═══════════════╬═══════════════╬══════════════════════════════╬══════════║\n");
        while (temp != NULL) {
            if (temp->prev == NULL) {
                printf("║ %-3d║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", position, temp->name, temp->surname, temp->phone, temp->whatsApp, temp->email, temp->status);
                printf("╚════╩═══════════════╩═══════════════╩═══════════════╩═══════════════╩══════════════════════════════╩══════════╝\n");
            } else {
                printf("║ %-3d║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", position, temp->name, temp->surname, temp->phone, temp->whatsApp, temp->email, temp->status);
            }
            temp = temp->prev;
            position++;
        }
    }

}

int comparteString(char string1[30], char string2[30]) {
    int i = 0;
    for (i = 0; i < 30; i++) {
        printf("String1: %c\n", string1[i]);
        printf("String2: %c\n", string2[i]);
        if (string1[i] != string2[i]) {
            return 0;
        }
    }
    return 1;
}

/*
 * Search a channel in the linked list by name or number
 * @param first The pointer to the first channel
 * @param name The name of the channel to find
 * @param number The number of the channel to find
 * @return struct node* The node found in the linked list
 * */
struct tvChannel *searchChannel(struct tvChannel **first, char name[30], int number) {
    struct tvChannel *temp = *first;
    if (temp == NULL) {
        return NULL;
    }
    do {
        if (comparteString(temp->name, name) == 1 || temp->number == number) {
            return temp;
        }
        temp = temp->next;
    } while (temp != *first);
    return NULL;
}

/*
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

/*
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


/*
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

/*
 * Count the number of channels
 * @param first The pointer to the first channel
 * @return int The number of channels
 * */
int counterChannels(struct node **first) {
    struct tvChannel *temp = *first;
    int counter = 0;
    //Take into account that the linked list is circular and the start point is the head
    do {
        counter++;
        temp = temp->next;
    } while (temp != *first);
    return counter;
}
