//
// File: play_game.c
// Description: Contains functions that advance the 2D array of chars to the 
// next cycle by computing the happiness of each char, moving them if 
// necessary, and computing the average happiness of the board.
//
// @author ldc1618: Luke Chelius
//
// // // // // // // // // // // // // // // // // // // // // // // // // //


#include "play_game.h"


/**
 * getHappiness(): Finds the happiness rating for a specific char by comparing
 * it to the 8 possible surrounding chars and counting how many are valid and
 * how many of them are the same type.
 */
double getHappiness(int dimensions, char board[dimensions][dimensions],
                 int row, int col) {

    char current = board[row][col];
    int sameNeighbors = 0;
    int totalNeighbors = 0;

    // Checks row above if possible
    if (row > 0 && board[row - 1][col] != '.') {
        totalNeighbors++;
        if (board[row - 1][col] == current) {
            sameNeighbors++;
        }
    }

    // Checks previous column if possible
    if (col > 0 && board[row][col - 1] != '.') {
        totalNeighbors++;
        if (board[row][col - 1] == current) {
            sameNeighbors++;
        }
    }

    // Checks row below if possible
    if (row < dimensions - 1 && board[row + 1][col] != '.') {
        totalNeighbors++;
        if (board[row + 1][col] == current) {
            sameNeighbors++;
        }
    }

    // Checks next column if possible
    if (col < dimensions - 1 && board[row][col + 1] != '.') {
        totalNeighbors++;
        if (board[row][col + 1] == current) {
            sameNeighbors++;
        }
    }

    // Checks upper left and right if possible
    if (row > 0) {
        if (col > 0 && board[row - 1][col - 1] != '.') {
            totalNeighbors++;
            if (board[row - 1][col - 1] == current) {
                sameNeighbors++;
            }
        }
        if (col < dimensions - 1 && board[row - 1][col + 1] != '.') {
            totalNeighbors++;
            if (board[row - 1][col + 1] == current) {
                sameNeighbors++;
            }
        }
    }

    // Checks lower left and right if possible
    if (row < dimensions - 1) {
        if (col > 0 && board[row + 1][col - 1] != '.') {
            totalNeighbors++;
            if (board[row + 1][col - 1] == current) {
                sameNeighbors++;
            }
        }
        if (col < dimensions - 1 && board[row + 1][col + 1] != '.') {
            totalNeighbors++;
            if (board[row + 1][col + 1] == current) {
                sameNeighbors++;
            }
        }
    }

    // Computes and returns the happiness of the char
    double happiness = 100.0;
    if (totalNeighbors > 0) {
        happiness = (sameNeighbors / ((double)totalNeighbors)) * 100;
    }

    return happiness;
}


/**
 * moveAgent(): Moves an endline or newline char to the next vacant spot either
 * the first or last available spot based on if first is false (0) or true 
 * (non-zero).
 */
int moveAgent(int dimensions, char board[dimensions][dimensions],
              char tempBoard[dimensions][dimensions], int row,
              int col, int first) {

    // If first is true, look for the first available spot
    if (first) {

        // Nested for loops to look for '.' char in both board and tempBoard
        for (int i = 0; i < dimensions; i++) {
            for (int j = 0; j < dimensions; j++) {

                // If the index is empty in both boards, swap the current char
                // with the index found by the loops in board
                if (tempBoard[i][j] == '.' && board[i][j] == '.') {
                    board[i][j] = board[row][col];
                    board[row][col] = '.';
                    return 1;
                }
            }
        }
    }
    // Otherwise, look for the last available spot
    else {

        // Nested for loops to look for '.' char in both board and tempBoard
        for (int i = dimensions - 1; i >= 0; i--) {
            for (int j = dimensions - 1; j >= 0; j--) {

                // If the index is empty in both boards, swap the current char
                // with the index found by the loops in board
                if (tempBoard[i][j] == '.' && board[i][j] == '.') {
                    board[i][j] = board[row][col];
                    board[row][col] = '.';
                    return 1;
                }
            }
        }
    }

    return 0;
}


/**
 * gameMove(): Moves as many chars as possible that do not meet the happiness
 * threshold using moveAgent, and returns the number of moves that were made. 
 */
int gameMove(int dimensions, char board[dimensions][dimensions],
             int strengthThreshold) {

    char tempBoard[dimensions][dimensions];  // Array to hold copy of board

    int numMoves = 0;  // Set the number of moves to 0 to start

    // Make a deep copy of board in tempBoard
    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {
            tempBoard[i][j] = board[i][j];
        }
    }

    // Passed to moveAgent, will switch between 1 and 0 to make the function
    // find the first vacant space, then the last vacant space, then the first,
    // and so on
    int first = 0;

    // Nested loops go through each index in the array to find non-empty chars
    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {

            // If non-empty find its happiness
            if (tempBoard[i][j] != '.' && (tempBoard[i][j] == board[i][j])) {

                // Get the char's happiness
                int happiness = getHappiness(dimensions, tempBoard, i, j);

                // If the happiness is below the threshold, move the char to
                // a new, valid, empty spot
                if (happiness < strengthThreshold) {
                    int validMove = moveAgent(dimensions, board, tempBoard,
                                              i, j, first);

                    // Increment numMoves only if a move was made
                    if (validMove) {
                        numMoves++;  // Increment the number of moves

                        // Change first from 1 to 0 or vice versa to look for
                        // a vacant spot from the other end of the array next
                        // time an agent is moved
                        first = (first + 1) % 2;
                    }
                }
            }
        }
    }

    // Calculate and return the total number of moves
    return numMoves;
}


/**
 * getBoardHappiness(): Computes the average happiness for the entire board
 * by using getHappiness on each non-vacant char, summing all of them, and
 * dividing by the total number of non-vacant chars in the array.
 */
double getBoardHappiness(int dimensions, char board[dimensions][dimensions]) {

    double totalHappiness = 0;  // Will hold the grid's happiness level
    int numCounted = 0;  // Counts the number of non-empty chars in the grid

    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {

            if (board[i][j] != '.') {

                // Get the char's happiness and add it to the total
                totalHappiness += getHappiness(dimensions, board, i, j);
                numCounted++;  // Increment the number of valid chars
            }
        }
    }

    // Return the average happiness for the board
    return (totalHappiness / numCounted) / 100.0;
}

