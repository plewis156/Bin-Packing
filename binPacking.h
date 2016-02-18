/***************************************************************
  Name: Paul Lewis
  File Name: binPacking.h

  Contains struct definitions, libraries, #defines, and function prototypes for project4.c
***************************************************************/

#ifndef _BINPACKING_H
#define _BINPACKING_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include "bin.h"

/*
 * The number of policies
 */
#define NUMPOLICIES 5

/*
 * Structure for the items
 *
 * @field int **itemArray, contains arrays of items for each run
 * @field int **processedArray, contains arrays of items sorted in descending order
 * @field int *runArray, an array containing the number of items in each run
 * @field int numRuns, the number of runs
 */
struct itemStruct {
    int **itemArray;
    int **processedArray;
    int *runArray;
    int numRuns;
};
/*
 * Structure for a run
 *
 * @field struct binList **listArray, an array of bin lists structs
 * @field int optimal, boolean to show whether or not a run created a optimal solution
 */
struct run {
    struct binList **listArray;  
    int optimal;
};
/*
 * A function to get items from items.txt
 *
 * @return struct itemStruct *, returns an item struct
 */
struct itemStruct *getItems();
/*
 * A function to process the items for offline algorithm
 *
 * @param struct itemStruct *s, the item struct
 */
void preProcess(struct itemStruct *s);
/*
 * First fit algorithm
 *
 * @param int *array, the array of items to place in bins
 * @param in n, the number of items to place
 *
 * @return struct binList *, the bin list struct containing linked list of bins
 */
struct binList *firstFit(int *array, int n);
/*
 * Next fit algorithm
 *
 * @param int *array, the array of items to place in bins
 * @param in n, the number of items to place
 *
 * @return struct binList *, the bin list struct containing linked list of bins
 */
struct binList *nextFit(int *array, int n);
/*
 * Best fit algorithm
 *
 * @param int *array, the array of items to place in bins
 * @param in n, the number of items to place
 *
 * @return struct binList *, the bin list struct containing linked list of bins
 */
struct binList *bestFit(int *array, int n);
/*
 * A function to allocate a new run struct
 *
 * @return struct run *, the new run struct
 */
struct run *newRun();
/*
 * A function to free a run struct
 *
 * @param struct run *r, the run struct to free
 *
 * @return struct run *, the freed run struct
 */
struct run *freeRun(struct run *r);
/*
 * A function to free an itemStruct struct
 *
 * @param struct itemStruct *s, the itemStruct struct to free
 *
 * @return struct itemStruct *, the freed itemStruct struct
 */
struct itemStruct *freeItemStruct(struct itemStruct *s);
/*
 * A function to perform a run with online and offline versions of each algorithm
 *
 * @param int *array, the array of items to pack in bins
 * @param int *procArray, the processed array for offline algorithm
 * @param int n, the number of items to pack
 *
 * @return struct run *, the run struct
 */
struct run *doRun(int *array, int *procArray, int n);
/*
 * A function to call other functions to run full simulation
 *
 * @param struct itemStruct *s, the item struct
 */ 
void runSimulation(struct itemStruct *s);
/*
 * A function to scan a list of bins for best fit algorithm
 *
 * @param struct bin *f, the first bin in list
 * @param struct bin *l, the last bin in list
 * @param int x, the item to place
 *
 * @return struct bin *, the current bin
 */
struct bin *scanList(struct bin *f, struct bin *l, int x);
/*
 * A function to print data for tables to console
 *
 * @param struct itemStruct *s, the item struct
 * @param struct run **runs, an array of runs
 */
void printTables(struct itemStruct *s, struct run **runs);
/*
 * A function to print the contents of the bins for each policy
 *
 * @param struct run **runs, an array of runs
 * @param int i, the current run to print
 */
void printPolicies(struct run **runs, int i);
/*
 * A helper function to print the name of a given policy
 *
 * @param int x, the current policy
 */
void printPolicyName(int x);
/*
 * Quicksort function to sort in descending order
 *
 * @param int *array, the array to sort
 * @param int n, size of array
 */
void quickSort(int *array, int n);

#endif
