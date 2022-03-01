//
// File: bracetopia.c
// Description: Takes input via commandline using getopt to process various
// user supplied flags as input for the program. Error messages are given if
// incorrect, incomplete, or invalid input is provided. Then either infinite
// or print mode is entered depending on the user input, infinite modes
// uses ncurses to output continuously until the user stops with Control-C.
// Print mode outputs a specified number of times and stops.
//
// The program simulates a city with a 2D array, that is filled with 'agents'
// that are either endline ('e') or newline ('n'), as well as vacant ('.') 
// chars. These agents want to be near other agents of the same type, so 
// they have a happiness rating which is the average number of surrounding
// cells that are the same agent as them. If this happiness os below a value
// then the agent moves to a vacant space. This is simulated in this program.
//
// @author ldc1618: Luke Chelius
//
// // // // // // // // // // // // // // // // // // // // // // // // // //


#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <getopt.h>
#include <time.h>

#include "init_board.h"
#include "play_game.h"


/**
 * printUsage prints the usage message for bracetopia.c to standard error.
 */
void printUsage(void) {
    fprintf(stderr, "usage:\n"
            "bracetopia [-h] [-t N] [-c N] [-d dim] [-s %%str]"
            " [-v %%vac] [-e %%end]\n");
}


/**
 * printModePrint prints a 2D array of chars as well as additional information
 * about the board like the cycle it displays, the number of moves made that
 * cycle, the overall happiness rating of the board, and the dimensions,
 * strength threshold, and vacancy and endline percentage for the board.
 *
 * @param dimensions         the size of the square 2D array given
 * @param board              a square 2D array of chars
 * @param cycle              the cycle number the board is on (0 initially)
 * @param moves              the number of moves made during the cycle the 
 *                           board shows
 * @param happiness          the average happiness of all chars in board
 * @param strengthThreshold  the value a chars happiness must be greater than
 *                           or equal to to stay in place
 * @param vacancy            the percentage of the board that is vacant
 * @param endline            the percentage of the remaining spots on the board
 *                           filled with endline agents
 */
void printModePrint(int dimensions, char board[dimensions][dimensions],
                    int cycle, int moves, double happiness,
                    int strengthThreshold, int vacancy, int endline) {
    
    // Nested loops to print the board
    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }

    // Print additional board info for print mode
    printf("cycle: %d\n", cycle);
    printf("moves this cycle: %d\n", moves);
    printf("teams\' \"happiness\": %lf\n", happiness);
    printf("dim: %d, %%strength of preference: %*d%%, %%vacancy: %*d%%, "
           "%%end: %*d%%\n", dimensions, 3, strengthThreshold, 3, vacancy,
           3, endline);
}


/**
 * infiniteModePrint prints a 2D array of chars along with additional
 * information about the board like the cycle being displayed, the number of
 * moves made this cycle, the average happiness of the board, and the
 * dimensions, strength threshold, and vacancy and endline percentages for the
 * board. This is done using curses rather than standard output.
 *
 * @param dimensions         the size of the square 2D array given
 * @param board              a 2D array of chars
 * @param cycle              the cycle number the board is on (0 initially)
 * @param moves              the number of moves made during the cycle board
 *                           currently contains
 * @param happiness          the average happiness of all chars in board
 * @param strengthThreshold  the value a char's happiness must be greater than
 *                           or equal to to stay in place
 * @param vacancy            the percentage of the board that is vacant
 * @param endline            the percentage of the remaining spots in the board
 *                           filled with endline agents
 */
void infiniteModePrint(int dimensions, char board[dimensions][dimensions],
                       int cycle, int moves, double happiness, 
                       int strengthThreshold, int vacancy, int endline) {
    
    move(0, 0);  // Move to begining of the window
    
    // Nested loops to print the board one char at a time
    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {
            
            printw("%c", board[i][j]);  // Prints a char
        }

        printw("\n");  // Move to new line after each row prints
    }

    // Print additional info for the board
    printw("cycle: %d\n", cycle);
    printw("moves this cycle: %d\n", moves);
    printw("teams\' \"happiness\": %lf\n", happiness);
    printw("dim: %d, %%strength of preference: %*d%%, %%vacancy: %*d%%, "
           "%%end: %*d%%\n", dimensions, 3, strengthThreshold, 3, vacancy,
           3, endline);
    printw("Use Control-C to quit.");

    refresh();  // Refresh screen to show new output
}


/**
 * main uses getopt to process flags from the commandline supplied by the user
 * in order to take possible input for the number of cycles for print mode,
 * the size of the 2D array board, the happiness level threshold, the
 * percentage of vacant spaces, the percentage of endline spaces, the sleep
 * time for infinite mode, or to display a help screen. It then populates
 * and shuffles the board and enters print mode or infinite mode depending
 * on the flags given from the commandline.
 *
 * @param argc  the total number of command line args given, at least 1 as it
 *              always includes the program name
 * @param argv  a 2D array of chars contianing the command line args given:
 *              program-name commandline-arg(s)
 * @returns     EXIT_SUCCESS if run successfully, EXIT_FAILURE if an invalid or
 *              incomplete argument is given, or (1 + EXIT_FAILURE) if an 
 *              incorrect argument is given
 */
int main(int argc, char *argv[]) {
    
    int opt;  // Variable used for getopt
    int temp;  // Temporary variable to get flag input
    int time = 900000;  // Default sleep time for infinite mode
    int infiniteMode = 1;  // Boolean, true as default is infinite mode
    int numCycles;  // Holds the number of cycles to do if in print mode
    int dimensions = 15;  // Default size for board
    int strengthThreshold = 50;  // Default happiness threshold
    int vacant = 20;  // Default vacancy percentage
    int endline = 60;  // Default endline agent percentage

    // While loop runs until no more commandline args are left
    while ((opt = getopt(argc, argv, "ht:c:d:s:v:e:")) != -1) {

        // Switch statement to process different flags from the commandline
        switch(opt) {
        
        // help flag displays usage message and descriptions of the flags
        case 'h':
            printUsage();
            fprintf(stderr, "Option      Default   Example   Description\n"
                    "'-h'        NA        -h        print this usage "
                    "message.\n"
                    "'-t N'      900000    -t 5000   microseconds cycle "
                    "delay.\n"
                    "'-c N'      NA        -c4       count cycle maximum "
                    "value.\n"
                    "'-d dim'    15        -d 7      width and height "
                    "dimension.\n"
                    "'-s %%str'   50        -s 30     strength of "
                    "preference.\n"
                    "'-v %%vac'   20        -v30      percent vacancies.\n"
                    "'-e %%endl'  60        -e75      percent Endline "
                    "braces. Others want Newline.\n");

            // Ends the program returning EXIT_SUCCESS
            return EXIT_SUCCESS;
        
        // Time flag, only accepted if it is greater than 0, otherwise the
        // default value is used, determines how long the program sleeps
        // in between infinite mode outputs
        case 't':
            temp = (int)strtol(optarg, NULL, 10);
            if (temp > 0) {
                time = temp;
            }
            break;
        
        // Count flag, accepted if greater than or equal to 0, sets program to
        // print mode if included and determines how many cycles the program
        // runs for
        case 'c':
            temp = (int)strtol(optarg, NULL, 10);
            if (temp >= 0) {
                infiniteMode = 0;
                numCycles = temp;
            }
            // Prints an error message if the flag has an invalid value, and
            // returns EXIT_FAILURE to end the program
            else {
                fprintf(stderr, "count (%d) must be a non-negative integer.\n",
                        temp);
                printUsage();
                return (1 +  EXIT_FAILURE);
            }
            break;
        
        // Dimension flag, accepted if between 5 and 39 (inclusive), determines
        // the size of the 2D array board
        case 'd':
            temp = (int)strtol(optarg, NULL, 10);
            if (temp >= 5 && temp <= 39) {
                dimensions = temp;
            }
            // Prints an error message if the flag's value isn't in the range,
            // and returns EXIT_FAILURE to end the program
            else {
                fprintf(stderr, "dimension (%d) must be a value in [5...39]\n",
                        temp);
                printUsage();
                return (1 + EXIT_FAILURE);
            }
            break;
        
        // Strength threshold flag, accepted if between 1 and 99 (inclusive),
        // determines the happiness rating a char must be below in order to
        // be moved
        case 's':
            temp = (int)strtol(optarg, NULL, 10);
            if (temp > 0 && temp < 100) {
                strengthThreshold = temp;
            }
            // Prints an error message if the flag's value isn't in the range,
            // and returns EXIT_FAILURE to end the program
            else {
                fprintf(stderr, "preference strength (%d) must be a value in "
                        "[1...99]\n", temp);
                printUsage();
                return (1 + EXIT_SUCCESS);
            }
            break;

        // Vacancy flap, accepted if between 1 and 99 (inclusive), determines 
        // the percentage of the board that will be filled with vacant '.' 
        // chars
        case 'v':
            temp = (int)strtol(optarg, NULL, 10);
            if (temp > 0 && temp < 100) {
                vacant = temp;
            }
            // Prints an error message if the flag's value isn't in the range,
            // and returns EXIT_FAILURE to end the program
            else {
                fprintf(stderr, "vacancy (%d) must be a value in [1...99]\n",
                        temp);
                printUsage();
                return (1 + EXIT_FAILURE);
            }
            break;

        // Endline flag, accepted if between 1 and 99 (inclusive), determines 
        // the percent of the remaining spots in the board, after the vacant
        // chars have found, that will be endline 'e' chars
        case 'e':
            temp = (int)strtol(optarg, NULL, 10);
            if (temp > 0 && temp < 100) {
                endline = temp;
            }
            // Prints an error message if the flag's value isn't in the range,
            // and returns EXIT_FAILURE to end the program
            else {
                fprintf(stderr, "endline proportion (%d) must be a value in "
                        "[1...99]\n", temp);
                printUsage();
                return (1 + EXIT_FAILURE);
            }
            break;

        // Default case runs if an invalid or incomplete flag is passed, prints
        // the usage and returns EXIT_FAILURE to end the program
        default:
            printUsage();
            return EXIT_FAILURE;
        }
    }
    
    char board[dimensions][dimensions];  // Create the board of size dimensions

    populateBoard(dimensions, board, vacant, endline);  // Fill board
    shuffle(dimensions, board);  // Shuffle the chars in board

    int cycle = 0;  // Variable to track the current cycle
    int moves = 0;  // Variable to track the number of moves made this cycle
    // Variable holds the board's happiness rating for this cycle
    double happiness = getBoardHappiness(dimensions, board);

    // If infinite mode is selected (-c flag is not used), enter infinite mode
    if (infiniteMode) {
        
        initscr();  // Begin curses mode by initializing the screen
        refresh();  // Refresh the screen

        // While loop to run infinitely until user stops with Control-C
        while (infiniteMode) {
            
            // Print out the board and info using curses
            infiniteModePrint(dimensions, board, cycle, moves, happiness,
                              strengthThreshold, vacant, endline);
            usleep(time);  // 'Sleep' for amount of time for readability

            cycle++;  // Increment cycle number
            // Generate the next cycle and store the number of moves made
            moves = gameMove(dimensions, board, strengthThreshold);
            // Get happiness of the next cycle
            happiness = getBoardHappiness(dimensions, board);
        }

        endwin();  // End curses mode at end of program
    }
    // Otherwise print mode runs (-c flag was included)
    else {

        // For loop runs specified number of times from -c flag
        for (cycle = 0; cycle <= numCycles; cycle++) {

            // Print the board and the info about it
            printModePrint(dimensions, board, cycle, moves, happiness,
                           strengthThreshold, vacant, endline);
            
            // Generate the next cycle and store the number of moves made
            moves = gameMove(dimensions, board, strengthThreshold);
            // Get the happiness of the next cycle
            happiness = getBoardHappiness(dimensions, board);
        }
    }

    // Return EXIT_SUCCESS if program runs successfully
    return EXIT_SUCCESS;
}
