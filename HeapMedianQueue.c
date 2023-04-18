#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "Heap.h"
#include "MedianQueue.h"

typedef struct{
    HeapReference* ref;
    bool type;
}ReferenceNode;

struct median_queue_t {
	size_t size;
	size_t start;
	Heap *max_heap;
	Heap *min_heap;
	ReferenceNode *circular;
};

static void swap_tops(MedianQueue* mq){
size_t index_top_min;
size_t index_top_max;
bool found_top_min=false;
bool found_top_max=false;

for(size_t i=0; i<mq->size; i++){
    if(mq->circular[i].type){
        if(heapGet(mq->max_heap,mq->circular[i].ref) == heapTop(mq->max_heap)){
            index_top_max = i;
            found_top_max = true;
            if(found_top_min)
                break;
        }
    }
    else{
         if (heapGet(mq->min_heap,mq->circular[i].ref) == heapTop(mq->min_heap)){
            index_top_min =i;
            found_top_min=true;
            if(found_top_max)
                break;
        }
    }

}
if (found_top_min && found_top_max){
    double top_max = heapTop(mq->max_heap);
    double top_min = heapTop(mq->min_heap);

    heapReplace(mq->min_heap,mq->circular[index_top_min].ref,top_max);
    heapReplace(mq->max_heap,mq->circular[index_top_max].ref,top_min);

    ReferenceNode tmp = mq->circular[index_top_min];
    mq->circular[index_top_min] = mq->circular[index_top_max];
    mq->circular[index_top_max] = tmp;

    }


}


MedianQueue* mqCreate(const double* values, size_t size) {
    MedianQueue* mq = malloc(sizeof(MedianQueue));
    if (!mq){
        free(mq);
        return NULL;
    }

	mq->start = 0;
    mq->size = size;
    mq->min_heap = heapCreate(false);
    mq->max_heap = heapCreate(true);

    if (!mq->min_heap || !mq->max_heap){
        free(mq->min_heap);
        free(mq->max_heap);
        free(mq);
        return NULL;
    }

    mq->circular = malloc(size*sizeof(ReferenceNode));

    if (!mq->circular)
    {
        heapFree(mq->min_heap);
        heapFree(mq->max_heap);
        free(mq->circular);
        free(mq);
    }

    bool done;

    for(size_t i=0;i<mq->size;i++)
    {
        mq->circular[i].ref= referenceArrayAlloc(1);
        if(!mq->circular[i].ref){
            while(i>0){
                free(mq->circular[i].ref);
                i--;
            }
            heapFree(mq->min_heap);
            heapFree(mq->max_heap);

            free(mq->circular);
            free(mq);
            return NULL;
        }

        if (i<size/2){
            done = heapAdd(mq->max_heap,values[i],mq->circular[i].ref);
            mq->circular[i].type = true;
        }
        //and here the MinHeap
        else
        {
            done = heapAdd(mq->min_heap,values[i],mq->circular[i].ref);
            mq->circular[i].type= false;
        }
        if (!done){
            while(i>0){
                free(mq->circular[i].ref);
                i--;
            }
            heapFree(mq->min_heap);
            heapFree(mq->max_heap);

            free(mq->circular);
            free(mq);
            return NULL;
        }

    }

    while(heapTop(mq->min_heap)<heapTop(mq->max_heap))
    {
    swap_tops(mq);
    }
    return mq;
}

void mqFree(MedianQueue* mq){
    for(size_t i = 0; i<mq->size;i++)
        free(mq->circular[i].ref);

	heapFree(mq->min_heap);
	heapFree(mq->max_heap);

    free(mq->circular);
	free(mq);
}

void mqUpdate(MedianQueue* mq, double value){
    if (mq->circular[mq->start].type){
        heapReplace(mq->max_heap,mq->circular[mq->start].ref,value);
    }
    else{
        heapReplace(mq->min_heap,mq->circular[mq->start].ref,value);
    }

    if (heapTop(mq->min_heap)<heapTop(mq->max_heap))
        swap_tops(mq);

    mq->start= (mq->start+1) %mq->size ;
}

double mqMedian(const MedianQueue* mq){
    if (!mq) { return INFINITY; }
	return heapTop(mq->min_heap);
}
