#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <ctype.h>
#include <conio.h>


typedef struct moves_stack {
    int top;
    int allocated;
    int** moves;
} moves_stack;

extern char hard[];
extern char medium[];
extern char easy[];
extern char* difficulty;
extern int insert_row;
extern int insert_col;
extern int insert_value;
extern int previous_value;
extern bool quit_current_game;
extern moves_stack undo_stack;
extern moves_stack redo_stack;
extern bool undo_operation;

void menu(); // prints main menu loops menu until enter is pressed with no input will return to main menu after a game has been finished
void settings(); // prints settings menu
void ingame_settings(); // ingame settings menu
void change_board_size(); // prints menu for changing board size
void change_difficulty(); // prints menu for changing difficulty
int parse_difficulty_input(int, int); // determines if integer entered is valid and if so changes variable
int parse_size_input(int, int); // determines if integer entered is valid and if so changes variable
int parse_settings_input(int, int, bool); // determines if integer entered is valid and if so changes variable
int parse_main_input(int, int); // gets user input and returns invalid boolean, true if first character entered is not a number
int get_input(char*); // returns invalid int flag (true if input is either not an int or if enter pressed to quit/go back)
void set_num_to_remove(); // sets num_to_remove based on difficulty set before generating a new game
void create_new_game();  // sets variables prior to generating board
void play_game(); // displays gameboard until solved
void check_grid_solved();
void get_grid_ref(); // asks user for grid reference and value to insert
int check_insert_input(char*); // checks if users input for inserting to the grid is valid
int pop_stack(moves_stack*); // Undoes or Redoes a move
void init_stack(int, moves_stack*); // initialises undo and redo stacks
void push_stack(moves_stack*, bool); // pushes last move details to undo stack
void extend_stack(moves_stack*, int);
void deallocate_stacks();
void save();
void load_game();
void write_to_file(int);
void read_from_file(int);
void list_files(bool, bool*, char*); // lists save/load files available and gets users choice

#endif // USER_INTERFACE_H
