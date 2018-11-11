#include <stdio.h>
#include <stdlib.h>
#include "minesweep.h"
#include "load.h"

//Author: David Vargas Puga

/* function prototypes */
void print_game(game*);
void free_game(game*);
int play_game(char*);
int process_click(game*, int, int, int);
int uncover(game*, int, int);
int check_game(game*);
int is_safe(int, int, int, int);

//prints the the board:
void print_game(game* current_board)
{
    //runs a for loop that iterates through the 2D array.
    //prints specificed characters if certain conditions
    for(int i = 0; i < current_board->row_max; i++)
    {
        for(int j = 0; j<current_board->col_max; j++)
        {
            if((current_board->cells[i][j].color == white)
               && (( current_board->cells[i][j].mine > 0) ||(current_board->cells[i][j].mine < 9)))
            {
                printf("%d ", current_board->cells[i][j].mine); //if cell color white & cell has mine count, print count
            }
            else if(current_board->cells[i][j].color == white)
            {
                printf("0 "); //if cell color white, print 0
            }
            else if(current_board->cells[i][j].color == black)
            {
                printf("B "); //if cell color black, print B
            }
            else if(current_board->cells[i][j].color == gray)
            {
                printf("E "); //if cell color gray, print E
            }
        }
        printf("\n");
    }
}

//checks if game is won or not:
int check_game (game* current_state)
{
    for(int i = 0; i < current_state->row_max; i++)
    {
        for(int j = 0; j < current_state->col_max; j++)
        {
            //if a cell with a mine has cell color other than black, game is not won
            if(current_state->cells[i][j].mine == -1 && current_state->cells[i][j].color != black)
                return 0;
            //if a cell does not has a mine but the color is black, game is not won
            if(current_state->cells[i][j].mine == 0 && current_state->cells[i][j].color == black)
                return 0;
        }
    }
    return 1; //otherwise, the game is won
}

//returns game status if ended successfully or not:
int play_game(char* file_name)
{
    //initializes variables
    int x=0;
    int y=0;
    int move=0;
    
    //creates the game pointer and calls file_load to read a file
    game * my_game = file_load(file_name);
    if(my_game == NULL)
    {
        return 0; //if file is not opened, quit game
    }
    
    //prints the current board
    printf("\n");
    print_game(my_game);
    printf("\n");
    
    //while the the game is not won
    while(1)
    {
        
        printf("Enter your move in the format row:col move\n");
        scanf("%d:%d %d", &x, &y, &move); //reads command line for click's x/y and click type
        if(x == -1) //if user enters -1, quits the game
        {
            if(check_game(my_game)) printf("CONGRATS YOU WON!\n"); //if won, prints message
            else printf("YOU LOST\n"); //if lost, prints message
            free_game(my_game); //deallocates memory for the game object
            return 0;
        }
        
        process_click(my_game, x, y, move); //calls process_click with the given x/y and click type
        if(process_click(my_game, x, y, move) != 1)
        {
            printf("GAME OVER\n"); //if the click was unsuccessful, game is over
            free_game(my_game); //deallocates memory for the game object
            return 0; //if the click was not processed succesful, game ends
        }
        //prints the current board
        printf("\n");
        print_game(my_game);
        printf("\n");
    }
    return 1; //return 1 if ended successfully
}

//return 1 if click was processed succesfully and game continues. otherwise, return 0:
int process_click(game* current_board, int x, int y, int move)
{
    //initializes x and y one less to compensate for the lesser value of arrays
    x--;
    y--;
    if( !(is_safe(x,y, current_board->row_max, current_board->col_max))) //checks to make sure position is valid
    {
        printf("Position is not safe");
        return 0;
    }
    if(move == 1)
    {
        current_board->cells[x][y].color= black; //if move 1, set cell color to black
        return 1;
    }
    else
    {
        if(current_board->cells[x][y].color == gray
           && current_board->cells[x][y].mine > 0)
            current_board->cells[x][y].color = white; //otherwise, if cell color is gray & valid mine count, cell color is white
        else if(current_board->cells[x][y].color == white)
        {
            //otherwise, if cell color is white, do nothing
        }
        else if(current_board->cells[x][y].mine == -1)
            return 0; //otherwise, if cell is a mine, game is over
        else
            uncover(current_board, x, y); //otherwise, otherwise, uncover all adjacent cells
    }
    return 1; //return 1 if click was processed succesfully
}

//method to uncover a specific cell:
int uncover(game* current_board, int row, int col)
{
    if(is_safe(row, col, current_board->row_max, current_board->col_max)) //checks that cell is safe
    {
        if(current_board->cells[row][col].color == white)
        {
            //if the cell color is white, do nothing
        }
        else if(current_board->cells[row][col].color == black) return 0; //if the cell color is black, game is over
        else if (current_board->cells[row][col].color == gray && current_board->cells[row][col].mine > 0){
            current_board->cells[row][col].color = white;
            return 1;
        }else if(current_board->cells[row][col].color == gray)
        {	    current_board->cells[row][col].color = white; //if cell color is gray, set to white
            
            //following lines uncover adjacent cells:
            uncover(current_board, row-1, col-1);
            uncover(current_board, row-1, col);
            uncover(current_board, row-1, col+1);
            uncover(current_board, row, col-1);
            uncover(current_board, row, col+1);
            uncover(current_board, row+1, col-1);
            uncover(current_board, row+1, col);
            uncover(current_board, row+1, col+1);
            
        }
        return 1; //otherwise
    }
}

//checks if the cell is valid:
int is_safe(int row, int col, int row_max, int col_max)
{
    if((0 <= row && row < row_max) && (0 <= col && col < col_max)) return 1; //if row and col are valid offset values, return 1
    else return 0; //otherwise, return 0;
}

//deallocates the memory of a game object:
void free_game(game* current_board)
{
    
    for(int j = 0; j<current_board->row_max; j++)
    {
        free(current_board->cells[j]); //frees each column in a row
    }
    free(current_board->cells); //after each column in each row is freed, free the specific row
    
    free(current_board); //then free the whole game object
}
