#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

// doubly linked list of all possible numbers to be entered in a box/row/column
typedef struct node {
    int data;
    struct node* next;
    struct node* prev;
} node;

// maintains list and attributes for each box/row/column - doubly linked list
typedef struct candidates {
    struct node* cand_list; // points to list of nodes containing all candidate numbers
    int id;
    int remaining; // number of candidates left in list
    struct candidates* next;
    struct candidates* prev;
} candidates;

// stores list of cell references
// when used by empty_cells, it is used like a stack
// when used by populated_cells, it is randomly accessed to select a cell and used similar to an array
typedef struct cell_ref {
    // pointers due to size of arrays is dependant on size which is game board size dependant
    int* row;
    int* col;
    int top;
} cell_ref;

// stores the grid references for each populated cell
// circular doubly linked list allowing pointer into the list to be shuffled
typedef struct populated_node {
    int row;
    int col;
    struct populated_node* next;
    struct populated_node* prev;
} populated_node;

// stores the first access node to the populated list and number of nodes remaining
typedef struct populated {
    struct populated_node* populated_list;
    int remaining;
} populated;

// stored as global variables to avoid recalculating - will be constant for each game board size
int SIZE;
int BOX_ROWS; // rows per box - 9x9 board will be 3, 6x6 will be 2
int BOX_COLS; // cols per box - 9x9 board will be 3, 6x6 will be 3

void display_board(int** grid); // iterates through grid to print game board
void init(int** grid); // initialise grid
void sepearator_row(); // prints a row of --- dependant on board SIZE
void partially_complete(int** grid, candidates* row_cand, candidates* col_cand, candidates* box_cand); // randomly fills boxes which do not conflict with each other
node* create_box_num_list(); // creates doubly linked list of all numbers for a box/row/column
void clean_up(int*** sol_grid, int*** player_grid, candidates* row_cand, candidates* col_cand, candidates* box_cand, cell_ref* emp, populated* pop); // deallocates memory
candidates* generate_candidates(bool populated); // creates structure (doubly linked list) to contain all candidates for either rows/columns/boxes
void set_order_to_compare(candidates** least_cand, candidates** mid_cand, candidates** lon_cand, candidates* row, candidates* col, candidates* box); // sets node pointers for when looking for a match to reduce comparisons
int find_match(node** least_list, candidates* mid_cand, candidates* most_cand, int* least_rem); // returns a matching candidate that exists in all lists (row/col/box)
node* remove_candidate(candidates** can, int del_num, int id_num); // removes number from candidates list, node is returned for restoration and freeing
void allocate_cell_ref(cell_ref* cells); // allocates memory for cell references structure
void init_cell_ref(cell_ref* cells, int** grid); // populates struct with grid references;
void fill_first_empty_cell(cell_ref* empties, int** grid, candidates* row_cand, candidates* col_cand, candidates* box_cand); // tries all matches in first empty cell (a solution will exist)
void restore_candidate(candidates** can, node* res_num, int id_num); // restores node to list of candidates
void insert_candidate(candidates** can, int insert, int id_num); // used to create a node and insert into candidates when removing numbers from grid
int solve(cell_ref* empties, int** grid, candidates* row_cand, candidates* col_cand, candidates* box_cand); // attempts to solve, if no matches, backtracks
int get_box_num(int row, int col); // calculate box number (0 indexed)
void allocate_grid(int*** grid); // allocates memory for grid
void copy_grid(int** from, int** to); // copies one grid to another
void rem_grid_nums(int** grid, int remove, populated* remain_ref); // removes 'remove' amount of numbers from grid (based on difficulty)
populated* create_populated(); // generates populated cells list;
void deallocate_candidates(candidates* row_cand, candidates* col_cand, candidates* box_cand); // frees memory for candidates


// program starts here
int main() {

    // grid is SIZE * SIZE
    SIZE = 9;

    // number of rows/cols per box, will be constant for each game board
    BOX_ROWS = floor(sqrt(SIZE)); // rows per box - 9x9 board will be 3, 6x6 will be 2
    BOX_COLS = ceil(sqrt(SIZE)); // cols per box - 9x9 board will be 3, 6x6 will be 3


    // sets seed for random number generator
//    srand(time(0));
srand(1);//////////////////////////////////////////////////////////////////////////

    // declare grid
    int** solution_grid;
    int** player_grid;

    // allocates memory for grids
    allocate_grid(&solution_grid);
    allocate_grid(&player_grid);

    // initialise grid, setting each cell to 0
    init(solution_grid);

    // generate candidates
    // each points to the first candidates list in row/col/box
    candidates* row_cand = generate_candidates(true);
    candidates* col_cand = generate_candidates(true);
    candidates* box_cand = generate_candidates(true);

    // randomly fills boxes which do not conflict with each other
    partially_complete(solution_grid, row_cand, col_cand, box_cand);

    // creates stack of empty cell grid references;
    cell_ref empty_cells;
    allocate_cell_ref(&empty_cells);
    init_cell_ref(&empty_cells, solution_grid);

    // completes grid - calls solve recursively within function
    fill_first_empty_cell(&empty_cells, solution_grid, row_cand, col_cand, box_cand);

    // copies completed grid to players grid before removing numbers
    copy_grid(solution_grid, player_grid);

    // creates doubly linked circular list of cell references to all populated cells
    populated* populated_list = create_populated();




    // removes number passed amount of numbers from grid
    rem_grid_nums(player_grid, 10, populated_list);



    // print board
//   display_board(solution_grid);
//    printf("\n");
//   display_board(player_grid);

    // deallocates memory
    clean_up(&solution_grid, &player_grid, row_cand, col_cand, box_cand, &empty_cells, populated_list);

    return 0;
}


// deallocates memory
void clean_up(int*** sol_grid, int*** player_grid, candidates* row_cand, candidates* col_cand, candidates* box_cand, cell_ref* emp, populated* pop) {

    // frees grids
    free(*sol_grid[0]);
    *sol_grid[0] = NULL;
    free(*sol_grid);
    *sol_grid = NULL;

    free(*player_grid[0]);
    *player_grid[0] = NULL;
    free(*player_grid);
    *player_grid = NULL;


    // frees candidates (nodes are freed in solve function)
    deallocate_candidates(row_cand, col_cand, box_cand);

    // frees cell_refs
    free(emp->row);
    emp->row = NULL;
    free(emp->col);
    emp->col = NULL;

    // frees populated list nodes and structure
    int del = pop->remaining;
    populated_node* node= pop->populated_list->next;
    for(int i=0; i<del-1; i++){
        populated_node* temp = node;
        node = temp->next;
        free(temp);
        temp = NULL;
    }
    node = NULL;
    free(pop->populated_list);
    pop->populated_list=NULL;

    free(pop);
    pop = NULL;
}


// frees memory for candidates
void deallocate_candidates(candidates* row_cand, candidates* col_cand, candidates* box_cand) {
    candidates* temp;

    while(row_cand -> next != NULL) {
        temp = row_cand;
        row_cand = row_cand -> next;
        free(temp);
        temp = NULL;
    }
    free(row_cand);
    row_cand = NULL;

    while(col_cand -> next != NULL) {
        temp = col_cand;
        col_cand = col_cand -> next;
        free(temp);
        temp = NULL;
    }
    free(col_cand);
    col_cand = NULL;

    while(box_cand -> next != NULL) {
        temp = box_cand;
        box_cand = box_cand -> next;
        free(temp);
        temp = NULL;
    }
    free(box_cand);
    box_cand = NULL;
}


/*/////////// number to remove for hard
size = 4 -> 10 (6 clues, min is 4)
size = 6 -> 26 (10 clues, min is 8)
size = 9 -> 61 (20 clues, min is 17)
size = 12 -> 94 (50 clues, unknown min, found other hards with 48)
size = 16 ->.....research and test

medium = hard - SIZE
easy = hard - ceil(SIZE*1.5)
*/


// removes numbers from player_grid
void rem_grid_nums(int** grid, int remove, populated* remain_ref) {
    int removed = 0;
    int all_clue_nums_remain = false;
    populated_node* remove_from;
    int* frequency;

    // keeps track of how many of each number remain in grid to ensure only 1 can be reduced to 0 clues
    frequency = (int*) malloc(SIZE*sizeof(int));
    for(int i=0; i<SIZE; i++) {
        frequency[i] = SIZE;
    }

    // generate empty candidate lists
    candidates* row_cand = generate_candidates(true);
    candidates* col_cand = generate_candidates(true);
    candidates* box_cand = generate_candidates(true);

    // allocates memory for empty cell refs and sets top to -1
    cell_ref empties;
    allocate_cell_ref(&empties);

removed=11;
    while(removed < remove) {
        // sets cell to remove from to point at first node in list
        remove_from = remain_ref->populated_list;

        // generate random number between 0 and number of populated cells left to be pick from -1
        int r_num = rand() % (remain_ref->remaining);

        // randomly sets remove_from cell reference node pointer
        for(int i=0; i<r_num; i++) {
            remove_from = remove_from->next;
        }

        // updates access node if pointing to node to be removed
        if(remove_from == remain_ref->populated_list) {
            remain_ref->populated_list = remove_from->next;
        }

        // removes from populated_list and updates remaining
        remove_from->next->prev = remove_from->prev;
        remove_from->prev->next = remove_from->next;
        remain_ref->remaining--;

        // gets value from grid
        int grid_value = grid[remove_from->row][remove_from->col];

        // if a value has been totally removed from the grid and this is the last of another, continue and do not remove
        if((all_clue_nums_remain == 0) && (frequency[grid_value-1] == 1)) {
            continue;
        }

        // removes value from grid and adds to candidate lists & empties
        grid[remove_from->row][remove_from->col] = 0;

        empties.top++;
        empties.row[empties.top] = remove_from->row;
        empties.col[empties.top] = remove_from->col;

        insert_candidate(&row_cand, grid_value, remove_from->row);
        insert_candidate(&col_cand, grid_value, remove_from->col);
        insert_candidate(&box_cand, grid_value, get_box_num(remove_from->row, remove_from->col));




// sets candidates to correct cell??????????????????????????????????

        removed++;

        free(remove_from);
        remove_from = NULL;
    }
//////////// free empties ----- possibly done further up in match loop
    deallocate_candidates(row_cand, col_cand, box_cand);
    row_cand, col_cand, box_cand = NULL;
    free(frequency);
    frequency = NULL;
}


// generate populated cells list
populated* create_populated() {
    populated* p;
    populated_node* last, * current;//, * access;
    int row = 0;
    int col = 0;

    // allocates memory for populated structure (seg fault without this)
    p = (populated*) malloc(sizeof(populated));
    if(p==NULL) {
        printf("Memory allocation failure\n");
        exit(1);
    }

    // allocates memory for first node
    // outside loop as first points to NULL both ways
    current = (populated_node*) malloc(sizeof(populated_node));
    if(current == NULL) {
            printf("Memory allocation failure\n");
            exit(1);
        }

    current->next = NULL;
    current->prev = NULL;
    current->row = row;
    current->col = col;

    // sets access nodes next to point at current
    p->populated_list = current;
    p->remaining = 1;

    last = current;
    col++;

    // creates nodes for doubly linked list
    for(row; row<SIZE; row++) {
        for(col; col<SIZE; col++) {
            current = (populated_node*) malloc(sizeof(populated_node));
            if(current == NULL) {
                    printf("Memory allocation failure\n");
                    exit(1);
                }

            current->next = NULL;
            current->prev = last;
            current->row = row;
            current->col = col;

            last->next = current;
            last = current;

            p->remaining++;
        }
        col = 0;
    }

    // makes list circular
    current->next = p->populated_list;
    p->populated_list->prev = current;

    return p;
}


// copies grid
void copy_grid(int** from, int** to) {

    for(int row=0; row<SIZE; row++) {
        for(int col=0; col<SIZE; col++) {
            to[row][col] = from[row][col];
        }
    }
}


// allocates memory for grid
void allocate_grid(int*** grid) {

    // memory allocation code and 2d array set-up adapted from
    // (No name or date, Multidimensional arrays in C, https://www.uio.no/studier/emner/matnat/ifi/IN3200/v19/teaching-material/multidimarrays.pdf)
     *grid = malloc(SIZE * sizeof **grid);
     if(*grid == NULL) {
        printf("Memory allocation failure\n");
        exit(1);
     }

     *grid[0] = malloc(SIZE*SIZE * sizeof **grid[0]);
     if(*grid[0] == NULL) {
        printf("Memory allocation failure\n");
        exit(1);
     }

     for(int i=1; i<SIZE; i++) {
        (*grid)[i] = &((*grid)[0][i*SIZE]);
     }
}


// attempts to solve grid, backtracks if no matches found
int solve(cell_ref* empties, int** grid, candidates* row_cand, candidates* col_cand, candidates* box_cand) {
    // if no more empties, board is solved
    if(empties->top == -1) {
        return true;
    }


    // gets grid reference for next empty cell and updates top
    int row = empties->row[empties->top];
    int col = empties->col[empties->top];
    empties->top--;

    candidates* curr_row_can = row_cand;
    candidates* curr_col_can = col_cand;
    candidates* curr_box_can = box_cand;

    // removed candidates to either restore or free
    node* temp_row, * temp_col, * temp_box;

    // sets candidates to correct cell
    while(curr_row_can->id != row) {
        curr_row_can = curr_row_can->next;
    }
    while(curr_col_can->id != col) {
        curr_col_can = curr_col_can->next;
    }
    while(curr_box_can->id != get_box_num(row, col)) {
        curr_box_can = curr_box_can->next;
    }

    // ordered candidates, allows access to remaining for each list (without knowing which is row/col/box)
    candidates* least, * mid, * most;
    // smallest list, allows to be progressed without affecting originals to find ALL matches
    node* least_list;

    // sets order
    set_order_to_compare(&least, &mid, &most, curr_row_can, curr_col_can, curr_box_can);

    // sets list to be able to move forward for all matches
    least_list = least->cand_list;
    int least_counter = least->remaining;
    int match;

    // looks for match against every element from shortest list
    while(least_counter>0) {
        match = find_match(&least_list, mid, most, &least_counter);
        // when theres a match
        if(match != -1) {
            // insert into grid
            grid[row][col] = match;

            // remove candidates
            temp_row = remove_candidate(&curr_row_can, match, row);
            temp_col = remove_candidate(&curr_col_can, match, col);
            temp_box = remove_candidate(&curr_box_can, match, curr_box_can->id);

            // if fully solved, done
            // else restore candidates and try next match
            if(solve(empties, grid, row_cand, col_cand, box_cand) == 1) {
                free(temp_row);
                temp_row = NULL;
                free(temp_col);
                temp_col = NULL;
                free(temp_box);
                temp_box = NULL;
                return 1;
            } else {
                restore_candidate(&curr_row_can, temp_row, row);
                restore_candidate(&curr_col_can, temp_col, col);
                restore_candidate(&curr_box_can, temp_box, curr_box_can->id);
            }
        }
    }
    // restores empty cell to list of empties when backtracking (does not restore grid to 0, as no need. Will get overwritten next time it tries this cell)
    empties->top++;
    empties->row[empties->top] = row;
    empties->col[empties->top] = col;
    return false;
}


// tries all matches in first empty cell (a solution will exist)
void fill_first_empty_cell(cell_ref* empties, int** grid, candidates* row_cand, candidates* col_cand, candidates* box_cand) {
    // gets grid reference for first empty cell and updates top
    int row = empties->row[empties->top];
    int col = empties->col[empties->top];
    empties->top--;

    candidates* curr_row_can = row_cand;
    candidates* curr_col_can = col_cand;
    candidates* curr_box_can = box_cand;

    // removed candidates to either restore or free
    node* temp_row, * temp_col, * temp_box;

    // sets candidates to correct cell
    while(curr_row_can->id != row) {
        curr_row_can = curr_row_can->next;
    }
    while(curr_col_can->id != col) {
        curr_col_can = curr_col_can->next;
    }
    while(curr_box_can->id != get_box_num(row, col)) {
        curr_box_can = curr_box_can->next;
    }

    // ordered candidates, allows access to remaining for each list (without knowing which is row/col/box)
    candidates* least, * mid, * most;
    // smallest list, allows to be progressed without affecting originals to find ALL matches
    node* least_list;

    // sets order
    set_order_to_compare(&least, &mid, &most, curr_row_can, curr_col_can, curr_box_can);

    // sets list to be able to move forward for all matches
    least_list = least->cand_list;
    int least_counter = least->remaining;
    int match;

    // looks for match against every element from shortest list, one of these will work (as first cell which can cause other conflicts)
    while(least_counter>0) {
        match = find_match(&least_list, mid, most, &least_counter);
        // when theres a match
        if(match != -1) {
            // insert into grid
            grid[row][col] = match;

            // remove candidates
            temp_row = remove_candidate(&curr_row_can, match, row);
            temp_col = remove_candidate(&curr_col_can, match, col);
            temp_box = remove_candidate(&curr_box_can, match, curr_box_can->id);

            // if fully solved, done
            // else restore candidates and try next match
            if(solve(empties, grid, row_cand, col_cand, box_cand) == 1) {
                free(temp_row);
                temp_row = NULL;
                free(temp_col);
                temp_col = NULL;
                free(temp_box);
                temp_box = NULL;
                return;
            } else {
                restore_candidate(&curr_row_can, temp_row, row);
                restore_candidate(&curr_col_can, temp_col, col);
                restore_candidate(&curr_box_can, temp_box, curr_box_can->id);
            }
        }
    }
    printf("something went wrong, no match found for first empty cell\n");
}


// sets grid references for empty cells
void init_cell_ref(cell_ref* cells, int** grid) {
    // iterates rows from bottom right corner
    // (so top of stack will be pointing at first empty cell in top row)
    for(int row=SIZE-1; row>=0; row--) {
        // iterates columns starting at last column
        for(int col=SIZE-1; col>=0; col--) {
            if(grid[row][col] == 0) {
                cells->top = cells->top+1;
                cells->row[cells->top] = row;
                cells->col[cells->top] = col;
            }
        }
    }
}


// allocates memory for cell references structure for empty cells
void allocate_cell_ref(cell_ref* cells) {

    cells->top = -1;
    int MAX = SIZE*SIZE;

    cells->row = (int*)malloc(MAX * sizeof(int));
    if(cells->row == NULL) {
        printf("Memory allocation failure\n");
        exit(1);
    }

    cells->col = (int*)malloc(MAX * sizeof(int));
    if(cells->col == NULL) {
        printf("Memory allocation failure\n");
        exit(1);
    }
}


/* returns a matching candidate that exists in all lists (row/col/box)
   expects candidates to be passed in order of least remaining candidates (to reduce comparisons)
   least_list (with fewest remaining) is passed in as a list (pointer to a list pointer) rather than a candidate,
   so pointer can be advanced when finding ALL matches without affecting original candidate pointer
   remaining are passed in as candidates */
int find_match(node** least_list, candidates* mid_cand, candidates* most_cand, int* least_counter) {
    node* temp_mid, * temp_most;
    int num;

    // if none remain in any list, then cannot be a match
    if(*least_counter == 0 || mid_cand->remaining == 0 || most_cand->remaining == 0) {
        *least_list = NULL;
        *least_counter = 0;
        return -1;
    }

    // iterates smallest list (this will never need resetting)
    for(int i=0; i < *least_counter; i++) {
        // gets first number from smallest list
        num = (*least_list)->data;

        // updates remaining to check for smallest list
        *least_counter = *least_counter-1;

        // resets temp nodes which have pointers moved forward in loop
        temp_mid = mid_cand->cand_list;
        temp_most = most_cand->cand_list;

        // iterates medium length list
        for(int j=0; j < mid_cand->remaining; j++) {

            // if number matches mid length list, check longest list
            if(num == temp_mid->data) {

                for(int k=0; k < most_cand->remaining; k++){
                    // if match found, return
                    if(num == temp_most->data) {
                        *least_list = (*least_list)->next;
                        return num;
                    }

                    // if number in most is greater than num, no point checking further in list
                    if(temp_most->data > num){
                        break;
                    }

                    // check next
                    temp_most = temp_most->next;
                }
            }

            // if number in mid is greater than num, no point checking further in list
            if(temp_mid->data > num) {
                break;
            }

            // check next
            temp_mid = temp_mid->next;
        }
        *least_list = (*least_list)->next;
    }

    // no match found
    return -1;
}


// sets candidate pointers for when looking for a match
// allows iterating over the list with the least number of remaining candidates first to reduce comparisons
void set_order_to_compare(candidates** least_cand, candidates** mid_cand, candidates** lon_cand, candidates* row, candidates* col, candidates* box) {
    int rem_in_row = row->remaining;
    int rem_in_col = col->remaining;
    int rem_in_box = box->remaining;

    if(rem_in_row<rem_in_col) {

        // row, col, box order
        if(rem_in_col<rem_in_box) {
            *least_cand = row;
            *mid_cand = col;
            *lon_cand = box;

        // row, box, col order
        } else if (rem_in_row<rem_in_box) {
            *least_cand = row;
            *mid_cand = box;
            *lon_cand = col;

        // box, row, col order
        } else {
            *least_cand = box;
            *mid_cand = row;
            *lon_cand = col;
        }
    } else {

        // col, row, box order
        if(rem_in_row<rem_in_box) {
            *least_cand = col;
            *mid_cand = row;
            *lon_cand = box;

        // col, box, row order
        } else if (rem_in_col<rem_in_box) {
            *least_cand = col;
            *mid_cand = box;
            *lon_cand = row;

        // box, col, row order
        } else {
            *least_cand = box;
            *mid_cand = col;
            *lon_cand = row;
        }
    }
}


// creates structure to contain all candidates for either rows/columns/boxes
// when populated is true, candidate list will be fully populated, else it will point to NULL and remaining will be 0
candidates* generate_candidates(bool populated) {
    candidates* cand, * last, * current;
    cand = (candidates*) malloc(sizeof(candidates));
    if(cand == NULL) {
        printf("Memory allocation failure\n");
        exit(1);
    }

    // sets attributes for first row/column/box
    if(populated == 1) {
        cand -> cand_list = create_box_num_list();
        cand -> remaining = SIZE;
    } else {
        cand -> cand_list = NULL;
        cand -> remaining = 0;
    }
    cand -> id = 0;
    cand -> next = NULL;
    cand -> prev = NULL;

    // tracks last candidate row/column/box to be created
    last = cand;

    // adds candidates for remaining rows/columns/boxes
    for(int i=1; i<SIZE; i++) {
        current = (candidates*) malloc(sizeof(candidates));
        if(current == NULL) {
            printf("Memory allocation failure\n");
            exit(1);
        }

        if(populated == 1) {
            current -> cand_list = create_box_num_list();
            current -> remaining = SIZE;
        } else {
            current -> cand_list = NULL;
            current -> remaining = 0;
        }
        current -> id = i;
        current -> next = NULL;
        current -> prev = last;

        last -> next = current;
        last = current;
    }

    return cand;
}


// creates doubly linked list of all numbers for a box/row/column
node* create_box_num_list() {
    // creates new list
    node* list, * last, *  current;
    list = NULL;

    if(SIZE>0) {
        // allocates memory for first node
        list = (node*) malloc(sizeof(node));
        if(list == NULL) {
            printf("Memory allocation failure\n");
            exit(1);
        }

        // creates first node
        list -> prev = NULL;
        list -> data = 1;
        list -> next = NULL;

        // tracks last node created
        last = list;

        // creates new node for each number in a box and appends to list
        for(int i=2; i<=SIZE; i++) {
            // allocates memory for each new node
            current = (node*) malloc(sizeof(node));
            if(current == NULL) {
                printf("Memory allocation failure\n");
                exit(1);
            }

            // inserts data to new node and sets prev to point at last and sets next to point back at first node
            current -> prev = last;
            current -> data = i;
            current -> next = NULL;

            // sets last node created to point at new node
            last -> next = current;

            // updates last node tracker
            last = current;
        }
    }

    return list;
}


/* randomly fills boxes which do not conflict with each other
   note rows are populated in turn, but the for-loop iterates for number of box rows
   columns are calculated by box cols * box counter
   ie each box is completely filled starting with the top row, before going onto the next row followed by the next box */
void partially_complete(int** grid, candidates* row_cand, candidates* col_cand, candidates* box_cand) {
    int row = 0;
    int col = 0;
    int box_counter = 0;
    candidates* current_box_cand = box_cand;
    node* cand_list, * temp;

    // randomly fills non conflicting boxes
    // iterates boxes diagonally down and to the right
    for(int box=0; box<SIZE; box+=BOX_ROWS+1) {

        // sets pointer to current box candidates
        while(current_box_cand -> id != box){
            current_box_cand = current_box_cand -> next;
        }

        // iterates rows in a box
        for(int i=0; i<BOX_ROWS; i++) {
            // sets column index to beginning of next box to the right
            col = BOX_COLS*box_counter;

            // iterates columns in a box
            for(int j=0; j<BOX_COLS; j++) {
                // sets candidate list pointer
                cand_list=current_box_cand->cand_list;

                // generate random number between 0 and number of box_num_remaining-1
                int cycle_num = rand() % (current_box_cand -> remaining);

                // moves pointer to candidate list of remaining numbers to be placed by the random number, essentially selecting a random number from those remaining for box
                // doesn't move pointer if only 1 remaining
                for(int i=0; i<cycle_num; i++){
                    cand_list = cand_list -> next;
                }

                // inserts random number from box_numbers list into grid
                int insert_num = cand_list -> data;
                grid[row][col] = insert_num;

                // remove candidate number just inserted from box/row/col candidates
                temp = remove_candidate(&current_box_cand, insert_num, current_box_cand->id);
                free(temp);
                temp = NULL;
                temp = remove_candidate(&row_cand, insert_num, row);
                free(temp);
                temp = NULL;
                temp = remove_candidate(&col_cand, insert_num, col);
                free(temp);
                temp = NULL;

                col++;
            }
            row++;
        }
        box_counter++;
    }
}


// inserts new node to candidate list
// used when nodes inserting do not point anywhere when numbers are removed from grid
void insert_candidate(candidates** can, int insert, int id_num) {
    candidates* current_can = *can;

    // allocate memory for new node to insert
    node* res_num = (node*) malloc(sizeof(node));

    res_num->data = insert;
    res_num->next = NULL;
    res_num->prev = NULL;

    // finds correct row/box/col candidates
    while(current_can -> id != id_num) {
        current_can = current_can -> next;
    }


    // if none in list
    if(current_can->remaining == 0) {
        current_can->cand_list = res_num;
    } else if(current_can->cand_list->data > insert) {
        // first in list
        current_can->cand_list->prev = res_num;
        res_num->next = current_can->cand_list;
        current_can->cand_list = res_num;
    } else {
        node* temp = current_can->cand_list;

        // sets temp to largest number lower than insert
        while((temp->next != NULL) && (temp->next->data < insert)) {
            temp = temp->next;
        }

        // if number to be inserted won't be last in list
        if(temp->next != NULL) {
            temp->next->prev = res_num;
            res_num->next = temp->next;
        }

        temp->next = res_num;
        res_num->prev = temp;
    }

    current_can->remaining++;
}


// restores candidate to list
void restore_candidate(candidates** can, node* res_num, int id_num) {
    candidates* current_can = *can;

    // finds correct row/box/col candidates
    while(current_can -> id != id_num) {
        current_can = current_can -> next;
    }

    // if none in list
    if(current_can->remaining == 0) {
        current_can->cand_list = res_num;
    } else {

        // if first in list
        if(res_num->prev == NULL) {
            current_can->cand_list = res_num;
            res_num->next->prev = res_num;
        } else {
            res_num->prev->next = res_num;
        }

        // if not last in list (if it is, it will already be pointing forward to NULL)
        if(res_num->next != NULL) {
            res_num->next->prev = res_num;
        }
    }

    current_can->remaining++;
}


// removes number from candidates list
// beware, can hang if del_num has already been removed from list (shouldn't happen, as only called during grid creation - no user interaction)
node* remove_candidate(candidates** can, int del_num, int id_num) {
    candidates* current_can = *can;
    node* temp;

    // finds correct row/box/col candidates
    while(current_can -> id != id_num) {
        current_can = current_can -> next;
    }

    // temp node, used to free deleted node after updating pointers
    temp = current_can -> cand_list;

    // sets temp to be pointing at correct node to delete
    while(temp->data!=del_num) {
        temp = temp->next;
    }

    // removes candidate del_num
    // only updates list pointers if more than one,
    // else can set list to be NULL if last number used
    if(current_can -> remaining > 1) {
        // if first node in list
        if(temp->prev == NULL){
            current_can->cand_list = temp->next;
            current_can->cand_list->prev = NULL;

        // if last node in list
        } else if (temp->next == NULL) {
            temp->prev->next = NULL;
        } else {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
        }
    } else {
        current_can -> cand_list = NULL;
    }

    // updates number of candidates remaining
    current_can -> remaining--;

    return temp;
}


// iterates through grid to print game board
void display_board(int** grid) {
    int row;
    int col;

    // indents top left corner of board (header row)
    printf("    ");

    // prints column headers
    for(int c=1; c<=SIZE; c++) {
        // formats for double digits in grid
        if(SIZE>9) {
            printf(" ");
        }

        // converts int to char
        printf("%c ", c+64);

        // increases spacing for new box headers, checks if end of a box horizontally
        if((c)%BOX_COLS == 0) {
            printf("  ");
        }
    }
    printf("\n");

    sepearator_row();

    // iterates rows
    for(row=0; row<SIZE; row++) {

        // iterates columns
        for(int col=0; col<SIZE; col++) {

            // row ID
            if(col==0){
                if(row<9) {
                    printf(" %d| ", row+1);
                }else {
                    // double digit row ID
                    printf("%d| ", row+1);
                }
            }

            // print board values
            int value = grid[row][col];
            if(SIZE>9 && value<10) {
                // formats for double digit values in grid
                printf(" ");
            }
            if(value == 0){
                // increases readability rather than using 0's
                printf("_ ");
            }else{
                printf("%d ", value);
            }

            // box separator, checks if end of a box horizontally
            if((col+1)%BOX_COLS == 0) {
                printf("| ");
            }
        }

        printf("\n");

        // inserts separator row between boxes, checks if end of box vertically
        if((row+1)%BOX_ROWS == 0) {
            sepearator_row();
        }
    }
}


// initialise grid
void init(int** grid) {
    int row;
    int col;

    // iterates through grid and setting each cell to 0
    for(row=0; row<SIZE; row++) {
        for(col=0; col<SIZE; col++){
            grid[row][col] = 0;
        }
    }
}


// prints a separator row
void sepearator_row() {
    // covers row id column
    printf("  -");
    // for grid sizes under 10, amount of dashes required is double the number of columns per box + 2
    // for grid sizes over 10, amount of dashes required is triple the number of columns per box + 2
    // for all cases, then multiply by number of boxes horizontally which coincides with the number of rows per box
    if(SIZE<10) {
        for(int i=0; i<(BOX_COLS*2+2)*BOX_ROWS; i++) {
            printf("-");
        }
    }else {
        for(int i=0; i<(BOX_COLS*3+2)*BOX_ROWS; i++) {
            printf("-");
        }
    }
    printf("\n");
}


// calculate box number (0 indexed)
int get_box_num(int row, int col) {
    int box_num = ((int)row) / BOX_ROWS * BOX_ROWS + ((int)col) / BOX_COLS;
    return box_num;
}
