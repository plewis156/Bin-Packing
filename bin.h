/***************************************************************
  Name: Paul Lewis
  File Name: bin.h

  Contains struct definitions, libraries, #defines, and function prototypes for bin.c
***************************************************************/

#ifndef _BIN_H
#define _BIN_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
/*
 * The maximum amount a bin can hold
 */
#define BINSIZE 100
/*
 * Structure for a bin
 *
 * @field int max, the maximum amount a bin can hold
 * @field int *binArray, the array to hold the items in the bin
 * @field int current, the sum of all items currently in bin
 * @field int remaining, the amount of space left in bin
 * @field int numItems, the number of items in the bin
 * @field struct bin *nextBin, pointer to next bin in linked list
 */
struct bin {
    int max;
    int *binArray;
    int current;
    int remaining;
    int numItems;
    struct bin *nextBin;
};
/*
 * Structure for a list of bins
 * 
 * @field struct bin *firstBin, pointer to first bin in linked list
 * @field int numBins, number of bins in list
 * @field int numFull, number of bins that are full
 * @field int *badItems, array contain illegal items found in list array
 * @field int numBad, number of bad items found
 * @field int sum, sum of all items in each bin
 * @field int optimal, boolean if all bins are full 
 */
struct binList {
    struct bin *firstBin;
    int numBins;
    int numFull;
    int *badItems;
    int numBad;
    int sum;
    int optimal;
};
/*
 * Function to create a new bin struct
 * 
 * @return struct bin *, pointer to new bin struct
 */ 
struct bin *newBin();
/*
 * Function to free a bin struct
 *
 * @param struct bin *b, the bin to free
 * 
 * @return struct bin *, pointer to freed bin struct
 */
struct bin *freeBin(struct bin *b);
/*
 * Function to create a new binList struct
 *
 * @param int runSize, size of run
 *
 * @return struct binList *, pointer to new binList struct
 */
struct binList *newBinList(int runSize);
/*
 * Function to free a binList struct
 * 
 * @param struct binList *bl, the binList struct to free
 * 
 * @return struct binList *, pointer to freed binList struct
 */
struct binList *freeBinList(struct binList *bl);

#endif
