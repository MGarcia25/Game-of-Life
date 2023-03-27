/**
 * @file life.h
 *
 * @author Mariano Garcia
 *
 * @date 4/24/2020
 *
 * Assignment: Lab 10
 *
 * @brief The main header file for life.h
 *
 * @details This is going to contain the prototypes for life.h
 *
 * @bugs none
 *
 * @todo none
 */
#ifndef LIFE_H_
#define LIFE_H_


typedef struct {
    int row;
    int col;
} Cell;

unsigned char **alloc_life(int rows, int cols);

void free_life(unsigned char ***plife, int rows, int cols);


// set all cells of life with 0
void reset_life(unsigned char **life, int rows, int cols);

// fills 'value'  at cell
//     life - matrix of cells
//     rows - number of rows in life
//     cols - number of cols in life
//     cell - position of cell where value should be filled
//     value - value to be filled in that cell
void fill_cell_in_life(unsigned char **life, int rows, int cols,
                  Cell cell, unsigned char value);

unsigned char** init_life_from_file(unsigned char **life, int rows, int cols, char edge,
                        Cell cur, // initial position
                        const char *filename) ;
unsigned char** process_generation(unsigned char **lifeA, int rows, int cols,
                           unsigned char **lifeB, char edge);
int get_alive_nbrs(unsigned char **life, int rows, int cols, char edge, Cell cell);

Cell get_next_cell(int rows, int cols, char edge, Cell cur, Cell dist);
#endif
