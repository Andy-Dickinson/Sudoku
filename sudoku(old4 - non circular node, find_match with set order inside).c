#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define SIZE 9

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

// stored as global variables to avoid recalculating - will be constant for each game board size
int BOX_ROWS = floor(sqrt(SIZE)); // rows per box - 9x9 board will be 3, 6x6 will be 2
int BOX_COLS = ceil(sqrt(SIZE)); // cols per box - 9x9 board will be 3, 6x6 will be 3

void display_board(int** grid); // iterates through grid to print game board
void init(int** grid); // initialise grid
void sepearator_row(); // prints a row of --- dependant on board SIZE
void partially_complete(int** grid, candidates** row_cand, candidates** col_cand, candidates** box_cand); // randomly fills boxes which do not conflict with each other
node* create_box_num_list(); // creates doubly linked list of all numbers for a box/row/column
void clean_up(int** grid, candidates* row_cand, candidates* col_cand, candidates* box_cand); // deallocates memory
candidates* generate_candidates(); // creates structure (doubly linked list) to contain all candidates for either rows/columns/boxes
int check_match(node* row_list, node* col_list, node* box_list); // returns true if first number in each list match, false otherwise
void set_order_to_compare(node** least, node** mid, node** most, int* rem_least, int* rem_mid, int* rem_most, candidates* row, candidates* col, candidates* box); // sets node pointers for when looking for a match to reduce comparisons
int find_match(candidates* row_cand, candidates* col_cand, candidates* box_cand); // returns a matching candidate that exists in all lists (row/col/box)

//////////////////////currently not being used
int get_box_num(int row, int col); // calculate box number (0 indexed)


// program starts here
int main() {

    // sets seed for random number generator
//    srand(time(0));
    srand(1);/////////////////////////////////////////////////////////////////////////////////////////////////

    // declare grid
    int** grid;

    // allocate memory for grid
    // memory allocation code and 2d array set-up adapted from
    // (No name or date, Multidimensional arrays in C, https://www.uio.no/studier/emner/matnat/ifi/IN3200/v19/teaching-material/multidimarrays.pdf)
    grid = malloc(SIZE * sizeof *grid);
    if(grid == NULL) {
        printf("Memory allocation failure\n");
        return 1;
    }

    grid[0] = malloc(SIZE*SIZE * sizeof *grid[0]);
    if(grid[0] == NULL) {
        printf("Memory allocation failure\n");
        return 1;
    }

    for(int i=1; i<SIZE; i++) {
        grid[i] = &(grid[0][i*SIZE]);
    }

    // initialise grid, setting each cell to 0
    init(grid);

    // generate candidates
    // each points to the first candidates list in row/col/box
    candidates* row_cand = generate_candidates();
    candidates* col_cand = generate_candidates();
    candidates* box_cand = generate_candidates();

    // randomly fills boxes which do not conflict with each other
    partially_complete(grid, row_cand, col_cand, box_cand);

    // print board
    display_board(grid);


candidates* temp_row = row_cand;
candidates* temp_col = col_cand;
candidates* temp_box = box_cand;

// sets col to 3 and box to 1 (row is on 0)
temp_col= temp_col->next->next->next;
temp_box=temp_box->next;

// 6 in row
//remove_candidate(temp_row, 2, 0); // 5 now remaining
//remove_candidate(temp_row, 3, 0); // 4 now remaining

// 6 in col
remove_candidate(temp_col, 2, 3); // 5 now remaining
//remove_candidate(temp_col, 5, 3); // 4 now remaining

// 9 in box
remove_candidate(temp_box, 1, 1); // 8 now remaining
remove_candidate(temp_box, 3, 1); // 7 now remaining
remove_candidate(temp_box, 4, 1); // 6 now remaining
remove_candidate(temp_box, 5, 1); // 5 now remaining
remove_candidate(temp_box, 6, 1); // 4 now remaining

printf("remaining, row = %d, col = %d, box = %d\n", temp_row->remaining, temp_col->remaining, temp_box->remaining);
printf("box remains = 2, 7, 8, 9\n");
printf("col remains = 1, 5, 6, 7, 8\n");
printf("row remains = 1, 2, 3, 5, 7, 8\n");
find_match(temp_row, temp_col, temp_box);



    clean_up(grid, row_cand, col_cand, box_cand);

    return 0;
}


// returns a matching candidate that exists in all lists (row/col/box)
int find_match(candidates* row_cand, candidates* col_cand, candidates* box_cand) {
    int rem_in_least;
    int rem_in_mid;
    int rem_in_most;
    node* least, * mid, * most, * temp_mid, * temp_most;
    int num;

    // if none remain in any list, then cannot be a match
    if(row_cand->remaining == 0 || col_cand->remaining == 0 || box_cand == 0) {
        return NULL;
    }

    // sets node pointers and remaining variables so iterating for match will take minimal comparisons
    set_order_to_compare(&least, &mid, &most, &rem_in_least, &rem_in_mid, &rem_in_most, row_cand, col_cand, box_cand);

printf("least = %d\n", least->data);

    // iterates smallest list (this will never need resetting)
    for(int i=0; i<rem_in_least; i++) {
        // gets first number from smallest list
        num = least->data;

        // resets temp nodes which have pointers moved forward in loop
        temp_mid = mid;
        temp_most = most;

        // iterates medium length list
        for(int j=0; j<rem_in_mid; j++) {

            // if number matches mid length list, check longest list
            if(num == temp_mid->data) {

                for(int k=0; k<rem_in_most; k++){
                    // if match found, return
                    if(num == temp_most->data) {
                        printf("match = %d\n", num);
                        return num;
                    }

                    // if number in most is greater than num, no point checking further in list
                    if(temp_most->data>num){
                        break;
                    }

                    // check next
                    temp_most = temp_most->next;
                }
            }

            // if number in mid is greater than num, no point checking further in list
            if(temp_mid->data>num) {
                break;
            }

            // check next
            temp_mid = temp_mid->next;
        }
        least = least->next;
    }

    // no match found
    return NULL;
}

// sets node pointers for when looking for a match
// allows iterating over the list with the least number of remaining candidates first to reduce comparisons
void set_order_to_compare(node** least_list, node** mid_list, node** lon_list, int* rem_least, int* rem_mid, int* rem_most, candidates* row, candidates* col, candidates* box) {
    int rem_in_row = row->remaining;
    int rem_in_col = col->remaining;
    int rem_in_box = box->remaining;

    if(rem_in_row<rem_in_col) {

        // row, col, box order
        if(rem_in_col<rem_in_box) {
            *least_list = row->cand_list;
            *mid_list = col->cand_list;
            *lon_list = box->cand_list;
            *rem_least = rem_in_row;
            *rem_mid = rem_in_col;
            *rem_most = rem_in_box;

        // row, box, col order
        } else if (rem_in_row<rem_in_box) {
            *least_list = row->cand_list;
            *mid_list = box->cand_list;
            *lon_list = col->cand_list;
            *rem_least = rem_in_row;
            *rem_mid = rem_in_box;
            *rem_most = rem_in_col;

        // box, row, col order
        } else {
            *least_list = box->cand_list;
            *mid_list = row->cand_list;
            *lon_list = col->cand_list;
            *rem_least = rem_in_box;
            *rem_mid = rem_in_row;
            *rem_most = rem_in_col;
        }
    } else {

        // col, row, box order
        if(rem_in_row<rem_in_box) {
            *least_list = col->cand_list;
            *mid_list = row->cand_list;
            *lon_list = box->cand_list;
            *rem_least = rem_in_col;
            *rem_mid = rem_in_row;
            *rem_most = rem_in_box;

        // col, box, row order
        } else if (rem_in_col<rem_in_box) {
            *least_list = col->cand_list;
            *mid_list = box->cand_list;
            *lon_list = row->cand_list;
            *rem_least = rem_in_col;
            *rem_mid = rem_in_box;
            *rem_most = rem_in_row;

        // box, col, row order
        } else {
            *least_list = box->cand_list;
            *mid_list = col->cand_list;
            *lon_list = row->cand_list;
            *rem_least = rem_in_box;
            *rem_mid = rem_in_col;
            *rem_most = rem_in_row;
        }
    }
}

// returns true if first number in each list match, false otherwise
int check_match(node* row_list, node* col_list, node* box_list) {
    if((row_list->data == col_list->data) && (row_list->data == box_list->data)) {
        return true;
    }
    else return false;
}


// creates structure to contain all candidates for either rows/columns/boxes
candidates* generate_candidates() {
    candidates* cand, * last, * current;
    cand = (candidates*) malloc(sizeof(candidates));

    // sets attributes for first row/column/box
    cand -> cand_list = create_box_num_list();
    cand -> id = 0;
    cand -> next = NULL;
    cand -> prev = NULL;
    cand -> remaining = SIZE;

    // tracks last candidate row/column/box to be created
    last = cand;

    // adds candidates for remaining rows/columns/boxes
    for(int i=1; i<SIZE; i++) {
        current = (candidates*) malloc(sizeof(candidates));

        current -> cand_list = create_box_num_list();
        current -> id = i;
        current -> next = NULL;
        current -> prev = last;
        current -> remaining = SIZE;

        last -> next = current;
        last = current;
    }

    return cand;
}


// deallocates memory
void clean_up(int** grid, candidates* row_cand, candidates* col_cand, candidates* box_cand) {
    candidates* temp;

    free(grid[0]);
    free(grid);


    while(row_cand -> next != NULL) {
        temp = row_cand;
        row_cand = row_cand -> next;
        free(temp);
    }
    free(row_cand);

    while(col_cand -> next != NULL) {
        temp = col_cand;
        col_cand = col_cand -> next;
        free(temp);
    }
    free(col_cand);

    while(box_cand -> next != NULL) {
        temp = box_cand;
        box_cand = box_cand -> next;
        free(temp);
    }
    free(box_cand);
}


// creates doubly linked list of all numbers for a box/row/column
node* create_box_num_list() {
    // creates new list
    node* list, * last, *  current;
    list = NULL;

    if(SIZE>0) {
        // allocates memory for first node
        list = (node*) malloc(sizeof(node));

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


// randomly fills boxes which do not conflict with each other
// note rows are populated in tern, but the for-loop iterates for number of box rows
// columns are calculated by box cols * box counter
// ie each box is completely filled starting with the top row, before going onto the next row
void partially_complete(int** grid, candidates** row_cand, candidates** col_cand, candidates** box_cand) {
    int row = 0;
    int col = 0;
    int box_counter = 0;
    candidates* current_box_cand = box_cand;
    node* cand_list;

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
                remove_candidate(current_box_cand, insert_num, current_box_cand->id);
                remove_candidate(row_cand, insert_num, row);
                remove_candidate(col_cand, insert_num, col);

                col++;
            }
            row++;
        }
        box_counter++;
    }
}


// removes number from candidates list
// beware, can hang if del_num has already been removed from list (shouldn't happen, as only called during grid creation - no user interaction)
void remove_candidate(candidates** can, int del_num, int id_num) {
    candidates* current_can = can;
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
            temp ->prev->next = temp->next;
            temp->next->prev = temp->prev;
        }
    } else {
        current_can -> cand_list = NULL;
    }

    free(temp);

    // updates number of candidates remaining
    current_can -> remaining--;
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
    // 3 dashes to cover row id column
    printf("---");
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
