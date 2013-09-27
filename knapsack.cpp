#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "debug.h"

#define ELEMENT(A,i,x,n) A[((x)*(n))+(i)]
#define MAX(x,y) (((x) >= (y)) ? (x) : (y))

typedef struct structItem {
	int id;
	int v;
	int w;
} Item;

typedef struct structKnapSack {
	int W;
	int n;
	Item *items;
	int *pSolution;
} KnapSack;

typedef struct structSolutionSet {
	int nItems;
	int maxWeight;
	int maxValue;
	int *items;
} SolutionSet;
	

void PrintSolutionMatrix(int *A, int n, int W)
{
	for (int i = 0; i < n; i++)
	{
		for (int x = 0; x <= W; x++)
			std::cout << ELEMENT(A,i,x,n) << "\t";
		std::cout << "\n";
	}
}

SolutionSet * AllocateSolutionSet(int n)
{
	int size = sizeof(SolutionSet) + (sizeof(int) * n);
	unsigned char *p = (unsigned char *) malloc(size);
	if (!p)
	{
		std::cerr << "Can't allocate enough memory for solution set (" << n << ").\n";
		return (SolutionSet *)0;
	}
	memset((void *)p, 0, size);
	
	unsigned char *q = &p[sizeof(SolutionSet)];
	SolutionSet *pss = (SolutionSet *)p;
	pss->items = (int *)q;
	return pss;
}

int * AllocateSolutionMatrix(int n, int m)
{
	int size = sizeof(int) * n * m;
	int *A = (int *)malloc(size);
	if (!A)
	{
		std::cerr << "Can't allocate enough memory for solution matrix (" << n << "x" << m << ").\n";
		return A;
	}
	memset((void *)A, 0, size);
	return A;
}

SolutionSet * ComputeSolutionSet(KnapSack *pk, int *A)
{
	int n = pk->n;
	
	SolutionSet *pss = AllocateSolutionSet(n);
	if (!pss) return pss;
	
	for (int i = n-1, curW = pk->W; i >= 0; i--)
	{
		int thisW = pk->items[i].w;
		int thisV = pk->items[i].v;
		
		if (curW < thisW)
		{
			// can't fit current item in current available capacity, skip
			continue;
		}
				
		int vWith = thisV + ((i == 0) ? 0 : ELEMENT(A,i-1,curW-thisW,n));
		int vWithout = ((i == 0) ? 0 : ELEMENT(A,i-i,curW,n));
		
		if (vWith >= vWithout)
		{
			// better off with current item in, commit
			pss->items[pss->nItems++] = i;
			pss->maxWeight += thisW;
			pss->maxValue += thisV;
			
			curW -= thisW;
		}
	}
	
	return pss;
}

int * ComputeSolutionMatrix(KnapSack *pk)
{

	int n = pk->n;	
	int *A = AllocateSolutionMatrix(n, pk->W+1);
	if (!A) return A;
	
	for (int i = 0; i < n; i++)
	{
		int thisV = pk->items[i].v;
		int thisW = pk->items[i].w;
		
		for (int x = 0; x <= pk->W; x++)
		{
			int prevV = ((i == 0) ? 0 : ELEMENT(A,i-1,x,n));
			
			if (thisW > x)
				ELEMENT(A,i,x,n) = prevV;
			else if (i > 0)
				ELEMENT(A,i,x,n) = MAX(prevV, thisV + ELEMENT(A,i-1,x-thisW,n));
			else
				ELEMENT(A,i,x,n) = thisV;
		}
	}

	return A;
}

void ReportSolution(SolutionSet *pss, KnapSack *pk)
{
	std::cout << "Computed solution set S={";
		
	for (int i = 0; i < pss->nItems; i++)
		std::cout << pk->items[pss->items[i]].id << ((i < (pss->nItems-1)) ? ", " : "");
			
	std::cout << "}\nmaxWeight=" << pss->maxWeight << " maxValue=" << pss->maxValue << "\n";
}

int Solve(KnapSack *pk)
{
	int *A = ComputeSolutionMatrix(pk);	
	if (!A) return 0;
	
	SolutionSet *S = ComputeSolutionSet(pk, A);
	if (!S) return 0;
	
	ReportSolution(S, pk);
	
	free(S);
	free(A);
	return 1;
}


KnapSack *LoadFile(char *filename)
{
	std::ifstream f(filename);
	std::string line;
	KnapSack *pk = (KnapSack *)0;
	int n, W;
	
	for (int i = 0; getline(f, line); i++)
	{
		std::istringstream ss(line);
		
		if (i > 0)
		{
			if (i > n) break;
			
			pk->items[i-1].id = i;
			ss >> pk->items[i-1].v;
			ss >> pk->items[i-1].w;
		}
		else
		{
			// first iteration, allocate and initialize knapsack structure
			// read capacity and number of items
			ss >> W;
			ss >> n;
			
			int size = sizeof(KnapSack) + n * sizeof(Item);
			pk = (KnapSack *) malloc(size);
			
			if (pk == (KnapSack *)0)
				return pk;
				
			memset((void *)pk, 0, size);
			unsigned char *p = (unsigned char *)pk;
			unsigned char *q = &p[sizeof(KnapSack)];
			pk->items = (Item *)q;
			pk->n = n;
			pk->W = W;
		}
	}
	
	return pk;

}


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " filename\n";
		return 2;
	}
	
	KnapSack *pk = LoadFile(argv[1]);
	
	if (pk == (KnapSack *)0)
	{
		std::cerr << "Can't allocate enough memory.\n";
		return 3;
	}
	
	std::cout << "Read " << pk->n << " items, total capacity: " << pk->W << ".\n";
	
	Solve(pk);
	
	if (pk) free(pk);
	return 0;
}