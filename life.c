/**
 * @file life.c
 *
 * @author Mariano Garcia
 *
 * @date 4/24/2020
 *
 * Assignment: Lab 10
 *
 * @brief Defining the functions that will be used
 *
 * @details This is going to be defining the functions that will be used in gl.c
 *
 * @bugs none
 *
 * @todo none
 */
#include <stdio.h>
#include <stdlib.h>
#include "life.h"

#define NUM (8)
static const unsigned char DEAD = 0;
static const unsigned char ALIVE = 1;

/**
 * This is going to allocate the life cells
 * @param  rows This will be the rows for the matrices
 * @param  cols The columns for the matrices
 * @return      This will return the allocated memory
 */
unsigned char **alloc_life(int rows, int cols)
{
    int i,j;
    unsigned char **a = malloc(rows * sizeof(unsigned char *));
    if (!a) return NULL;

    for (i=0; i<rows; i++) {
        a[i] = malloc(cols * sizeof(unsigned char));
        if (!a[i]) {
            for (j=0; j < i; j++) free(a[j]);
            free(a);
            return NULL;
        }
    }
    return a;
}

// free memory allocated for matrix of rows X cols
// instead passing the matrix pass the address of that matrix
// so that matrix can be set to NULL
/**
 * This is going to free memory that was allocated for the matrix
 * @param plife printing life
 * @param rows  The rows for the matrix
 * @param cols  The columns for the matrix
 */
void free_life(unsigned char ***plife, int rows, int cols)
{
    unsigned char ** life = *plife;
    int i;
    for(i=0; i<rows; i++) {
        free(life[i]);
    }
    free(life);
    life = NULL;
}

/**
 * This will reset the life after being transfered to other matrix
 * @param life The life that is alive in previous matrix
 * @param rows Rows for the matrix
 * @param cols Columns for the matrix
 */
void reset_life(unsigned char **life, int rows, int cols) {
    if (!life) return;
    int i,j;
    for (i=0; i < rows; i++) {
        for (j=0; j < cols; j++) {
            life[i][j] = 0;
        }
    }
}

/**
 * This will initalize life for the matrix
 * @param  life     The live ccells
 * @param  rows     The rows for the matrix
 * @param  cols     The columns for the matrix
 * @param  edge     The edge that will be in use
 * @param  cur      The initial current positions
 * @param  filename Which file you would like to use (getopts)
 * @return          Returns your simulation
 */
unsigned char** init_life_from_file(unsigned char **life, int rows, int cols, char edge,
                        Cell cur, // initial position in matrix
                        const char *filename) //
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Could not open file %s\n", filename);
       return NULL;
    }
    char line[1024] = { 0 };
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        //if (line[0] == '#') continue;
        int x,y;
        sscanf(line, "%d %d", &x, &y);
        // printf("X=%d, Y=%d\n", x,y); // DEBUG print
        Cell cell;
        cell.col = x;
        cell.row = y;
        cur = get_next_cell(rows, cols, edge, cur, cell);
        fill_cell_in_life(life, rows, cols, cur, ALIVE);
    }
    fclose(fp);
    return life;
}

/**
 * This is the second matrix. This will follow the rules of Conway's Game of lifeA
 * @param  rows The rows of the matrix
 * @param  cols The coulmns of the matrix
 * @param  edge The type of edge
 * @param  cur  The current position of the cells
 * @param  dist The distance from the current
 * @return      Returns updated cells
 */
Cell get_next_cell(int rows, int cols, char edge,
                        Cell cur, Cell dist)
{
    Cell cell = {0, 0}; // position of next cell
    if (edge == 'H') {
        cell.row = cur.row + dist.row;
        cell.col = cur.col + dist.col;
        // if nbr is within matrix then return nbr
        // otherwise return original
        if (cell.row >= 0 && cell.row < rows &&
            cell.col >= 0 && cell.col < cols)
        {
            return cell;
        } else {
            return cur;
        }
    }
    if (edge == 'T') {
        cell.row = (cur.row + dist.row) % rows;
        cell.col = (cur.col + dist.col) % cols;
        if (cell.row < 0 ) cell.row += rows;
        if (cell.col < 0 ) cell.col += cols;
    }
    if (edge == 'K') {
        cell.row = (cur.row + dist.row) % rows;
        cell.col = (cur.col + dist.col) % cols;
        if (cell.row < 0) {
            cell.row = rows - 1;
        }else if (cell.row > rows - 1) {
            cell.row = 0;
        }

    }
    return cell;
}

/**
 * This will get the cells from the updated function.
 * @param life  The life from the matrix
 * @param rows  The rows of the matrix
 * @param cols  The columns of the matrix
 * @param cell  The cells count
 * @param value The
 */
void fill_cell_in_life(unsigned char **life, int rows, int cols,
                  Cell cell, unsigned char value)
{
    if (cell.row >= 0 && cell.row < rows &&
        cell.col >= 0 && cell.col < cols) {
        life[cell.row][cell.col] = value;
    } else {
        printf("Error: can not fill %u at %d %d\n", value, cell.row, cell.col);
    }
}

// get number of alive NUM of 'cell'
/**
 * This is going to get a count of alive cells
 * @param  life Life cells
 * @param  rows The rows of the matrix
 * @param  cols The columns of the matrix
 * @param  edge The edge type
 * @param  cell Cell count
 * @return      Returns count of alive cells
 */
int get_alive_NUM(unsigned char **life, int rows, int cols, char edge, Cell cell)
{
    // distances of rows and cols for all the eight neighbors
    static const int nbrRowsDist[NUM] = {-1, -1, -1,  0, 0,  1, 1, 1 };
    static const int nbrColsDist[NUM] = {-1,  0,  1, -1, 1, -1, 0, 1 };

    int aliveNUM = 0;
    int n;
    for (n=0; n<NUM; n++) // all NUM of each cell
    {
        Cell nbrDist;
        nbrDist.row = nbrRowsDist[n];
        nbrDist.col = nbrColsDist[n];
        // get the position of nbr cell based on edge type
        Cell nbr = get_next_cell(rows, cols, edge, cell, nbrDist);

        // when edge was 'hedge' few NUM of cell on edge can't cross broder
        // hence for such nbr position would be same as original
        // consider that nbr was dead cell and continue.
        if (nbr.row == cell.row && nbr.col == cell.col) {
            continue;
        }
        if (life[nbr.row][nbr.col]) aliveNUM++;
    }
    return aliveNUM;
}

/**
 * This will process the next generation
 * @param  lifeA The Source Matrix
 * @param  rows  The Rows of the matrix
 * @param  cols  The Columns of the matrix
 * @param  lifeB The destination matrix
 * @param  edge  Edge type
 * @return       The brand new updated matrix
 */
unsigned char** process_generation(unsigned char **lifeA, int rows, int cols,
                           unsigned char **lifeB, char edge)
{
    // before re-draw new gget_nexeneration reset all cells to dead
    reset_life(lifeB, rows, cols);
    int r,c;
    for (r=0; r<rows; r++) { // for all rows
        for (c=0; c<cols; c++) { // for all cols
            Cell cell;
            cell.row = r;
            cell.col = c;
            int aliveNUM = get_alive_NUM(lifeA, rows, cols, edge, cell);
            //int deadNUM = NUM - aliveNUM;
            if (ALIVE == lifeA[cell.row][cell.col]) { // for alive cell
                if (aliveNUM < 2 || aliveNUM > 3) {
                    fill_cell_in_life(lifeB, rows, cols, cell, DEAD);
                } else {
                    fill_cell_in_life(lifeB, rows, cols, cell, ALIVE);
                }
            } else { // for dead cell
                if (aliveNUM == 3) {
                    fill_cell_in_life(lifeB, rows, cols, cell, ALIVE);
                }
            }
        }
    }
    return lifeB;
}
