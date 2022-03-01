//
// File: play_game.h
// Description: Contains functions that will be used to change the state of 
// the 2D array of chars, by moving a char to a vacant spot. This is only done
// if the char's happiness is below a certain amount, which can be found using
// getHappiness. The individual happiness of each char is also used to find
// the average happiness of the board.
//
// @author ldc1618: Luke Chelius
//
// // // // // // // // // // // // // // // // // // // // // // // // // //


// Include guard for play_game.h
#ifndef _PLAY_GAME_H_
#define _PLAY_GAME_H_

/**
 * getHappiness compares a char at a specific row and column to its 8
 * surrounding neighbors to find the percentage of the neighboring
 * chars that are the same.
 *
 * @param dimensions  the size of the square 2D array given
 * @param board       a 2D array of chars
 * @param row         the specified row for the current char
 * @param col         the specified column for the current char
 * @returns           the percentage of non-vacant neighbors that are the
 *                    same type as the specified char
 */
double getHappiness(int dimensions, char board[dimensions][dimensions],
                    int row, int col);


/**
 * moveAgent moves a single char in the grid to the first or last vacant '.'
 * spot in the 2D array depending on the parameter 'first'.
 *
 * @param dimensions  the size of the square 2D array given
 * @param board       the new board that is being updated currently, used
 *                    to determine which open spaces have not been filled
 * @param tempBoard   the board from the previous cycle, used to determine
 *                    which spaces are open to move the char to
 * @param row         the specified row for the char to move
 * @param col         the specified colomn for the char to move
 * @param first       represents a boolean, 1 if the first vacant spot should
 *                    be found, 0 if the last vacnt spot should be found
 * @returns           1 if the char was successfully moved, 0 otherwise
 */
int moveAgent(int dimensions, char board[dimensions][dimensions],
              char tempBoard[dimensions][dimensions], int row,
              int col, int first);


/**
 * gameMove find the happiness rating for every non-empty char in the board,
 * and moves it to a new empty location if it is below the happiness threshold
 * if possible. It counts the total number of moves made.
 *
 * @param dimensions         the size of the square 2D array given
 * @param board              a 2D array of chars
 * @param strengthThreshold  the happiness value a char must be greater than
 *                           or equal to to stay in place
 * @returns                  the number of moves made this cycle
 */
int gameMove(int dimensions, char board[dimensions][dimensions],
             int strengthThreshold);


/**
 * getBoardHappiness calculates the happiness rating for the entire board which
 * is found by averaging the happiness ratings of all the endline and newline
 * chars in the board.
 *
 * @param dimensions  the size of the square 2D array given
 * @param board       a 2D array of chars
 * @returns           the average happiness of the endline and newline chars
 *                    in board
 */
double getBoardHappiness(int dimensions, char board[dimensions][dimensions]);


// End include guard
#endif
