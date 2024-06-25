// Christian Catalano, 1120832, Assignment 4, March 26th 2022
#include "Q2.h"

int main(int argc, char* argv[]) {
    char filename[1000];
    char* fileString = NULL;
    person** mArr = NULL;
    person** fArr = NULL;
    int arrLen = 0;

    // Prompt user for filename
    while(mArr == NULL || fArr == NULL) {
        printf("Please enter a filename: \n");
        scanf("%s", filename);

        // Read data in from filename
        readDataFromFile(&mArr, &fArr, &arrLen, filename);

        if( mArr == NULL || fArr == NULL ) {
            fprintf(stderr, "Could not open filename: %s\n\n", filename);
            return 0;
        }

        // Compute stable marriages from a table of male and female preferences
        rank** matchedPairs = computeStableMarriage(mArr, fArr, arrLen);

        // Print the final matched pairs
        printMatchedPairsAsTable(matchedPairs, arrLen);

        // Free all values
        freeTable(mArr, arrLen);
        freeTable(fArr, arrLen);
        freeMatchedPairs(matchedPairs, arrLen);
    }  

    return 0;
}

// Gets data from file and updates *mArr and *fArr with new tables. *arrLen is the length of the table, *arrLen x *arrLen.
void readDataFromFile(person*** mArr, person*** fArr, int* arrLen, char* filename) {
    FILE *fp;
    int len;

    fflush(stdin); // Empty out stdin just in case
    fp = fopen(filename, "r");

    // Error checking
    if(fp == NULL)
        return;

    // Determine the amt of lines in the file
    fscanf(fp, " %d ", &len);

    // Create male and female array, ingest male data
    int* prefList;
    *mArr = malloc(len * sizeof(person*));
    *fArr = malloc(len * sizeof(person*));
    for(int i = 0; i < len; i++) {
        (*mArr)[i] = malloc(sizeof(person));
        (*fArr)[i] = malloc(sizeof(person));
        
        (*mArr)[i]->currMate = -1;
        (*mArr)[i]->r = malloc(len * sizeof(int));
        prefList = (*mArr)[i]->r;
        for(int j = 0; j < len; j++) {
            
            fscanf(fp, " %d ", &(prefList[j]) ); // Read male data into array
        }
    }

    for(int i = 0; i < len; i++) {
        (*fArr)[i]->currMate = -1;
        (*fArr)[i]->r = malloc(len * sizeof(int));
        prefList = (*fArr)[i]->r;
        for(int j = 0; j < len; j++) {
            fscanf(fp, " %d ", &(prefList[j]) ); // Read female data into array
        }
    }

    *arrLen = len;
    fclose(fp);
}

// Uses the Gale-Shapley stable marriage algorithm to compute stable marriages
// Returns a list of matched pairs, must be freed by caller
rank** computeStableMarriage(person** mArr, person** fArr, int len) {
    rank** matchedPairs = malloc(len * sizeof(rank*));
    for(int i = 0; i < len; i++) {
        matchedPairs[i] = createRank(-1, -1); // Create new ranked pair to return
        matchedPairs[i]->mRank = i + 1;
    }


    int currMan = -1;
    int proposedWoman = -1;
    int proposedMan = -1;
    for(int i = 0; (i < (len * len)) && !allTaken(mArr, len) ; i++) { // Run for a maximum of len^2 iterations
        for(int j = 0; j < len; j++) { // Find the next free man
            if(!isTaken(mArr[j])) {
                proposedMan = j + 1;
                break;
            }
        }

        for(int j = 0; j < len; j++) { // Find next woman to propose to
            proposedWoman = (mArr[proposedMan - 1]->r)[j];
            if(isTaken(fArr[proposedWoman - 1])) { // Woman is taken
                currMan = fArr[proposedWoman - 1]->currMate;
                if(getRank(fArr[proposedWoman - 1], len, proposedMan) < getRank(fArr[proposedWoman - 1], len, currMan)) { // Find the rankings of both men, see if proposed is higher
                    updateRank(matchedPairs, currMan, proposedMan, proposedWoman); // Update rank to include this pair
                    fArr[proposedWoman - 1]->currMate = proposedMan;
                    mArr[proposedMan - 1]->currMate = proposedWoman;
                    mArr[currMan - 1]->currMate = -1;
                    break;
                }
                continue;
            }
            else { // Woman is not taken
                fArr[proposedWoman - 1]->currMate = proposedMan; // Update relationship status
                mArr[proposedMan - 1]->currMate = proposedWoman;
                updateRank(matchedPairs, -1, proposedMan, proposedWoman); // Update rank to include this pair
                break;
            }
        }
    }

    return matchedPairs;
}

// Create a new rank, initalize the values, and return it
// Must be freed by calling code
rank* createRank(int mRank, int fRank) {
    rank* newR = malloc(sizeof(rank));
    newR->mRank = mRank;
    newR->fRank = fRank;

    return newR;
}

// Updates the rankings, changes the fRank of oldM to -1, and the fRank of newM to the newF
// Only changes oldM if oldM != -1. Allows for it to not be updated if this is a new marriage and not overriding a previous marriage
void updateRank(rank** mp, int oldM, int newM, int newF) {
    if(mp != NULL) {
        if(oldM != -1)
            mp[oldM - 1]->fRank = -1;
        mp[newM - 1]->fRank = newF;
    }
}

// find the rank a given person f has associated with this target m. Returns either 1, 2, or 3
int getRank(person* f, int len, int m) {
    if(f != NULL) {
        for(int i = 0; i < len; i++) {
            if(f->r != NULL && (f->r)[i] == m) { 
                return i + 1;
            }
        }
    }
}

// Returns true if person p is taken and false if not
bool isTaken(person* p) {
    if(p != NULL)
        return p->currMate != -1;
}

// Checks if all people in the array are taken. If so returns true, otherwise false
bool allTaken(person** arr, int len) {
    if(arr != NULL) {
        for(int i = 0; i < len; i++) {
            if(!isTaken(arr[i])) {
                return false;
            }
        }
    }

    return true;
}

// Prints either a male or female table, for debugging
void printTable(person** arr, int len) {
    int* prefList;
    if(arr != NULL) {
        printf("-----\n");
        for(int i = 0; i < len; i++) {
            if(arr[i] != NULL) {
                prefList = arr[i]->r;
                for(int j = 0; j < len; j++) {
                    printf("%d ", prefList[j]);
                }
                printf(", currentMate = %d\n", arr[i]->currMate);
            }
        }
        printf("-----\n");
    }
}

// Prints a set of matched pairs computed from computeStableMarriage, for debugging
void printMatchedPairs(rank** mp, int len) {
    printf("+---------------------------------------Final Matches----------------------------------------+\n");
    printf("| # corresponds to person, i.e. (2,3) means the second male is matched with the third female |\n");
    printf("+--------------------------------------------------------------------------------------------+\n\n");
    if(mp != NULL) {
        for(int i = 0; i < len; i++) {
            printf("(%d,%d)  ", mp[i]->mRank, mp[i]->fRank);
        }
        printf("\n\n");
    }
}

// Prints a set of matched pairs computed from computeStableMarriage in a table format, for final output
void printMatchedPairsAsTable(rank** mp, int len) {
    printf("+--------------------Final Matches--------------------+\n");
    printf("| 1 corresponds to a match, 0 corresponds to no match |\n");
    printf("+-----------------------------------------------------+\n\n");
    if(mp != NULL) {
        for(int i = 0; i < len; i++) {
            for(int j = 1; j <= len; j++) {
                printf("%d ", mp[i]->fRank == j);
            }
            printf("\n");
        }
        printf("\n");
    }
}

// Frees the male and female tables
void freeTable(person** arr, int len) {
    if(arr != NULL) {
        for(int i = 0; i < len; i++) {
            if(arr[i] != NULL) {
                free(arr[i]->r);
                free(arr[i]);
            }
        }
        free(arr);
    }
}

// Frees the set of matched pairs associated with the final stable marriages
void freeMatchedPairs(rank** mp, int len) {
    if(mp != NULL) {
        for(int i = 0; i < len; i++) {
            free(mp[i]);
        }
        free(mp);
    }
}
