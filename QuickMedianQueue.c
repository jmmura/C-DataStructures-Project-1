#include <stdlib.h>
#include <math.h>


#include "MedianQueue.h"


struct median_queue_t {
	double* circular;
	double* sorted;
	size_t size;
	size_t start;
};

/* ------------------------------------------------------------------------- *
 * Double comparison function.
 * ------------------------------------------------------------------------- */
static int compareDouble(const void* d1, const void* d2);
static int compareDouble(const void* d1, const void* d2) {
	double diff = (*(double*) d2 - *(double*) d1);
	return diff > 0 ? -1 : (diff < 0 ? 1 : 0);
}

MedianQueue* mqCreate(const double* values, size_t size) {
	// create the median queue
	MedianQueue* mq = malloc(sizeof(MedianQueue));
	if (!mq)
		return NULL;

	mq->size = size;
	mq->start = 0;

	// allocate circular array
	mq->circular = malloc(sizeof(double) * mq->size);
	if (!mq->circular) {
		free(mq);
		return NULL;
	}

	// allocated sorted array
	mq->sorted = malloc(sizeof(double) * mq->size);
	if (!mq->circular) {
		free(mq->circular);
		free(mq);
		return NULL;
	}

	for (size_t i = 0; i < mq->size; ++i) {
		mq->circular[i] = values[i];
		mq->sorted[i] = values[i];
	}

	qsort(mq->sorted, mq->size, sizeof(double), compareDouble);

	return mq;
}

void mqFree(MedianQueue* mq) {
	free(mq->sorted);
	free(mq->circular);
	free(mq);
}

static void swap(double *a, double *b){
double k;
k=*a;
*a=*b;
*b=k;
}


static size_t Partition(double *A,size_t p, size_t r){
double pivot=A[r];
size_t i=p-1;
for(size_t j=p; j<r;j++){
    if(A[j]<=pivot){
        i++;
        swap(&A[i],&A[j]);

    }
}
swap(&A[i+1],&A[r]);
return i+1;
}

static void QuickSort(double *A,double p,double r){
if(p<r){
    int q=Partition(A,p,r);
    QuickSort(A,q+1,r);
}
}


void mqUpdate(MedianQueue* mq, double value) {
	if (!mq) { return; }
	size_t n = mq->size;
	mq->circular[mq->start] = value;
	mq->start = (mq->start + 1) % n;
	size_t i;
	for (i = 0; i < n; ++i) {
		mq->sorted[i] = mq->circular[(mq->start + i) % n];
	}
	size_t m = (n-1)/2;
    i=0;
    size_t q=n;
    while(q!=m && n-i>m){
        QuickSort(mq->sorted,0,n-1-i);
        i++;
    }
}

double mqMedian(const MedianQueue* mq) {
	if (!mq) { return INFINITY; }
	return mq->sorted[mq->size / 2];
}
