/*
Name: Aaryan Sharma
CS 211 (Fall 2023) - Prof Scott Reckinger
Project - 4 (Popular Vote Minimizer)
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "MinPopVote.h"


bool setSettings(int argc, char** argv, int* year, bool* fastMode, bool* quietMode) {                                   // This function is used to read all of the command-line prompts, in order to configure the program correctly.
    (*year) = 0;
    (*fastMode) = 0;
    (*quietMode) = 0;

    for (int i=1; i<argc; i++) {
        if (strcmp(argv[i], "-y") == 0) {                                                                               // This branch of code updates the value present inside the "year" parameter, if the command-line prompt is valid.
            if (atoi(argv[i+1]) % 4 == 0) {
                (*year) = atoi(argv[i+1]);
            }
            i++;
        }

        else if (strcmp(argv[i], "-f") == 0) {                                                                          // This branch of code updates the "fastMode" option for the code.
            (*fastMode) = 1;
        }

        else if (strcmp(argv[i], "-q") == 0) {                                                                          // This branch of code updates the "quietMode" option for the code.
            (*quietMode) = 1;
        }

        else {                                                                                                          // This branch returns false, if there is an invalid command line prompt.
            return false;
        }
    }

    return true;
}


void inFilename(char* filename, int year) {                                                                             // This function finds the input "filename" belonging to the "year" parameter.
    sprintf(filename, "data/%d.csv", year);
    return;
}


void outFilename(char* filename, int year) {                                                                            // This function gives the output "filename" belonging to the "year" parameter.
    sprintf(filename, "toWin/%d_win.csv", year);
    return;
}


bool parseLine(char* line, State* myState) {                                                                            // This function parses an input string, and adds the data to "myState".

    if (line[strlen(line)-1] == '\n') {                                                                                 // If the "line" string contains "\n" character at the very end, we replace it with "\0".
        line[strlen(line)-1] = '\0';
    }

    int count = sscanf(line, "%49[^,],%2[^,],%d,%d", myState->name, myState->postalCode, &myState->electoralVotes, &myState->popularVotes);

    if (count != 4) {                                                                                                   // Returns false whenever the line is invalid, else returns true.
        return false;
    }

    return true;
}


bool readElectionData(char* filename, State* allStates, int* nStates) {
    FILE* fPtr;
    fPtr = fopen(filename, "r");                                                                                        // Opening "filename".
    (*nStates) = 0;

    if (fPtr != NULL) {                                                                                                 // If the file exists, we read each line and parse it using the "parseLine" function.
        char line[100];
        while (fgets(line, 100, fPtr) != NULL) {
            parseLine(line, &allStates[(*nStates)]);
            (*nStates)++;
        }
        fclose(fPtr);
    }

    else {                                                                                                              // If the file wasn't read successfully, we return false, else true is returned.
        return false;
    }

    return true;
}


int totalEVs(State* states, int szStates) {
    int evTotal = 0;

    for (int i=0; i<szStates; i++) {                                                                                    // Traversing through the "states" array and adding up the total electoral votes.
        evTotal += states[i].electoralVotes;
    }

    return evTotal;
}


int totalPVs(State* states, int szStates) {
    int pvTotal = 0;

    for (int i=0; i<szStates; i++) {                                                                                    // Traversing through the "states" array and adding up the total popular votes.
        pvTotal += states[i].popularVotes;
    }

    return pvTotal;
}


MinInfo minPopVoteAtLeast(State* states, int szStates, int start, int EVs) {                                            // This recursive function traverses an array of states and decides on the best possible strategy to win the election.

    if (EVs <= 0) {                                                                                                     // (Base Case - 1) to exit recursion: requirement for additional EVs has been met.
        MinInfo res;
        res.subsetPVs = 0;
        res.szSomeStates = 0;
        res.sufficientEVs = true;                                                                                       // Sets "sufficientEVs" as true, since the basic limit has been exceeded.
        return res;
    }

    if (start == szStates) {                                                                                            // (Base Case - 2) to exit recursion: The subset is complete.
        MinInfo res;
        res.subsetPVs = 0;
        res.szSomeStates = 0;
        res.sufficientEVs = false;                                                                                      // Sets "sufficientEVs" as false, since there are no more states to possibly satisfy the basic limit.
        return res;
    }

    MinInfo include = minPopVoteAtLeast(states, szStates, start + 1, EVs - states[start].electoralVotes);               // Recursive call for the "minPopVoteAtLeast", which is used to find a state which satisfy the condition to win.
    MinInfo exclude = minPopVoteAtLeast(states, szStates, start + 1, EVs);                                              // Recursive call for the "minPopVoteAtLeast", which finds a state that does not satisfy the condition to win.

    include.someStates[include.szSomeStates] = states[start];
    include.szSomeStates += 1;
    include.subsetPVs += (states[start].popularVotes/2)+1;

    if ((exclude.subsetPVs < include.subsetPVs) && exclude.sufficientEVs) {                                             // If the excluded PVs is less than the included PVs, we return "exclude" object.
        return exclude;
    }

    else {
        return include;                                                                                                 // In case the PV that we included is the minimum, we return the "include" object.
    }
}


MinInfo minPopVoteToWin(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = (totEVs/2) + 1;
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}


MinInfo minPopVoteAtLeastFast(State* states, int szStates, int start, int EVs, MinInfo** memo) {                        // This recursive function traverses an array of states and decides on the best possible strategy to win the election. And employs the concept of "Memoization".

    if (EVs <= 0) {                                                                                                     // (Base Case - 1) to exit recursion: requirement for additional EVs has been met.
        MinInfo res;
        res.subsetPVs = 0;
        res.szSomeStates = 0;
        res.sufficientEVs = true;                                                                                       // Sets "sufficientEVs" as true, since the basic limit has been exceeded.
        return res;
    }

    if (start == szStates) {                                                                                            // (Base Case - 2) to exit recursion: The subset is complete.
        MinInfo res;
        res.subsetPVs = 0;
        res.szSomeStates = 0;
        res.sufficientEVs = false;                                                                                      // Sets "sufficientEVs" as false, since there are no more states to possibly satisfy the basic limit.
        return res;
    }

    if (memo[start][EVs].subsetPVs != -1) {                                                                              // If the value exists inside the "memo" array, we simnply return that element, instead of performing the calculations.
        return memo[start][EVs];
    }

    MinInfo include =
            minPopVoteAtLeastFast(states, szStates, start + 1, EVs - states[start].electoralVotes, memo);               // Recursive call for the "minPopVoteAtLeastFast", which is used to find a state which satisfy the condition to win.
    MinInfo exclude = minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);                                    // Recursive call for the "minPopVoteAtLeastFast", which finds a state that does not satisfy the condition to win.

    include.someStates[include.szSomeStates] = states[start];
    include.szSomeStates += 1;
    include.subsetPVs += (states[start].popularVotes/2)+1;

    if ((exclude.subsetPVs < include.subsetPVs) && exclude.sufficientEVs) {                                             // If the excluded PVs is less than the included PVs, we return "exclude" object.
        memo[start][EVs] = exclude;
        return exclude;
    }

    else {
        memo[start][EVs] = include;
        return include;                                                                                                 // In case the PV that we included is the minimum, we return the "include" object.
    }
}

MinInfo minPopVoteToWinFast(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = (totEVs/2) + 1; // required EVs to win election

    MinInfo** memo = (MinInfo**)malloc((szStates+1)*sizeof(MinInfo*));
    for (int i = 0; i < szStates+1; ++i) {
        memo[i] = (MinInfo*)malloc((reqEVs+1)*sizeof(MinInfo));
        for (int j = 0; j < reqEVs+1; ++j) {
            memo[i][j].subsetPVs = -1;
        }
    }
    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo);

    for (int i = 0; i < szStates+1; i++) {                                                                              // Traversing through each element and freeing the values stored inside "memo".
        free(memo[i]);
    }
    free(memo);                                                                                                         // Freeing "memo".

    return res;

}

bool writeSubsetData(char* filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin) {                              // This function writes the data present in the "MinInfo" object returned by the recursive functions.
    FILE *fPtr;
    fPtr = fopen(filenameW, "w");                                                                        // Opening the file in write - "w" mode.

    if (fPtr != NULL) {
        fprintf(fPtr, "%d,%d,%d,%d\n", totEVs, totPVs, wonEVs, toWin.subsetPVs);                                        // File header - totEVs, totPVs, wonEVs, toWin.subsetPVs.

        for (int i = toWin.szSomeStates - 1; i>=0; i--) {
            fprintf(fPtr, "%s,%s,%d,%d\n", (toWin.someStates[i]).name,
                    (toWin.someStates[i]).postalCode, (toWin.someStates[i]).electoralVotes, ((toWin.someStates[i]).popularVotes/2) + 1);
        }
        fclose(fPtr);                                                                                                   // Closing the file.
        return true;
    }

    return false;                                                                                                       // Function returns false, in case the file was not opened.
}
