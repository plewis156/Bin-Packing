/***************************************************************
  Name: Paul Lewis
  File Name: bin.c
  
  Contains functions for allocating and freeing bins and binLists
***************************************************************/

#include "bin.h"
/*
 * Function to create a new bin struct
 * 
 * @local struct bin *b, the new bin struct
 *
 * @return struct bin *, pointer to new bin struct
 */
struct bin *newBin() {
    struct bin *b;
    b = malloc (sizeof(struct bin));
    if(b == NULL) {
        perror("Error malloc bin\n");
        exit(1);
    }
    b->max = BINSIZE;
    b->binArray = malloc(BINSIZE * sizeof(int));
    if(b->binArray == NULL) {
        perror("Error malloc bin array\n");
        exit(1);
    }
    b->current = 0;
    b->remaining = BINSIZE;
    b->numItems = 0;

    return b;
}
/*
 * Function to create a new binList struct
 *
 * @param int runSize, size of run
 *
 * @local struct binList *bl, the new binList struct
 *
 * @return struct binList *, pointer to new binList struct
 */
struct binList *newBinList(int runSize) {
    struct binList *bl;
    bl = malloc(sizeof(struct binList));
    if(bl == NULL) {
        perror("Error malloc bin list\n");
        exit(1);
    }
    bl->badItems = malloc(runSize * sizeof(int));
    if(bl->badItems == NULL) {
        perror("Error malloc bl->badItems\n");
        exit(1);
    }
    bl->firstBin = newBin();
    bl->numBins = 1;
    bl->numFull = 0;
    bl->numBad = 0;
    bl->sum = 0;
    bl->optimal = 0;

    return bl;
}
/*
 * Function to free a bin struct
 *
 * @param struct bin *b, the bin to free
 * 
 * @return struct bin *, pointer to freed bin struct
 */
struct bin *freeBin(struct bin *b) {
    free(b->binArray);
    free(b);
    b = NULL;
    return b;
}
/*
 * Function to free a binList struct
 * 
 * @param struct binList *bl, the binList struct to free
 *
 * @local int i, counter
 * @local struct bin *temp1, temporary pointer to a bin
 * @local struct bin *temp2, temporary pointer to a bin
 * 
 * @return struct binList *, pointer to freed binList struct
 */
struct binList *freeBinList(struct binList *bl) {
    int i;
    struct bin *temp1, *temp2;
    temp1 = bl->firstBin;
    temp2 = temp1->nextBin;
    for(i=0;i<(bl->numBins-1);i++) {
        freeBin(temp1);
        temp1 = temp2;
        temp2 = temp2->nextBin;
    }
    free(temp1);
    free(bl->badItems);
    free(bl);
    bl = NULL;
    return bl;
}
