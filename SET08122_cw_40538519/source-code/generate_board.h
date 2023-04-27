#ifndef GENERATE_BOARD_H
#define GENERATE_BOARD_H

// doubly linked list of all possible numbers to be entered in a box/row/column
typedef struct node
{
    int data;
    struct node *next;
    struct node *prev;
} node;

// maintains list and attributes for each box/row/column - doubly linked list
typedef struct candidates
{
    struct node *cand_list; // points to list of nodes containing all candidate numbers
    int id;
    int remaining; // number of candidates left in list
    struct candidates *next;
    struct candidates *prev;
} candidates;

// stores list of cell references
// when used by empty_cells, it is used like a stack
// when used by populated_cells, it is randomly accessed to select a cell and used similar to an array
typedef struct cell_ref
{
    // pointers due to size of arrays is dependant on size which is game board size dependant
    int *row;
    int *col;
    int top;
} cell_ref;

// stores the grid references for each populated cell
// circular doubly linked list allowing pointer into the list to be shuffled
typedef struct populated_node
{
    int row;
    int col;
    struct populated_node *next;
    struct populated_node *prev;
} populated_node;

// stores the first access node to the populated list and number of nodes remaining
typedef struct populated
{
    struct populated_node *populated_list;
    int remaining;
} populated;

void init_grid(int**);                                                                                      // sets all cells to 0
void partially_complete(int**, candidates*, candidates*, candidates*);                                      // randomly fills first box
node *init_cand_num_list();                                                                                 // creates doubly linked list of all possible numbers for a box/row/column
void clean_up(candidates*, candidates*, candidates*, cell_ref*, populated*);                                // deallocates memory
candidates *init_candidates(bool);                                                                          // creates structure (doubly linked list) to contain all candidates for either rows/columns/boxes
void set_order_to_compare(candidates**, candidates**, candidates**, candidates*, candidates*, candidates*); // sets node pointers for when looking for a match to reduce comparisons
int get_match(node**, candidates*, candidates*, int*);                                                      // returns a matching candidate that exists in all lists (row/col/box)
node *remove_candidate(candidates**, int, int);                                                             // removes number from candidates list, node is returned for restoration and freeing
void allocate_cell_ref(cell_ref*);                                                                          // allocates memory for cell references structure
void init_cell_ref(cell_ref*, int**);                                                                       // populates struct with grid references;
void fill_first_empty_cell(cell_ref*, int**, candidates*, candidates*, candidates*);                        // tries all matches in first empty cell (a solution will exist)
void restore_candidate(candidates**, node*, int);                                                           // restores node to list of candidates
void insert_candidate(candidates**, int, int);                                                              // used to create a node and insert into candidates when removing numbers from grid
int solve(cell_ref*, int**, candidates*, candidates*, candidates*, bool, int);                              // attempts to solve, if no matches, backtracks
int get_box_num(int, int);                                                                                  // calculate box number (0 indexed)
void copy_grid(int**, int**);                                                                               // copies one grid to another
void rem_grid_nums(int**, populated*);                                                                 // removes 'remove' amount of numbers from grid (based on difficulty)
populated *init_populated_cell_ref();                                                                       // generates populated cells list;
void deallocate_candidates(candidates*, candidates*, candidates*);                                          // frees memory for candidates

// functions used elsewhere
void allocate_grid(int***);      // allocates memory for grid
void display_board(int**);       // iterates through grid to print game board
void deallocate_grid(int***);    // frees memory for grid
void sepearator_row();           // prints a row of --- dependant on board SIZE
void set_box_row_col();          // calculates number of rows/columns per box and sets variables so not having to calculate multiple times
void generate_grids(); // generates a solution and player grid
void set_seed(); // function also called when loading game (as used in display_board function)

#endif // GENERATE_BOARD_H
