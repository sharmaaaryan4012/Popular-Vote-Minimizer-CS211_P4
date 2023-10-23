# Popular Vote Minimizer Project

## Overview
This repository hosts a C application designed to analyze U.S. presidential election data, focusing on minimizing popular votes while still achieving sufficient electoral votes to win an election. The project involves various tasks including handling command-line arguments, file I/O, data parsing, and implementing a brute-force algorithm to solve the central question of the project.

## Structure
The project consists of several C files, primarily `app.c`, `MinPopVote.c`, and `test.c`, alongside a Makefile for easy compilation and testing.

### Main Components
1. **app.c**: The main application file.
2. **MinPopVote.c**: Contains core functionalities and algorithms.
3. **test.c**: Used for writing and running tests.
4. **Makefile**: Specifies how to compile and run the application and tests.

### Folders
- **data/**: Contains input election data files in CSV format.
- **toWin/**: Stores output files with results.

## Compilation and Usage

### Makefile Targets
- `make build`: Compiles `app.c` and `MinPopVote.c` to build `app.exe`.
- `make run`: Runs `app.exe` using default values for command-line arguments.
- `make run_quiet`: Runs `app.exe` in quiet mode with default values for all other arguments.
- `make run_fast`: Runs `app.exe` in fast mode with default values for all other arguments.
- `make valgrind`: Runs `app.exe` under valgrind to check for memory leaks.
- `make build_test`: Compiles `test.c` and `MinPopVote.c` to build `test.exe`.
- `make run_test`: Executes the `test.exe` testing suite.

### Adding Additional Run Targets
You can extend the Makefile to include additional run targets with different combinations of command-line arguments.

## Implementation Details

### File Handling and Data Parsing
- **outFilename()**: Generates output file names based on the election year.
- **parseLine()**: Parses a single line of election data and updates the State struct.
- **readElectionData()**: Reads election data from a file and fills an array of State structs.

### Algorithmic Functions
- **totalEVs()**: Returns the total number of electoral votes in an array of States.
- **totalPVs()**: Returns the total number of popular votes in an array of States.
- **minPopVoteAtLeast()**: Implements a brute-force recursive algorithm to find the subset of states with the minimum popular votes required to win the election.

### Testing
Develop your own test cases in `test.c` to thoroughly test each function's functionality and correctness. Make use of `make build_test` and `make run_test` to compile and run your tests, respectively.

## Testing and Validation
Always ensure that your code is well-tested before relying on the autograded test cases or deploying your application. You are encouraged to write comprehensive test cases in `test.c` for all functions implemented in the project.
