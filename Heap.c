#include <stdlib.h>
#include "Heap.h"

typedef struct{
double val;
HeapReference *ref;
}node;

struct heap_t{
size_t size;
size_t limit_size;
bool type;
unsigned int *indexes;
node *arr;
};

struct heap_ref_t{
unsigned int key;
};


static void down_Heapify(Heap* hp, unsigned int key){
    unsigned int l = (key*2)+1, r = (key*2)+2;
    unsigned int largest=key,smallest=key;
    size_t n = hp->size;
    bool t= hp->type;

    if(t)
    {
        if(l<n && hp->arr[l].val > hp->arr[key].val)
            largest = l;
        if(r<n && hp->arr[r].val  > hp->arr[largest].val)
            largest=r;

        if (largest != key)
        {
            node tmp_node = hp->arr[key];

            unsigned int index_largest = hp->arr[largest].ref->key;
            unsigned int index_node = hp->arr[key].ref->key;

            unsigned int tmp_key = hp->indexes[index_largest];

            hp->indexes[index_largest] = hp->indexes[index_node];
            hp->indexes[index_node] = tmp_key;

            hp->arr[key] = hp->arr[largest];
            hp->arr[largest] = tmp_node;

            down_Heapify(hp,largest);
        }
    }
    else
    {
        if (l<n && hp->arr[l].val < hp->arr[key].val)
            smallest = l;
        if (r<n && hp->arr[r].val < hp->arr[smallest].val)
            smallest = r;

        if (smallest != key){
            unsigned int index_smallest = hp->arr[smallest].ref->key;
            unsigned int index_node = hp->arr[key].ref->key;

            unsigned int tmp_key = hp->indexes[index_smallest];

            hp->indexes[index_smallest] = hp->indexes[index_node];
            hp->indexes[index_node] = tmp_key;

            node tmp_node = hp->arr[key];

            hp->arr[key] = hp->arr[smallest];
            hp->arr[smallest] = tmp_node;

            down_Heapify(hp,smallest);
        }
    }
}

static void up_Heapify(Heap* h, size_t place){
    if(place==0)
        return;
    bool t = h->type;
    size_t p = (place-1)/2;

    bool condi_max = t && h->arr[place].val > h->arr[p].val;
    bool condi_min =!t && h->arr[place].val < h->arr[p].val;

    if(condi_max || condi_min){

        unsigned int key_parent = h->arr[p].ref->key;
        unsigned int key_node = h->arr[place].ref->key;

        unsigned int k = h->indexes[key_parent];

        h->indexes[key_parent] = h->indexes[key_node];
        h->indexes[key_node] = k;

        node tmp = h->arr[p];

        h->arr[p] = h->arr[p];
        h->arr[place] = tmp;

        up_Heapify(h,p);
    }



}


Heap* heapCreate(bool maxHeap){
    Heap* h = malloc(sizeof(Heap));
    if(!h){
        free(h);
        return NULL;
    }
    h->type = maxHeap;
    h->size = 0;
    h->limit_size = 100;
    h->arr = malloc(h->limit_size*sizeof(node));
    h->indexes = malloc(h->limit_size*sizeof(unsigned int));

    if(!h->arr || !h->indexes){
        free(h->arr);
        free(h->indexes);
        free(h);
        return NULL;
    }

    return h;
}

size_t heapSize(const Heap* heap){
return heap->size;
}

double heapGet(const Heap* heap, const HeapReference* reference){

unsigned int x = heap->indexes[reference->key];
return heap->arr[x].val;
}

double heapTop(const Heap* heap){

return heap->arr[0].val;
}

HeapReference* heapTopReference(const Heap* heap){
if(heap->size<=0)
    return NULL;
else{
    HeapReference *top = malloc(sizeof(HeapReference));
    if(!top){
        free(top);
        return NULL;
    }
    double x = heap->indexes[0];
    size_t i = 0;
    while(x!=0){
        i++;
        x = heap->indexes[i];
    }
    top->key = i;
    return top;
}
}

bool heapAdd(Heap* heap, double value, HeapReference* dest){
if(!heap || !dest)
    return false;
if(heap->size == heap->limit_size){
    node *n = malloc(2*heap->limit_size*sizeof(node));
    unsigned int *ind = malloc(2*heap->limit_size*sizeof(unsigned int));
    if(!n || !ind){
        free(ind);
        free(n);
        return false;
    }
    for(size_t i=0; i<heap->size;i++){
        n[i].ref = heap->arr[i].ref;
        n[i].val = heap->arr[i].val;
        ind[i] = heap->indexes[i];
    }

    heap->indexes = ind;
    heap->arr = n;
    heap->limit_size *= 2;
}
    heap->arr[heap->size].val = value;
    heap->arr[heap->size].ref = dest;

    heap->indexes[heap->size] = heap->size;
    dest->key = heap->size;

    heap->size++;

    up_Heapify(heap,heap->size-1);

    return true;
}

void heapReplace(Heap* heap, HeapReference* toReplace, double value){

unsigned int new_key = heap->indexes[toReplace->key];
heap->arr[new_key].val=value;
down_Heapify(heap,new_key);
up_Heapify(heap,new_key);
}

void heapFree(Heap* heap){
free(heap->arr);
free(heap->indexes);
free(heap);
}


HeapReference* referenceArrayAlloc(size_t size){
HeapReference *arr = malloc(size * sizeof(HeapReference));
if(!arr){
    free(arr);
    return NULL;
}
return arr;
}

HeapReference* referenceArrayIndex(HeapReference* array, size_t n){
return array+n;
}

