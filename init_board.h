//
// File: init_board.h
// Description: Provides functions for initializing and shuffling a 2D array 
//              of chars with vacant ('.'), endline ('e'), and newline ('n')
//              chars. It uses the Fisher-Yates shuffle algorithm to randomize
//              the position of the chars in the board from lowest to highest
//              index.
//
// @author ldc1618: Luke Chelius
//
// // // // // // // // // // // // // // // // // // // // // // // // // //

// Header guard for init_board.h
#ifndef _INIT_BOARD_H_
#define _INIT_BOARD_H_

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <stdlib.h>
#include <time.h>


/**
 * populateBoard fills a 2D array of chars with the correct percentages of
 * vacant, endline, and newline chars from the supplied percentages given.
 *
 * @param dimensions  the size of the square 2D array given
 * @param board       a 2D array of size dimensions that will be filled with
 *                    vacant, endline, and newline chars in that order
 * @param vacant      the percentage of the spaces in board to be filled
 *                    with vacant chars
 * @param endline     the percentage of the remaining spaces in board to be
 *                    filled with endline chars
 */
void populateBoard(int dimensions, char board[dimensions][dimensions],
                   int vacant, int endline);


/**
 * shuffle uses the Fisher-Yates shuffle algorithm to randomly swap the
 * indices of a 2D array from lowest to highest index. The Fisher-Yates
 * algorithm operated on a single array, so in order to use a 2D array
 * the index needs to be divided by 5 to get the row, and modulus 5 for the
 * column.
 *
 * @param dimensions  the size of the square 2D array given
 * @param board       a 2D array of chars that will have its values randomly
 *                    swapped by the algorithm
 */
void shuffle(int dimensions, char board[dimensions][dimensions]);


// End header guard
#endif
