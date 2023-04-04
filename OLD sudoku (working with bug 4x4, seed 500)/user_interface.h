#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <ctype.h>
#include <conio.h>

extern char hard[];
extern char medium[];
extern char easy[];
extern char* difficulty;
extern int insert_row;
extern int insert_col;
extern int insert_value;

void menu(); // prints main menu loops menu until enter is pressed with no input will return to main menu after a game has been finished
void settings(); // prints settings menu
void change_board_size(); // prints menu for changing board size
void change_difficulty(); // prints menu for changing difficulty
int parse_difficulty_input(int, int); // determines if integer entered is valid and if so changes variable
int parse_size_input(int, int); // determines if integer entered is valid and if so changes variable
int parse_settings_input(int, int); // determines if integer entered is valid and if so changes variable
int parse_main_input(int, int); // gets user input and returns invalid boolean, true if first character entered is not a number
int get_input(char*); // returns invalid int flag (true if input is either not an int or if enter pressed to quit/go back)
void create_new_game();  // sets variables prior to generating board
void set_num_to_remove(); // sets num_to_remove based on difficulty set before generating a new game
void play_game(); // displays gameboard until solved
void get_grid_ref(); // asks user for grid reference and value to insert
int check_insert_input(char*); // checks if users input for inserting to the grid is valid
void check_grid_solved();

#endif // USER_INTERFACE_H
