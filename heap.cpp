
#include "heap.h"

#define DEFNUMELEMENTS 512
#define HEAPLEFT(i) (i*2)
#define HEAPRIGHT(i) (i*2+1)
#define HEAPPARENT(i) (i/2)
#define HEAPELEMENT(i) (pE[i-1])


static void HeapSwap( HeapElement *p, int i, int j )
{
    HeapElement t;
    memcpy((void *)&t, (void *)&p[i-1], sizeof(HeapElement));
    memcpy((void *)&p[i-1], (void *)&p[j-1], sizeof(HeapElement));
    memcpy((void *)&p[j-1], (void *)&t, sizeof(HeapElement));
}

HEAP::HEAP(void)
{
    nElements = 0;
    size = 0;
    pE = (HeapElement *)0;
    resize();
}

HEAP::~HEAP(void)
{
    if (pE) free(pE);
}

void HEAP::HeapifyUp(int i)
{
    while (i > 1)
    {
        int p = HEAPPARENT(i);
        int kp = HEAPELEMENT(p).key;
        int ki = HEAPELEMENT(i).key;
        
        if (kp <= ki)
        {
            // if parent <= than child (i), heap holds, done
            break;
        }
        else if (kp > ki)
        {
            // if parent greater than child, swap and continue checking upward
            HeapSwap(pE, p, i);
        }
        
        i = p;
    }
}

void HEAP::HeapifyDown(int i)
{
}

HEAP::insertElement(int key, void *pData)
{
    if (nElements >= size)
        resize();
        
    pE[nElements].key = key;
    pE[nElements].pData = pData;
    nElements++;
    HeapifyUp(nElements);
}

void * HEAP::getData(void)
{
    return ((nElements > 0) ? pE[0].pData : (void *)0);
}

int HEAP::Pop(void)
{
    ASSERT(nElements > 0, "Pop() on empty Heap");
    if (nElements > 0)
    {
        int k = pE[0].key;
        HeapSwap(pE, 1, nElements);
        nElements--;
        HeapifyDown(1);
        return k;
    }
    return 0;
}


void HEAP::resize(void)
{
    int newsize = ((size == 0) ? DEFNUMELEMENTS : size * 2);
    HeapElement *pNew = (HeapElement *) malloc(sizeof(HeapElement) * newsize);
    memset((void *)pNew, 0, sizeof(HeapElement) * newsize);
    if (pE)
    {
        memcpy((void *)pNew, (void *)pE, sizeof(HeapElement) * size);
        free(pE);
    }
    pE = pNew;
    size = newsize;
}