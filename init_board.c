//
// File: init_board.c
// Description: Contains a function used to initialize a 2D array of chars
// with the correct number of each type of agent and vacant spaces, and a
// function to randomly shuffle the agents around in the array from lowest
// to highest index.
//
// @author ldc1618: Luke Chelius
//
// // // // // // // // // // // // // // // // // // // // // // // // // //


//#define _DEFAULT_SOURCE
//#include <stdlib.h>
//#include <time.h>


#include "init_board.h"


/**
 * populateBoard(): Initialize the indices of a 2D array with the appropriate
 * number of vacant, endline, and newline chars based on given percentages
 * of the board to be filled with each type of char.
 */
void populateBoard(int dimensions, char board[dimensions][dimensions],
                   int vacant, int endline) {

    // Calculate total spaces in the board 2D array
    int totalSpaces = dimensions * dimensions;
    // Calculate the number of vacant, '.', spots from the percentage given
    int numVacant = totalSpaces * (vacant / 100.0);
    // Calculate the number of endline, 'e', spots from percent given
    int numEndline = (totalSpaces - numVacant) * (endline / 100.0);

    // Nested loops to initialize the board with the correct number of each
    // char, '.' for vacant, 'e' for endline, and 'n' for newline
    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {

            // Put correct number of vacant chars
            if (numVacant > 0) {
                board[i][j] = '.';
                numVacant--;
            }
            // Put correct number of endline chars
            else if (numEndline > 0) {
                board[i][j] = 'e';
                numEndline--;
            }
            // Fill the rest with newline chars
            else {
                board[i][j] = 'n';
            }
        }
    }
}


/**
 * shuffle(): Uses the Fisher-Yates shuffle algorithm to randomize the contents
 * of a 2D array from low to high index.
 */
void shuffle(int dimensions, char board[dimensions][dimensions]) {

    srandom(time(NULL));  // Uses time(NULL) for dynamic randomization

    // For loop to go through each index in the 2D array
    for (int i = 0; i < dimensions * dimensions - 2; i++) {

        // Get a random number and modulo it by the size of the array, adding
        // the current index to swap it with a higher index in the array
        long int swapIndex = random() % (dimensions * dimensions - i) + i;

        // Temporary variable to hold the current index value
        char temp = board[i / dimensions][i % dimensions];

        // Swap the random index with the current index
        board[i / dimensions][i % dimensions] =
                board[swapIndex / dimensions][swapIndex % dimensions];

        // Set the random index to temp which holds the current index's old
        // value
        board[swapIndex / dimensions][swapIndex % dimensions] = temp;
    }
}
