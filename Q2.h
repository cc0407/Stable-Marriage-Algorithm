// Christian Catalano, 1120832, Assignment 4, March 26th 2022
#ifndef Q2_HEADER
#define Q2_HEADER
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct rankStruct {
    int mRank;
    int fRank;
} typedef rank;

struct personStruct {
    int* r;
    int currMate;
} typedef person;

/* Stable Marriage Functions */
void readDataFromFile(person*** mArr, person*** fArr, int* arrLen, char* filename);
rank** computeStableMarriage(person** mArr, person** fArr, int len);
void printMatchedPairsAsTable(rank** mp, int len);

/* Person Helper Functions */
bool isTaken(person* p);
bool allTaken(person** arr, int len);
void printTable(person** arr, int len);
void freeTable(person** arr, int len);

/* Rank Helper Functions */
rank* createRank(int mRank, int fRank);
void updateRank(rank** mp, int oldM, int newM, int newF);
int getRank(person* f, int len, int m);
void printMatchedPairs(rank** mp, int len);
void freeMatchedPairs(rank** mp, int len);




#endif /*Q2_HEADER */