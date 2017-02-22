#include <stdio.h>

#define POPULATE   3//Number of neighbours required to come to life, and more and the tile will die of overpopulation
#define STARVE     2//Number of neighbours needed for a healthy food supply, any less and it will starve

//Print the current life array to the console using pretty ASCII characters to show the state of the game
void display(int rows,int cols,int life[rows][cols]) {
    printf("\n");//Spacers
    for (int i = 0; i < rows; i++){//Cycle through array
        for (int j = 0; j < cols; j++) {
            if (life[i][j]){//This tile is alive, make it dark
                printf("\u2593");
            }else {//This tile is dead, shade it lightly
                printf("\u2591");
            }
        }
        printf("\n");
    }
}

//Check for neighbour at specific position, and wrap around board if necessary
int wrapCheck(int r,int c,int rows,int cols,int life[rows][cols]){
    if (r<0){//Bounds checking and wrapping
        r=rows - 1;
    }
    if (r>=rows){
        r=0;
    }
    if (c<0){
        c=cols - 1;
    }
    if (c>=cols){
        c=0;
    }
    return life[r][c];//Is this tile alive?
}

//How many living neighbours does a given tile have?
int countNeighbours(int rows, int cols, int r, int c, int life[rows][cols]){
    int count = 0;
    for (int i = -1; i < 2; i++) {//Check a 3x3 square centred on this tile
        for (int j = -1; j < 2; j++) {
            if (i||j) { //Make sure we don't count tile as its own neighbour
                count += wrapCheck(r + i, c + j, rows, cols, life);//We need to use a function to check whether we should wrap around the array bounds
            }
        }
    }
    return count;
}

//Simulate the passage of one generation
void doStep(int rows, int cols, int life[rows][cols]){
    int newLife[rows][cols];//We need a new temporary array to do a whole generation at once
    for (int r = 0; r<rows; r++){
        for (int c = 0; c<cols; c++) {
            int neighbours = countNeighbours(rows, cols, r, c, life); //How many neighbours do we have?
            if (neighbours < STARVE || neighbours > POPULATE) { //Outside survivable range, die
                newLife[r][c] = 0;
            } else if (neighbours == POPULATE) { //Spawn new life
                newLife[r][c] = 1;
            } else {
                newLife[r][c] = life[r][c]; //Stagnate at current life
            }
        }
    }
    //Now copy the data back to the main array, since this generation has been simulated
    for (int r=0; r<rows; r++){
        for (int c=0; c<cols; c++){
            life[r][c] = newLife[r][c];
        }
    }
}

//Allow the user to give life, and take it, to/from any given tile. Pretend the array starts at 1,1 so we can use 0 for 'stop'
void populate(int rows, int cols, int life[rows][cols]){
    int r,c;
    while (1){
        display(rows,cols,life);
        printf("Enter a row, then a column, in the format \"1,1\", to toggle that cell, or enter 0 to stop:\n");
        scanf("%d,%d",&r,&c);//Get a coordinate pair to toggle
        if(!r || !c){//One of the arguments was zero, nice boolean check for stopping
            break;
        }
        r--;//Correct for zero-based array, user doesn't need to know
        c--;
        if (r<0 || c < 0 || r>=rows || c>=cols){//They tried to toggle a tile that doesn't exist
            printf("Invalid position\n");
            continue;
        }
        life[r][c] = !life[r][c];//Toggle the status of the tile
    }
}

int main() {
    int rows=0,cols=0,generations = 0;//Initialise some variables
    printf("Welcome to Conway's Game of Life\n2017-02-10 Dominic Brady (dob22@aber.ac.uk)\n");//Say hi
    printf("Enter the dimensions of the grid to begin:\nRows:");//Get user input
    scanf("%d",&rows);
    printf("Columns:");
    scanf("%d",&cols);
    int life[rows][cols];
    for (int i=0; i<rows; i++){ //Initialise the array so we don't get a random start situation, though it can be interesting to run without doing this part
        for (int j=0; j<cols; j++){
            life[i][j] = 0;
        }
    }
    printf("Now to populate the array\n");//More hand holding, let the user know what's happening
    populate(rows,cols,life);
    printf("How many generations should we run for?\n");
    scanf("%d",&generations);
    display(rows,cols,life);//Show the initial state
    for (int i=0; i<generations; i++) {//Now do the actual simulation and display after each generation
        doStep(rows,cols, life);
        display(rows,cols, life);
    }
    return 0;
}