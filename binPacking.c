/***************************************************************
  Name: Paul Lewis
  File Name: binPacking.c

  A program to pack items into bins
***************************************************************/

#include "binPacking.h"

int main() {
    
    struct itemStruct *theItems;  // struct containing items and data about items  

    theItems = getItems();
    preProcess(theItems);
    runSimulation(theItems);    
    return 0;
}
/*
 * A function to get items from items.txt
 *
 * @local FILE *fp, the file pointer
 * @local struct itemStruct *a, pointer to new itemStruct struct
 * @local int temp1, temporary variable
 * @local int i, j; counters
 * @char line[BINSIZE], buffer to hold string
 *
 * @return struct itemStruct *, returns an itemStruct struct
 */
struct itemStruct *getItems() {
    FILE *fp;
    struct itemStruct *a;
    a = malloc(sizeof(struct itemStruct));
    if(a == NULL) {
        perror("Error malloc item struct\n");
        exit(1);
    }
    int temp1, i, j;
    char line[BINSIZE];
    fp = fopen("items.txt", "r");
    if(fp == NULL) {
        perror("Error opening items.txt\n");
        exit(1);
    }
    fgets(line,sizeof(line),fp);    // get number of runs
    a->numRuns = atoi(line);
    bzero(line,sizeof(line));
    a->runArray = malloc(a->numRuns * sizeof(int));
    if(a->runArray == NULL) {
        perror("Error malloc a->runArray\n");
        exit(1);
    }
    a->itemArray = malloc(a->numRuns * sizeof(int *));
    if(a->itemArray == NULL) {
        perror("Error malloc a->itemArray\n");
        exit(1);
    }
    for(i=0;i<(a->numRuns);i++) {   // get items for each run and size of each run
        fgets(line,sizeof(line),fp);
        a->runArray[i] = atoi(line);
        bzero(line,sizeof(line));       
        a->itemArray[i] = malloc(a->runArray[i] * sizeof(int));
        if(a->itemArray[i] == NULL) {
            perror("Error malloc a->itemArray[i]\n");
            exit(1);
        }
        for(j=0;j<(a->runArray[i]);j++) {
            fgets(line,sizeof(line), fp);
            temp1 = atoi(line);
            bzero(line,sizeof(line));
            a->itemArray[i][j] = temp1;
        }
    }
    fclose(fp);
    return a;    
}
/*
 * A function to process the items for offline algorithm
 *
 * @local int i, j; counters
 *
 * @param struct itemStruct *s, the item struct
 */
void preProcess(struct itemStruct *s) {
    int i, j;
    s->processedArray = malloc(s->numRuns * sizeof(int *));
    if(s->processedArray == NULL) {
        perror("Error malloc s->processedArray\n");
        exit(1);
    }
    for(i=0;i<(s->numRuns);i++) {
        s->processedArray[i] = malloc(s->runArray[i] * sizeof(int));
        if(s->processedArray[i] == NULL) {
            perror("Error malloc s->processedArray[i]\n");
            exit(1);
        }
    }
    
    for(i=0;i<(s->numRuns);i++) {   // copy items into processedArray
        for(j=0;j<(s->runArray[i]);j++)
            s->processedArray[i][j] = s->itemArray[i][j];
    }
    
    for(i=0;i<(s->numRuns);i++) {   // sort items
        quickSort(s->processedArray[i], s->runArray[i]);
    }
}
/*
 * Quicksort function to sort in descending order
 *
 * @param int *array, the array to sort
 * @param int n, size of array
 * 
 * @local int pivot, the pivot
 * @local int temp, temporary variable
 * @local int *left, pointer to left side of array
 * @local int *right, pointer to right side of array
 */
void quickSort(int *array, int n) {
    int pivot, temp;
    int *left, *right;
    if(n < 2)
        return;
    pivot = array[n/2];
    left = array;
    right = array+n-1;
    while(left <= right) {
        if(*left > pivot) {
            left++;
        } else if(*right < pivot) {
            right--;
        } else {
            temp = *left;
            *left++ = *right;
            *right-- = temp;
        }
    }
    quickSort(array, right-array+1);
    quickSort(left, array+n-left);
}
/*
 * A function to allocate a new run struct
 *
 * @local struct run *r, the new run struct
 *
 * @return struct run *, the new run struct
 */
struct run *newRun() {
    struct run *r = malloc (sizeof(struct run));
    if(r == NULL) {
        perror("Error malloc struct run\n");
        exit(1);
    }
    r->listArray = malloc(NUMPOLICIES * sizeof(struct binList *));
    if(r->listArray == NULL) {
        perror("Error malloc r->listArray\n");
        exit(1);
    }
    r->optimal = 0;
    return r;
}
/*
 * A function to free a run struct
 *
 * @param struct run *r, the run struct to free
 *
 * @local int i, counter
 *
 * @return struct run *, the freed run struct
 */
struct run *freeRun(struct run *r) {
    int i;
    for(i=0;i<NUMPOLICIES;i++) {
        freeBinList(r->listArray[i]);
    }  
    free(r->listArray);
    free(r);
    r = NULL;
    return r;
}
/*
 * A function to perform a run with online and offline versions of each algorithm
 *
 * @param int *array, the array of items to pack in bins
 * @param int *procArray, the processed array for offline algorithm
 * @param int n, the number of items to pack
 *
 * @local int i, j; counters
 * @local struct run *r, the new run struct 
 *
 * @return struct run *, the run struct
 */
struct run *doRun(int *array, int *procArray, int n) {
    int i = 0, j;    
    struct run *r = newRun();
    r->listArray[i++] = firstFit(array,n);      // online first fit
    r->listArray[i++] = nextFit(array,n);       // online next fit
    r->listArray[i++] = bestFit(array,n);       // online best fit
    r->listArray[i++] = firstFit(procArray,n);  // offline first fit
    r->listArray[i++] = bestFit(procArray,n);   // offline best fit
    for(j=0;j<i;j++) {                          // check if run created optimal packing
        if(r->listArray[j]->optimal == 1)
            r->optimal = 1;
    }
    return r;
}
/*
 * A function to call other functions to run full simulation
 *
 * @local int i, counter
 * @local struct run **runs, and array of runs
 *
 * @param struct itemStruct *s, the item struct
 */
void runSimulation(struct itemStruct *s) {
    int i;
    struct run **runs = malloc(s->numRuns * sizeof(struct run *));
    for(i=0;i<(s->numRuns);i++)     // perform all runs
        runs[i] = doRun(s->itemArray[i], s->processedArray[i], s->runArray[i]);
    printTables(s,runs);
    for(i=0;i<(s->numRuns);i++)     // free everything
        freeRun(runs[i]);
    free(runs);
    freeItemStruct(s);   
}
/*
 * A function to print data for tables to console
 *
 * @param struct itemStruct *s, the item struct
 * @param struct run **runs, an array of runs
 *
 * @local int i, j; counters
 */
void printTables(struct itemStruct *s, struct run **runs) {
    int i, j;
    for(i=0;i<(s->numRuns);i++) {
        j = 0;        
        printf("\n");
        printf("|----------------------------------------|\n");
        printf("| RUN %d\n", i+1);
        printf("|----------------------------------------|\n");
        printf("|\tPolicy\t\tTotal Bins Used\n");
        printf("| All Bins Full?\t\t");
        if(runs[i]->optimal == 1)
            printf("yes\n");
        else
            printf("no\n");
        printf("| Online Algorithm\n");
        printf("|\tFirst Fit\t\t%d\n", runs[i]->listArray[j++]->numBins);
        printf("|\tNext Fit\t\t%d\n", runs[i]->listArray[j++]->numBins);
        printf("|\tBest Fit\t\t%d\n", runs[i]->listArray[j++]->numBins);
        printf("| Offline Algorithm\n");
        printf("|\tFirst Fit\t\t%d\n", runs[i]->listArray[j++]->numBins);
        printf("|\tBest Fit\t\t%d\n", runs[i]->listArray[j++]->numBins);
        printf("|----------------------------------------|\n");
        printf("\n");
        printf("Items : ");
        for(j=0;j<(s->runArray[i]);j++) {
            printf("%d ", s->itemArray[i][j]);
        }
        printf("\n");
        printf("Post-processed Items : ");
        for(j=0;j<(s->runArray[i]);j++) {
            printf("%d ", s->processedArray[i][j]);
        }
        printf("\n\n");
        printPolicies(runs,i);
        if((runs[i]->listArray[0]->numBad) > 0) {
            printf("Bad Items Not Used: ");                 // print bad items which weren't used, if any
            for(j=0;j<(runs[i]->listArray[0]->numBad);j++)
                printf("%d ", runs[i]->listArray[0]->badItems[j]);
            printf("\n");
        }
    }
}
/*
 * A function to print the contents of the bins for each policy
 *
 * @param struct run **runs, an array of runs
 * @param int i, the current run to print
 * 
 * @local int j, k, h; counters
 * @local struct bin *temp, temporary pointer to a bin
 */
void printPolicies(struct run **runs, int i) {
    
    int j, k, h;
    struct bin *temp;
    for(j=0;j<NUMPOLICIES;j++) {
        temp = runs[i]->listArray[j]->firstBin;
        printPolicyName(j);
        for(k=0;k<(runs[i]->listArray[j]->numBins);k++) {
            printf("Bin %d : ", k+1);
            for(h=0;h<(temp->numItems);h++)
                printf("%d ", temp->binArray[h]);
            printf("\n");
            temp = temp->nextBin;
        }
        printf("\n");
    }
}
/*
 * A helper function to print the name of a given policy
 *
 * @param int x, the current policy
 */
void printPolicyName(int x) {
    
    switch(x) {
        case 0: printf("Online First Fit : ");
                break;
        case 1: printf("Online Next Fit : ");
                break;
        case 2: printf("Online Best Fit : ");
                break;
        case 3: printf("Offline First Fit : ");
                break;
        case 4: printf("Offline Best Fit : ");
                break;
        default: printf("Error switch statement");
                break;
    }
    printf("\n");
}
/*
 * First fit algorithm
 *
 * @param int *array, the array of items to place in bins
 * @param in n, the number of items to place
 *
 * @local int i, counter
 * @local struct binList *bl, the new binList struct
 * @local struct bin *temp, temporary pointer to a bin
 * @local struct bin *last, pointer to last bin
 *
 * @return struct binList *, the bin list struct containing linked list of bins
 */
struct binList *firstFit(int *array, int n) {
    int i;
    struct binList *bl = newBinList(n);
    struct bin *temp, *last;
    last = bl->firstBin;
    for(i=0;i<n;i++) {
        temp = bl->firstBin;
        if(array[i] >= 0 && array[i] <= BINSIZE) {
            bl->sum += array[i];
            while(temp->remaining < array[i] && temp != last)   // scan list of bins starting from first
                temp = temp->nextBin;                           // to find first one that fits
            if(temp->remaining >= array[i]) {                   // if one has room in bin, place
                temp->binArray[temp->numItems] = array[i];
                temp->numItems++;
                temp->current += array[i];
                temp->remaining = temp->max - temp->current;
            } else {                                            // if one does not have room, create a new bin
                temp->nextBin = newBin();                       // and place item
                last = temp->nextBin;
                bl->numBins++;
                last->binArray[last->numItems] = array[i];
                last->numItems++;
                last->current += array[i];
                last->remaining = last->max - last->current;           
            }
        } else {
            bl->badItems[bl->numBad] = array[i];        // record bad items
            bl->numBad++;
        }
    }
    if(bl->sum == bl->numBins*BINSIZE)      // check if optimal (all bins are full)
       bl->optimal = 1;
    return bl;   
}
/*
 * Next fit algorithm
 *
 * @param int *array, the array of items to place in bins
 * @param in n, the number of items to place
 *
 * @local int i, counter
 * @local struct binList *bl, the new binList struct
 * @local struct bin *temp, temporary pointer to a bin
 *
 * @return struct binList *, the bin list struct containing linked list of bins
 */
struct binList *nextFit(int *array, int n) {
    int i;
    struct binList *bl = newBinList(n);
    struct bin *temp;
    temp = bl->firstBin;
    for(i=0;i<n;i++) {
        if(array[i] >= 0 && array[i] <= BINSIZE) {
            bl->sum += array[i];
            if(temp->remaining >= array[i]) {               // if current bin has room, place item in bin
                temp->binArray[temp->numItems] = array[i];
                temp->numItems++;
                temp->current += array[i];
                temp->remaining = temp->max - temp->current;
            } else {                                        // if current bin does not have room, make new bin and place item
                temp->nextBin = newBin();
                temp = temp->nextBin;
                bl->numBins++;
                temp->binArray[temp->numItems] = array[i];
                temp->numItems++;
                temp->current += array[i];
                temp->remaining = (temp->max - temp->current);           
            }
        } else {        // record bad items not used
            bl->badItems[bl->numBad] = array[i];
            bl->numBad++;
        }
    }
    if(bl->sum == bl->numBins*BINSIZE)  // check if all bins are full
       bl->optimal = 1;
    return bl;
}
/*
 * Best fit algorithm
 *
 * @param int *array, the array of items to place in bins
 * @param in n, the number of items to place
 *
 * @local int i, counter
 * @local struct binList *bl, the new binList struct
 * @local struct bin *currentBin, temporary pointer to a bin
 * @local struct bin *last, pointer to last bin
 *
 * @return struct binList *, the bin list struct containing linked list of bins
 */
struct binList *bestFit(int *array, int n) {
    int i;
    struct binList *bl = newBinList(n);
    struct bin *last, *currentBin;
    last = bl->firstBin;
    for(i=0;i<n;i++) {
        if(array[i] >= 0 && array[i] <= BINSIZE) {
            bl->sum += array[i];
            currentBin = scanList(bl->firstBin, last, array[i]);    // scan list for bin with best fit
            if(currentBin->remaining >= array[i]) {                 // when found, place item in bin
                currentBin->binArray[currentBin->numItems] = array[i];
                currentBin->numItems++;
                currentBin->current += array[i];
                currentBin->remaining = currentBin->max - currentBin->current;
            } else {                                                // if item can fit in no bins, make new bin and place item
                last->nextBin = newBin();
                last = last->nextBin;
                bl->numBins++;
                last->binArray[last->numItems] = array[i];
                last->numItems++;
                last->current += array[i];
                last->remaining = (last->max - last->current);
            }
        } else {        // record bad items not used
            bl->badItems[bl->numBad] = array[i];
            bl->numBad++;
        }
    }
    if(bl->sum == bl->numBins*BINSIZE)  //check if all bins are full
       bl->optimal = 1;
    return bl;
}
/*
 * A function to scan a list of bins for best fit algorithm
 *
 * @param struct bin *f, the first bin in list
 * @param struct bin *l, the last bin in list
 * @param int x, the item to place
 *
 * @local int t1, temporary variable used to check viability of bin
 * @local int t2, temporary variable
 * @local struct bin *temp, a temporary pointer to a bin
 * @local struct bin *currentBin, the optimal bin
 *
 * @return struct bin *, the current bin
 */
struct bin *scanList(struct bin *f, struct bin *l, int x) {
    int t1 = BINSIZE;
    int t2;
    struct bin *temp, *currentBin;
    temp = f;
    currentBin = temp;
    while(temp != l) {      // check bins for best fit
        if(temp->remaining >= x) {
            t2 = temp->remaining - x;
            if(t2 < t1 && t2 >= 0) {
                t1 = t2;
                currentBin = temp;
            }
        }
        temp = temp->nextBin;
    }
    if(l->remaining >= x) { // check last bin
        t2 = l->remaining - x;
        if(t2 < t1) {
            t1 = t2;
            currentBin = temp;
        }
    }
    
    return currentBin;
}
/*
 * A function to free an itemStruct struct
 *
 * @param struct itemStruct *s, the itemStruct struct to free
 *
 * @local int i, counter 
 *
 * @return struct itemStruct *, the freed itemStruct struct
 */
struct itemStruct *freeItemStruct(struct itemStruct *s) {
    int i;
    for(i=0;i<(s->numRuns);i++) 
        free(s->itemArray[i]);
    free(s->itemArray);
    for(i=0;i<(s->numRuns);i++) 
        free(s->processedArray[i]);
    free(s->processedArray);
    free(s->runArray);
    free(s);
    s = NULL;
    return s;
       
}
