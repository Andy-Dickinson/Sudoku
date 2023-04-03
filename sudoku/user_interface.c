#include "sudoku.h"
#include "user_interface.h"
#include "generate_board.h"

char hard[] = "Hard";
char medium[] = "Medium";
char easy[] = "Easy";
char* difficulty = &medium[0];
int insert_row;
int insert_col;
int insert_value;


// prints main menu
// loops menu until enter is pressed with no input
// will return to main menu after a game has been finished
void menu() {
    SIZE = 9;
    bool invalid = false;
    char input[2];
    timer_setting = false;
    colour_aid_setting = false;

    do {
        system("cls");
        fflush(stdin);

        printf("**********************  SUDOKU  **********************\n");
        printf("                      MAIN  MENU\n");
        printf("\n\n");
        printf("1. Start new game - %dx%d %s\n", SIZE, SIZE, difficulty);
        printf("2. Continue saved game\n");
        printf("3. Watch playthrough\n");
        printf("4. Settings\n");
        // if options are added/removed, change max flag passed to parse_main_input
        printf("\nPress enter with no input to quit\n\n");
        if(invalid){
            printf("%c is not a valid input\n", input[0]);
        } else{
            printf("\n");
        }

        invalid = get_input(&input[0]);

        if(invalid == 0) {
            invalid = parse_main_input(atoi(input), 4);
        }

    } while (input[0] != '\n');
}


// prints settings menu
void settings() {
    bool invalid = false;
    char input[2];
    char on[] = "ON";
    char off[] = "OFF";
    char* timer_text;
    char* colour_aid_text;

    do {
            if(timer_setting) {
                timer_text = &on[0];
            }else {
                timer_text = &off[0];
            }

            if(colour_aid_setting) {
                colour_aid_text = &on[0];
            }else {
                colour_aid_text = &off[0];
            }

            system("cls");

            printf("**********************  SUDOKU  **********************\n");
            printf("                       SETTINGS\n");
            printf("\n\n");
            printf("1. Timer - %s\n", timer_text);
            printf("2. Valid input colour aid - %s\n", colour_aid_text);
            printf("3. Change board size - %dx%d\n", SIZE, SIZE);
            printf("4. Change difficulty - %s\n", difficulty);
            // if options are added/removed, change max flag passed to parse_settings_input
            printf("\nPress enter to return to main menu\n\n");
            if(invalid){
                printf("%c is not a valid input\n", input[0]);
            } else{
                printf("\n");
            }

            invalid = get_input(&input[0]);

            if(invalid == 0) {
                invalid = parse_settings_input(atoi(input), 4);
            }

        } while (input[0] != '\n');
}


// prints menu for changing board size
void change_board_size(){
    bool invalid = false;
    char input[2];

    do {
            system("cls");

            printf("**********************  SUDOKU  **********************\n");
            printf("                      BOARD SIZE\n");
            printf("                          %dx%d\n", SIZE, SIZE);
            printf("\n");
            printf("1. 4x4\n");
            printf("2. 6x6\n");
            printf("3. 9x9\n");
            printf("4. 12x12\n");
            printf("5. 16x16\n");
            // if options are added/removed, change max flag passed to parse_size_input
            printf("\nPress enter to return to main menu\n\n");
            if(invalid){
                printf("%c is not a valid input\n", input[0]);
            } else{
                printf("\n");
            }

            invalid = get_input(&input[0]);

            if(invalid == 0) {
                invalid = parse_size_input(atoi(input), 5);
            }

            // returns to settings menu if num input was valid and setting changed
            if(invalid == 0) {
                break;
            }

        } while (input[0] != '\n');
}


// prints menu for changing difficulty
void change_difficulty() {
    bool invalid = false;
    char input[2];

    do {
            system("cls");

            printf("**********************  SUDOKU  **********************\n");
            printf("                      DIFFICULTY\n");
            if(difficulty != &medium[0]){
                printf(" ");
            }
            printf("                        %s\n", difficulty);
            printf("\n");
            printf("1. Easy\n");
            printf("2. Medium\n");
            printf("3. Hard\n");
            // if options are added/removed, change max flag passed to parse_size_input
            printf("\nPress enter to return to main menu\n\n");
            if(invalid){
                printf("%c is not a valid input\n", input[0]);
            } else{
                printf("\n");
            }

            invalid = get_input(&input[0]);

            if(invalid == 0) {
                invalid = parse_difficulty_input(atoi(input), 3);
            }

            // returns to settings menu if num input was valid and setting changed
            if(invalid == 0) {
                break;
            }

        } while (input[0] != '\n');
}


// determines if integer entered is valid and if so changes variable
int parse_difficulty_input(int select, int select_max) {
    bool invalid = false;

    if(select>select_max){
        select = 0;
    }

    switch(select) {
    case 1:
        difficulty = &easy[0];
        break;
    case 2:
        difficulty = &medium[0];
        break;
    case 3:
        difficulty = &hard[0];
        break;
    default:
        invalid = true;
        break;
    }
    return invalid;
}


// determines if integer entered is valid and if so changes variable
int parse_size_input(int select, int select_max) {
    bool invalid = false;
    if(select>select_max){
        select = 0;
    }

    switch(select) {
    case 1:
        SIZE = 4;
        break;
    case 2:
        SIZE = 6;
        break;
    case 3:
        SIZE = 9;
        break;
    case 4:
        SIZE = 12;
        break;
    case 5:
        SIZE = 16;
        break;
    default:
        invalid = true;
        break;
    }
    return invalid;
}


// determines if integer entered is valid and if so changes variable
int parse_settings_input(int select, int select_max) {
    bool invalid = false;
    if(select>select_max){
        select = 0;
    }

    switch(select) {
    case 1:
        if(timer_setting){
            timer_setting = false;
        }else {
            timer_setting = true;
        }
        break;
    case 2:
        if(colour_aid_setting) {
            colour_aid_setting = false;
        }else {
            colour_aid_setting = true;
        }
        break;
    case 3:
        change_board_size();
        break;
    case 4:
        change_difficulty();
        break;
    default:
        invalid = true;
        break;
    }
    return invalid;
}


// determines if integer entered is valid and if so changes variable
int parse_main_input(int select, int select_max) {
    bool invalid = false;
    if(select>select_max){
        select = 0;
    }

    switch(select) {
    case 1:
        create_new_game();
        break;
    case 2:
        printf("load game\n");/////////////////////////////////////////////////////////////////////////////////////////////
        break;
    case 3:
        printf("watch playthrough\n");///////////////////////////////////////////////////////////////////////////////////
        break;
    case 4:
        settings();
        break;
    default:
        invalid = true;
        break;
    }
    return invalid;
}


// gets user input and returns invalid boolean
// true if first character entered is not a number
int get_input(char* input) {
    bool invalid = true;

    printf("ENTER SELECTION:\n");

    fflush(stdin);
    fgets(input, sizeof(input), stdin);

    if(isdigit(input[0])){
        invalid = false;
        fflush(stdin);
    }else if(input[0] != '\n'){
        invalid = true;
        fflush(stdin);
    }

    return invalid;
}


// sets num_to_remove based on difficulty set before generating a new game
void set_num_to_remove(){
    num_to_remove = SIZE*SIZE; // attempts to remove maximum when set to hard

    switch(*difficulty) {
    case 'E':
        switch(SIZE){
        case 4:
            num_to_remove = 6;
            break;
        case 6:
            num_to_remove = 15;
            break;
        case 9:
            num_to_remove = 49;
            break;
        case 12:
            num_to_remove = 60;
            break;
        case 16:
            num_to_remove = 90;
            break;
        }
        break;
    case 'M':
        switch(SIZE){
        case 4:
            num_to_remove = 8;
            break;
        case 6:
            num_to_remove = 20;
            break;
        case 9:
            num_to_remove = 53;
            break;
        case 12:
            num_to_remove = 70;
            break;
        case 16:
            num_to_remove = 105;
            break;
        }
        break;
    }
}


// sets variables prior to generating board
void create_new_game() {
    set_num_to_remove();
    solved = false;

    printf("\nGenerating board...\n");
    generate_grids();

    play_game();

    deallocate_grid(&solution_grid);
    deallocate_grid(&player_grid);
    deallocate_grid(&original_player_grid);
}


// displays game board until solved
void play_game() {
    while(!solved) {
        get_grid_ref();

        if(player_grid[insert_row][insert_col] == 0 && insert_value != 0){
            grid_empties--;
        } else if (player_grid[insert_row][insert_col] != 0 && insert_value == 0){
            grid_empties++;
        }

        player_grid[insert_row][insert_col] = insert_value;

        if(grid_empties == 0){
            check_grid_solved();
        }
    }



    system("cls");
    printf("**********************  SUDOKU  **********************\n\n");

    display_board(player_grid);
    printf("\n\nCongratulations!!!\n");
    printf("Enter any key to return to the main menu\n");

    getchar();
}


void check_grid_solved() {


    for(int row=0; row<SIZE; row++){
        for(int col=0; col<SIZE; col++){
            if(solution_grid[row][col] != player_grid[row][col]){
                return;
            }
        }
    }
    solved = true;
}


void get_grid_ref() {
    bool invalid = false;
    char input[7];

    do {
        system("cls");
        printf("**********************  SUDOKU  **********************\n\n");

        display_board(player_grid);

        printf("\n");
        if(invalid) {
            for(int i=0; i<sizeof(input); i++){
                if(input[i] == '\n'){
                    break;
                } else {
                    printf("%c", input[i]);
                }
            }
            printf(" is not a valid input\n");
        } else{
            printf("\n");
        }
        printf("Please enter grid reference and value to insert e.g.: A4 3\n");

        fflush(stdin);
        fgets(&input[0], sizeof(input), stdin);

        invalid = check_insert_input(&input[0]);

    } while(invalid);
}



// checks if users input is valid
int check_insert_input(char* in) {
    char* input = in;
    bool invalid = false;

    // checks column input
    if(isalpha(input[0])){
        insert_col = input[0];

        //converts to int grid index
        if(insert_col>64 && insert_col<91) {
            // uppercase
            insert_col-=65;
        } else {
            // lowercase
            insert_col-=97;
        }

        // checks col is within range
        if(insert_col>=SIZE) {
            invalid = true;
            return invalid;
        }

        input++;
    } else {
        invalid = true;
        return invalid;
    }

    // checks row input
    if(isdigit(input[0])){
        // gets row input and converts to int grid index
        if(isdigit(input[1]) && input[2] == ' '){
            if(input[0] == '0'){
                invalid = true;
                return invalid;
            }
            insert_row = (input[0]+input[1]) - '0';
            insert_row -= 40;
            input+= 3;
        } else if(input[1] == ' ') {
            insert_row = (input[0]) - '0';
            insert_row -= 1;
            input+=2;
        } else {
            invalid = true;
            return invalid;
        }

        // checks row is in range
        if(insert_row>=SIZE) {
            invalid = true;
            return invalid;
        }
    } else {
        invalid = true;
        return invalid;
    }

    // checks insert_value input
    if(isdigit(input[0])){
        // gets insert_value input and converts to int
        if(isdigit(input[1])){
            if(input[0] == '0'){
                invalid = true;
                return invalid;
            }
            insert_value = (input[0]+input[1]) - '0';
            insert_value -= 39;
        } else{
            insert_value = (input[0]) - '0';
        }

        // checks row is in range
        if(insert_value>SIZE) {
            invalid = true;
            return invalid;
        }
    }else {
        printf("invalid\n");
        invalid = true;
        return invalid;
    }

    // checks if selected cell is available for manipulation
    if(original_player_grid[insert_row][insert_col] != 0){
        invalid = true;
    }

    return invalid;
}
