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
int previous_value;
bool quit_current_game;
moves_stack undo_stack;
moves_stack redo_stack;
bool undo_operation;


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
                invalid = parse_settings_input(atoi(input), 4, false);
            }

        } while (input[0] != '\n');
}


// in game settings menu
void ingame_settings() {
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
            printf("3. Save game\n");
            printf("4. Quit to main menu\n");
            // if options are added/removed, change max flag passed to parse_settings_input
            printf("\nPress enter to return to gameplay\n\n");
            if(invalid){
                printf("%c is not a valid input\n", input[0]);
            } else{
                printf("\n");
            }

            invalid = get_input(&input[0]);

            if(invalid == 0) {
                invalid = parse_settings_input(atoi(input), 4, true);
            }

            if(atoi(input) == 4){
                input[0] = '\n';
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
int parse_settings_input(int select, int select_max, bool ingame) {
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
        if(ingame){
            save();
        }else{
            change_board_size();
        }
        break;
    case 4:
        if(ingame){
            quit_current_game = true;
        }else{
            change_difficulty();
        }
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
        load_game();
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
    // based on game difficulty set
    set_num_to_remove();

    // ensures variables are set correctly before starting
    quit_current_game = false;
    solved = false;

    printf("\nGenerating board...\n");
    generate_grids();

    init_stack(SIZE*SIZE, &undo_stack);
    init_stack(SIZE*SIZE, &redo_stack);

    play_game();

    deallocate_grid(&solution_grid);
    deallocate_grid(&player_grid);
    deallocate_grid(&original_player_grid);
    deallocate_stacks();
}


// displays game board until solved
void play_game() {
    while(!solved && !quit_current_game) {
        /* global variables initialised on each loop to stop past values getting written to grid by mistake
         e.g. if a value was set, followed by the grid reference of another cell but without setting a value
         followed by entering and exiting the menu */

        insert_row = -1;
        insert_col = -1;
        insert_value = -1;

        get_grid_ref();

        // exits loop if quit selected from in game settings menu
        if(quit_current_game){
            break;
        }


        /* Stops possibly trying to insert a value when insert_value has not been set
        e.g. after going into settings and returning to gameplay without specifying a grid value
        then should continue loop */
        if(insert_value>=0){
            if(player_grid[insert_row][insert_col] == 0 && insert_value != 0){
                grid_empties--;
            } else if (player_grid[insert_row][insert_col] != 0 && insert_value == 0){
                grid_empties++;
            }

            // stops undo/redo pushing back to stack
            if(!undo_operation){
                push_stack(&undo_stack, false);

                // frees memory and clears redo stack when new move played
                for(int i=0; i<redo_stack.top; i++){
                    free(redo_stack.moves[i]);
                }
                redo_stack.top = -1;
            }
            player_grid[insert_row][insert_col] = insert_value;

            if(grid_empties == 0){
                check_grid_solved();
            }
        }
    }

    if(!quit_current_game){
        // completed board
        system("cls");
        printf("**********************  SUDOKU  **********************\n\n");

        display_board(player_grid);
        printf("\n\nCongratulations!!!\n");
        printf("Enter any key to return to the main menu\n");

        fflush(stdin);
        getchar();
    }
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

        printf("\nKey: U=Undo R=Redo S=Settings\n");


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
    undo_operation = false;

    // checks column input
    if(isalpha(input[0])){

        // if input is for undo/redo/settings
        switch(toupper(input[0])){
            case 'U':
                undo_operation = true;
                invalid = pop_stack(&undo_stack);
                return invalid;

            case 'R':
                undo_operation = false;
                invalid = pop_stack(&redo_stack);
                return invalid;

            case 'S':
                ingame_settings();
                return invalid;
        }

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


// Undoes or Redoes a move
int pop_stack(moves_stack* m_stack) {

    bool invalid = false;

    // if stack is empty
    if(m_stack->top == -1){
        return invalid = true;
    }

    insert_row = m_stack->moves[m_stack->top][0];
    insert_col = m_stack->moves[m_stack->top][1];

    // sets values so pushed to redo stack correctly
    insert_value = m_stack->moves[m_stack->top][2];
    player_grid[insert_row][insert_col] = m_stack->moves[m_stack->top][3];

    // push_stack function checks and allocates if required more memory for stack
    if(undo_operation){
        push_stack(&redo_stack, false);
        // previous value
        insert_value = m_stack->moves[m_stack->top][3];

        // inserts value prior to undo (for checking empties in play_game function)
        player_grid[insert_row][insert_col] = m_stack->moves[m_stack->top][2];
    }else{
        push_stack(&undo_stack, false);
    }

    // avoids adding back to stack once back in play_game function
    undo_operation = true;

    free(m_stack->moves[m_stack->top]);

    m_stack->top--;

    return invalid;
}



// initialises undo and redo stacks
void init_stack(int stack_size, moves_stack* stack) {
    stack->top = -1;
    stack->allocated = stack_size;

    stack->moves = malloc(stack_size * sizeof(int*));
    if (stack->moves == NULL)
    {
        printf("Memory allocation failure\n");
        exit(1);
    }
}


// pushes last move details to undo stack
void push_stack(moves_stack* m_stack, bool loading) {

    int* move = malloc(4*sizeof(int));
    if(move == NULL){
        printf("Memory allocation failure\n");
        exit(1);
    }

    if(!loading){
        previous_value = player_grid[insert_row][insert_col];
    }

    move[0] = insert_row;
    move[1] = insert_col;
    move[2] = insert_value;
    move[3] = previous_value;

    m_stack->top++;

    if(m_stack->top >= m_stack->allocated){
        extend_stack(m_stack, SIZE);
    }

    m_stack->moves[m_stack->top] = move;
}


// allocates more memory for stack
void extend_stack(moves_stack* m_stack, int extend_by) {
    int pre_extend_allocated = m_stack->allocated;

    m_stack->allocated = m_stack->allocated + extend_by;

    m_stack->moves = realloc(m_stack->moves, ((pre_extend_allocated * sizeof(int*)) + (extend_by * sizeof(int*))));
    if (m_stack->moves == NULL)
    {
        printf("Memory allocation failure\n");
        exit(1);
    }
}


// frees undo and redo stacks
void deallocate_stacks() {

    for(int i=0; i<undo_stack.top; i++){
        free(undo_stack.moves[i]);
    }

    for(int i=0; i<redo_stack.top; i++){
        free(redo_stack.moves[i]);
    }

    free(undo_stack.moves);
    free(redo_stack.moves);
}


void save(){
    bool invalid = false;
    char choice[3];

    do{
        system("cls");
        printf("**********************  SUDOKU  **********************\n");
        printf("                      SAVE MENU\n\n");

        list_files(true, &invalid, &choice[0]);

    }while(invalid);

    if(choice[0] != '\n'){
        write_to_file(atoi(choice));
    }
}


void load_game() {
    bool invalid = false;
    char choice[3];

    do{
        system("cls");
        printf("**********************  SUDOKU  **********************\n");
        printf("                       LOAD MENU\n\n");

        list_files(false, &invalid, &choice[0]);

    }while(invalid);

    if(choice[0] != '\n'){
        // sets variables
        read_from_file(atoi(choice));

        // sets random number seed as used in display_board function
        set_seed();
        set_box_row_col();
        quit_current_game = false;
        solved = false;

        play_game();

        deallocate_grid(&solution_grid);
        deallocate_grid(&player_grid);
        deallocate_grid(&original_player_grid);
        deallocate_stacks();
    }
}


void write_to_file(int selection){
    FILE* file_pt;
    char filename[80];
    DIR* directory;
    struct dirent* dir;

    directory = opendir("save-files");


    if(selection == 1){
        time_t rawtime;
        struct tm *timedate;

        time(&rawtime);

        timedate = localtime(&rawtime);

        strftime(filename,80,"%Y_%b_%d_%H.%M.%S.txt", timedate);

    } else {
        int file_number = 2;

        while((dir = readdir(directory)) != NULL) {
            if((strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0)){
                if(file_number == selection){
                    strcpy(filename, dir->d_name);
                    break;
                }
                file_number++;
            }
        }
    }

    char path[80] = "./save-files/";
    strcat(path, filename);

    file_pt = fopen(path, "w");
    fflush(stdin);
    rewind(file_pt);

    fprintf(file_pt, "%d\n", SIZE);
    fprintf(file_pt, "%c\n", *difficulty);
    fprintf(file_pt, "%d\n", colour_aid_setting);
    fprintf(file_pt, "%d\n", grid_empties);

    // undo stack
    fprintf(file_pt, "%d\n", undo_stack.allocated);
    fprintf(file_pt, "%d\n", undo_stack.top);
    for(int i=0; i<=undo_stack.top; i++){
        fprintf(file_pt, "%d ", undo_stack.moves[i][0]); // row index
        fprintf(file_pt, "%d ", undo_stack.moves[i][1]); // col index
        fprintf(file_pt, "%d ", undo_stack.moves[i][2]); // insert value
        fprintf(file_pt, "%d ", undo_stack.moves[i][3]); // previous value
    }
    fprintf(file_pt, "\n");

    // redo stack
    fprintf(file_pt, "%d\n", redo_stack.allocated);
    fprintf(file_pt, "%d\n", redo_stack.top);
    for(int i=0; i<=redo_stack.top; i++){
        fprintf(file_pt, "%d ", redo_stack.moves[i][0]); // row index
        fprintf(file_pt, "%d ", redo_stack.moves[i][1]); // col index
        fprintf(file_pt, "%d ", redo_stack.moves[i][2]); // insert value
        fprintf(file_pt, "%d ", redo_stack.moves[i][3]); // previous value
    }
    fprintf(file_pt, "\n");

    // solution grid
    for(int row=0; row<SIZE; row++){
        for(int col=0; col<SIZE; col++){
            fprintf(file_pt, "%d ", solution_grid[row][col]);
        }
    }
    fprintf(file_pt, "\n");

    // player grid
    for(int row=0; row<SIZE; row++){
        for(int col=0; col<SIZE; col++){
            fprintf(file_pt, "%d ", player_grid[row][col]);
        }
    }
    fprintf(file_pt, "\n");

    // original player grid
    for(int row=0; row<SIZE; row++){
        for(int col=0; col<SIZE; col++){
            fprintf(file_pt, "%d ", original_player_grid[row][col]);
        }
    }

    // if adding more to file, add new line character

    fclose(file_pt);
    closedir(directory);
}


void read_from_file(int selection) {
    FILE* file_pt;
    char filename[80];
    DIR* directory;
    struct dirent* dir;
    char buff[255];
    int read_int;


    directory = opendir("save-files");

    int file_number = 1;

    while((dir = readdir(directory)) != NULL) {
        if((strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0)){
            if(file_number == selection){
                strcpy(filename, dir->d_name);
                break;
            }
            file_number++;
        }
    }

    char path[80] = "./save-files/";
    strcat(path, filename);

    file_pt = fopen(path, "r");
    fflush(stdin);
    rewind(file_pt);


    // SIZE
    fscanf(file_pt, "%d", &SIZE);
    // moves file position to next line
    fgets(buff, 255, file_pt);

    // difficulty
    fgets(buff, 2, file_pt);
    if(strcmp(&buff[0],"E") == 0) {
        difficulty = &easy[0];
    } else if(strcmp(&buff[0],"M") == 0) {
        difficulty = &medium[0];
    }else {
        difficulty = &hard[0];
    }

    // colour_aid_setting
    fscanf(file_pt, "%d", &colour_aid_setting);
    // moves file position to next line
    fgets(buff, 255, file_pt);

    // grid_empties
    fscanf(file_pt, "%d", &grid_empties);
    // moves file position to next line
    fgets(buff, 255, file_pt);

    // undo_stack
    // allocated
    fscanf(file_pt, "%d", &read_int);
    // moves file position to next line
    fgets(buff, 255, file_pt);
    init_stack(read_int, &undo_stack);
    // undo.top
    fscanf(file_pt, "%d", &read_int);
    // moves file position to next line
    fgets(buff, 255, file_pt);
    for(int i=0; i<=read_int; i++){
        fscanf(file_pt, "%d", &insert_row);
        fscanf(file_pt, "%d", &insert_col);
        fscanf(file_pt, "%d", &insert_value);
        fscanf(file_pt, "%d", &previous_value);
        push_stack(&undo_stack, true);
    }
    // moves file position to next line
    fgets(buff, 255, file_pt);

    // redo_stack
    // allocated
    fscanf(file_pt, "%d", &read_int);
    // moves file position to next line
    fgets(buff, 255, file_pt);
    init_stack(read_int, &redo_stack);
    // redo.top
    fscanf(file_pt, "%d", &read_int);
    // moves file position to next line
    fgets(buff, 255, file_pt);
    for(int i=0; i<=read_int; i++){
        fscanf(file_pt, "%d", &insert_row);
        fscanf(file_pt, "%d", &insert_col);
        fscanf(file_pt, "%d", &insert_value);
        fscanf(file_pt, "%d", &previous_value);
        push_stack(&redo_stack, true);
    }
    // moves file position to next line
    fgets(buff, 255, file_pt);

    // solution_grid
    allocate_grid(&solution_grid);
    for(int row=0; row<SIZE; row++){
        for(int col=0; col<SIZE; col++){
            fscanf(file_pt, "%d", &read_int);
            solution_grid[row][col] = read_int;
        }
    }
    // moves file position to next line
    fgets(buff, 255, file_pt);

    // player_grid
    allocate_grid(&player_grid);
    for(int row=0; row<SIZE; row++){
        for(int col=0; col<SIZE; col++){
            fscanf(file_pt, "%d", &read_int);
            player_grid[row][col] = read_int;
        }
    }
    // moves file position to next line
    fgets(buff, 255, file_pt);

    // original_player_grid
    allocate_grid(&original_player_grid);
    for(int row=0; row<SIZE; row++){
        for(int col=0; col<SIZE; col++){
            fscanf(file_pt, "%d", &read_int);
            original_player_grid[row][col] = read_int;
        }
    }

    fclose(file_pt);
    closedir(directory);
}


/* lists save/load files available and returns users choice
also sets invalid boolean passed as required */
void list_files(bool saving, bool* invalid, char* selection) {
    char* choice = selection;
    choice[0] = '0';
    DIR* directory;
    struct dirent* dir;

    directory = opendir("save-files");

    // attempts to create directory if does not exist
    if(!directory){
        int dir_check_valid = mkdir("save-files");

        if(dir_check_valid != 0){
            printf("Cannot create directory, unable to save\n");
            printf("Press enter to continue\n");
            fflush(stdin);
            getchar();
            *invalid = false;
            return;
        }

        directory = opendir("save-files");
    }

    int file_number = 1;

    printf("Available files:\n\n");

    // option for when saving for new file
    if(saving){
        printf("%d. New save file\n\n", file_number);
        file_number++;
    }

    // prints current files (not including parent and current directory)
    while((dir = readdir(directory)) != NULL) {
        if((strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0)){
            printf("%d. %s\n", file_number, dir->d_name);
            file_number++;
        }
    }

    // when loading, if no files
    if(!saving && file_number == 1){
        printf("No files to load\n\n");
        printf("Press enter to continue\n");
        closedir(directory);
        fflush(stdin);
        getchar();
        choice[0] = '\n';
        *invalid = false;
        return;
    }

    if(*invalid){
        printf("\nChoice must be a whole number up to %d\n", file_number-1);
    }else{
        printf("\n\n");
    }

    printf("Please enter a number for your choice or enter to go back:\n");
    fflush(stdin);
    fgets(&choice[0], sizeof(choice), stdin);


    if(choice[0] == '\n'){
        *invalid = false;
    }else if(!isdigit(choice[0]) || atoi(choice) < 1 || atoi(choice) >= file_number){
        *invalid = true;
        choice = "";
    } else {
        *invalid = false;
    }

    closedir(directory);
    return;
}
