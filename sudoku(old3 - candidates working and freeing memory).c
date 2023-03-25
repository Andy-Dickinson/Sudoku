#include <stdio.h>
#include <math.h>
#include <time.h>

#define SIZE 9

// circular doubly linked list of all possible numbers to be entered in a box/row/column
typedef struct node {
    int data;
    struct node* next;
    struct node* prev;
} node;

// maintains list and attributes for each box/row/column - doubly linked list, NOT circular
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
node* create_box_num_list(); // creates doubly linked circular list of all numbers for a box/row/column
void clean_up(int** grid, candidates* row_cand, candidates* col_cand, candidates* box_cand); // deallocates memory
candidates* generate_candidates(); // creates structure (doubly linked list) to contain all candidates for either rows/columns/boxes

//////////////////////currently not being used
int get_box_num(int row, int col); // calculate box number (0 indexed)


// program starts here
int main() {

    // sets seed for random number generator
    srand(time(0));

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

    clean_up(grid, row_cand, col_cand, box_cand);

    return 0;
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

// creates doubly linked circular list of all numbers for a box/row/column
node* create_box_num_list() {
    // creates new list
    node* list, * last, *  current;
    list = NULL;

    if(SIZE>0) {
        // allocates memory for first node
        list = (node*) malloc(sizeof(node));

        // creates first node
        list -> prev = list;
        list -> data = 1;
        list -> next = list;

        // tracks last node created
        last = list;

        // creates new node for each number in a box and appends to list
        for(int i=2; i<=SIZE; i++) {
            // allocates memory for each new node
            current = (node*) malloc(sizeof(node));

            // inserts data to new node and sets prev to point at last and sets next to point back at first node
            current -> prev = last;
            current -> data = i;
            current -> next = list;

            // sets last node created to point at new node
            last -> next = current;

            // updates last node tracker
            last = current;
        }

        // sets first node to point back at last
        list -> prev = current;
    }

    return list;
}

// randomly fills boxes which do not conflict with each other
// note rows are iterated in tern, but the for loop iterates for number of box rows
// columns are calculated by box cols * box index
void partially_complete(int** grid, candidates** row_cand, candidates** col_cand, candidates** box_cand) {
    int row = 0;
    int col = 0;
    candidates* current_box_cand = box_cand;

    // randomly fills non conflicting boxes
    // iterates boxes diagonally down and to the right
    for(int box=0; box<SIZE/BOX_COLS; box++) {

        // sets pointer to current box candidates
        while(current_box_cand -> id != box){
            current_box_cand = current_box_cand -> next;
        }

        // iterates rows in a box
        for(int i=0; i<BOX_ROWS; i++) {
            // sets column index to beginning of next box to the right
            col = BOX_COLS*box;
            // iterates columns in a box

            for(int j=0; j<BOX_COLS; j++) {
                // generate random number between 0 and number of box_num_remaining-1
                int cycle_num = rand() % (current_box_cand -> remaining);

                // loops pointer to list of remaining numbers to be placed, essentially selecting a random number from those remaining for box
                // doesn't loop if only 1 remaining
                for(int i=0; i<cycle_num; i++){
                    current_box_cand -> cand_list = current_box_cand -> cand_list -> next;
                }

                // inserts random number from box_numbers list into grid
                int insert_num = current_box_cand -> cand_list -> data;
                grid[row][col] = insert_num;

                // remove candidate number just inserted from box/row/col candidates
                remove_candidate(current_box_cand, insert_num, current_box_cand->id);
                remove_candidate(row_cand, insert_num, row);
                remove_candidate(col_cand, insert_num, col);

                col++;
            }
            row++;
        }
    }
}

// removes number from candidates list
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
    // only updates list pointers if more than one
    // as if only 1 element remaining, it will be pointing at itself
    if(current_can -> remaining > 1) {
        temp ->prev->next = temp->next;
        temp->next->prev = temp->prev;
        current_can->cand_list= temp->next;
    }

    // sets candidates list to NULL if last remaining number used
    if(current_can -> remaining == 1) {
        current_can -> cand_list = NULL;
    }

    free(temp);
    temp = NULL;

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
