/*
Name: Aaryan Sharma
CS 211 (Fall 2023) - Prof Scott Reckinger
Project - 4 (Popular Vote Minimizer)
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "MinPopVote.h"

bool test_totalEVs() {
    State aStates[10];
    int res;
    
    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 10;

    printf(" Checking totalEVs() for 5 States:\n");
    res = totalEVs(aStates,5);
    if (res != 38) {
        printf("  individual state EVs are 5, 8, 13, 6, 7\n");
        printf("  expected total EVs = 38, actual total EVs = %d\n",res);
        return 0;
    }

    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 15;
    aStates[6].electoralVotes = 12;
    aStates[7].electoralVotes = 8;
    aStates[8].electoralVotes = 14;
    aStates[9].electoralVotes = 13;
    
    printf(" Checking totalEVs() for 10 States:\n");
    res = totalEVs(aStates,10);
    if (res != 100) {
        printf("  expected total EVs = 100, actual total EVs = %d\n",res);
        return false;
    }

    return true;
}


bool test_totalPVs() {                                                                                                  // This is a test case for the "totalPVs" function.
    State sampleStates[] = {
            {"Illinois", "IL", 20, 6033744},
            {"California", "CA", 55,17500881}
    };

    int totalPV = totalPVs(sampleStates, 2);

    return totalPV == 23534625;                                                                                         // Manually checking it against the addition of the 2 states' PV.
}


bool test_setSettings() {                                                                                               // This is a test case for the "setSettings" function.
    int year = 0;
    bool fastMode = 0;
    bool quietMode = 0;

    char* test1[] = {"./a.out", "-y", "2025"};                                                             // First test case, which tests the validity checking of "year".
    if (!setSettings(3, test1, &year, &fastMode, &quietMode) || year != 0) {
        return false;
    }
    year = 0;
    fastMode = 0;
    quietMode = 0;

    char* test2[] = {"./a.out", "-y", "2024", "-v"};                                                   // Testing for invalid arguments.
    if (!setSettings(2, test2, &year, &fastMode, &quietMode)) {
        return false;
    }
    year = 0;
    fastMode = 0;
    quietMode = 0;

    char* test3[] = {"./a.out", "-q", "-f"};                                                                // Testing for the ideal case.
    if (!setSettings(2, test3, &year, &fastMode, &quietMode) || !quietMode || fastMode) {
        return false;
    }
    return true;
}


bool test_inFilename() {                                                                                                // This is a test case for the "inFilename" function.
    char filename[100];
    inFilename(filename, 1832);

    return strcmp(filename, "data/1832.csv") == 0;                                                                      // Manually checking it against the correct "inFilename" string.
}


bool test_outFilename() {                                                                                               // This is a test case for the "outFilename" function.
    char filename[100];
    outFilename(filename, 1832);

    return strcmp(filename, "toWin/1832_win.csv") == 0;                                                                 // Manually checking it against the correct "outFilename" string.
}


bool test_parseLine() {                                                                                                 // This is a test case for the "parseLine" function.
    State sampleState;
    bool parsed = parseLine("California,CA,55,17500881", &sampleState);

    return parsed &&                                                                                                    // Test case is passed whenever all conditions are satisfied, i.e, each variable of the class was set properly.
            (strcmp(sampleState.name, "California") == 0) &&
           (strcmp(sampleState.postalCode, "CA") == 0) &&
           (sampleState.electoralVotes == 55) &&
           (sampleState.popularVotes == 17500881);
}


bool test_readElectionData() {                                                                                          // This function tests the overall functioning of the aforementioned functions.
    State sampleStates[] = {
            {"Alabama", "AL", 5, 18618},
            {"Connecticut", "CT", 8, 19378},
            {"Delaware", "DE", 3, 13944},
            {"Georgia", "GA", 9, 20004},
            {"Illinois", "IL", 3, 14222},
            {"Indiana", "IN", 5, 39210},
            {"Kentucky", "KY", 14, 70776},
            {"Louisiana", "LA", 5, 8687},
            {"Maine", "ME", 9, 34789},
            {"Maryland", "MD", 11, 45796},
            {"Massachusetts", "MA", 15, 39074},
            {"Mississippi", "MS", 3, 8344},
            {"Missouri", "MO", 3, 11654},
            {"New Hampshire", "NH", 8, 44035},
            {"New Jersey", "NJ", 8, 45570},
            {"New York", "NY", 36, 270975},
            {"North Carolina", "NC", 15, 51747},
            {"Ohio", "OH", 16, 131049},
            {"Pennsylvania", "PA", 28, 152220},
            {"Rhode Island", "RI", 4, 3580},
            {"South Carolina", "SC", 11, 51126},
            {"Tennessee", "TN", 11, 46533},
            {"Vermont", "VT", 7, 32833},
            {"Virginia", "VA", 24, 38924}
    };

    int nSample = 24;

    char filename[50];
    inFilename(filename, 1828);
    int nStates = 0;
    State toCheck[50];

    readElectionData(filename, toCheck, &nStates);

    if (nStates == nSample) {
        for (int i=0; i<nStates; i++) {
            if (strcmp(sampleStates[i].name, toCheck[i].name) != 0) {                                                   // If any element of the two arrays do not match, the function returns false, since both arrays are supposed to be equal.
                return false;
            }
        }
        return true;
    }

    return false;                                                                                                       // Returns false, if the number of states do not match.
}


bool test_minPVsSlow() {                                                                                                // This function tests the functionality of the "minPopVoteToWin" function.
    State smallStates[] = {
            {"StateA", "AA", 5, 60, },
            {"StateB", "BB", 2, 20},
            {"StateC", "CC", 8, 70},
            {"StateD", "DD", 3, 30}
    };

    MinInfo toCheck = minPopVoteToWin(smallStates, 4);                                                  // Calling the recursive function against a small set of states.

    if (toCheck.subsetPVs == 47) {                                                                                      // Returning "true"(test case passed). if the final value of "subsetPVs" of the returned object is correct.
        return true;
    }

    return false;
}


bool test_minPVsFast() {                                                                                                // This function tests the functionality of "minPopVoteToWinFast" function, against two sets of data (small & big).
    State smallStates[4] = {
            {"StateA", "AA", 5, 60, },
            {"StateB", "BB", 2, 20},
            {"StateC", "CC", 8, 70},
            {"StateD", "DD", 3, 30}
    };

    MinInfo toCheck = minPopVoteToWinFast(smallStates, 4);                                                          // Calling the recursive function against a small set of states.

    if (toCheck.subsetPVs != 47) {                                                                                      // Returning "false"(test case failed). if the final value of "subsetPVs" of the returned object is incorrect.
        return false;
    }

    State bigStates[] = {
            {"Alabama", "AL", 5, 18618},
            {"Connecticut", "CT", 8, 19378},
            {"Delaware", "DE", 3, 13944},
            {"Georgia", "GA", 9, 20004},
            {"Illinois", "IL", 3, 14222},
            {"Indiana", "IN", 5, 39210},
            {"Kentucky", "KY", 14, 70776},
            {"Louisiana", "LA", 5, 8687},
            {"Maine", "ME", 9, 34789},
            {"Maryland", "MD", 11, 45796},
            {"Massachusetts", "MA", 15, 39074},
            {"Mississippi", "MS", 3, 8344},
            {"Missouri", "MO", 3, 11654},
            {"New Hampshire", "NH", 8, 44035},
            {"New Jersey", "NJ", 8, 45570},
            {"New York", "NY", 36, 270975},
            {"North Carolina", "NC", 15, 51747},
            {"Ohio", "OH", 16, 131049},
            {"Pennsylvania", "PA", 28, 152220},
            {"Rhode Island", "RI", 4, 3580},
            {"South Carolina", "SC", 11, 51126},
            {"Tennessee", "TN", 11, 46533},
            {"Vermont", "VT", 7, 32833},
            {"Virginia", "VA", 24, 38924}
    };

    MinInfo toCheckBig =  minPopVoteToWinFast(bigStates, 24);                                           // Calling the "fast" recursive funcition against a bigger data set.

    if (toCheckBig.subsetPVs == 196802) {                                                                               // Returning "true"(test case passed). if the final value of "subsetPVs" of the returned object is correct.
        return true;
    }

    return false;
}


int main() {
    printf("Welcome to the Popular Vote Minimizer Testing Suite!\n\n");
    
    printf("Testing totalEVs()...\n"); 
    if (test_totalEVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing totalPVs()...\n"); 
    if (test_totalPVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing setSettings()...\n"); 
    if (test_setSettings()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing inFilename()...\n"); 
    if (test_inFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing outFilename()...\n"); 
    if (test_outFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing parseLine()...\n"); 
    if (test_parseLine()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing readElectionData()...\n"); 
    if (test_readElectionData()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing minPopVoteToWin()...\n"); 
    if (test_minPVsSlow()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWinFast()...\n"); 
    if (test_minPVsFast()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    return 0;
}
