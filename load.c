#include "load.h"
#include "minesweep.h"
#include "cell.h"

//Author: David Vargas Puga

//return allocated and value instantiated game object of NULL if error:
game* file_load(char* file_name)
{
    
    game * my_game = malloc(sizeof(game)); //allocates memory for a game object
    if(my_game == NULL) return NULL; //checks if memory was allocated successfully
    FILE * my_file = fopen(file_name, "r"); //opens the given file for reading
    if(my_file == NULL)
    {
        printf("Could not open file.\n");
        return NULL; //checks if file opened successfully
    }
    fscanf(my_file, "%d %d", &(my_game->row_max), &(my_game->col_max)); //retrieves the row/col max from file
    printf("%d %d\n", my_game->row_max, my_game->col_max);
    
    if((my_game->row_max < 3) || (my_game->col_max < 3)) //checks if rows and cols are more than 3
    {
        printf("The board must be at least 3 rows and at least 3 columns.\n");
        return NULL;
    }
    
    cell** cells = malloc (sizeof(cell*) * my_game->row_max); //allocates memory for the rows of cells
    for(int i = 0; i < my_game->row_max; i++)
    {
        cells[i] = malloc(sizeof(cell) * my_game->col_max); //allocates memory for the columns in each row of cells
    }
    
    for(int i = 0; i < my_game->row_max; i++)
    {
        for(int j = 0; j < my_game->col_max; j++)
        {
            fscanf(my_file, "%d ", &(cells[i][j].mine)); //reads integer from file and stores it in a cell
            if((cells[i][j].mine > 8) || (cells[i][j].mine < -1)) //checks for valid count
            {
                printf("The count of mines must be >-1 and <9\n");
                return 0;
            }
            cells[i][j].color = gray; //sets each cell color to gray
        }
    }
    my_game->cells=cells; //sets the cells 2D array to the game object cells
    fclose(my_file); //closes file
    return my_game; //return game object
}

