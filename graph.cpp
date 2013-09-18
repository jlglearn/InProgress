#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "debug.h"
#include "bitfield.h"
#include "graph.h"

static void * resizeBlock((void *) pOld, int oldsize, int newsize, int elementSize)
{
    void *pNew = (void *) malloc( elementSize * newsize );
    memset(pNew, 0, elementSize * newsize);
    if ((pOld != (void *)0) && (oldsize != 0))
    {
        memcpy(pNew, pOld, elementSize * oldsize);
        free(pOld);
    }
    return pNew;
}

GRAPH::GRAPH(void)
{
    pB = new BITFIELD(DEFNODESSIZE);
    
    sizeNodes = 0;
    sizeEdges = 0;
    nNodes = 0;
    nEdges = 0;
    
    pN = resizeNodes((GraphNode *)0, 0, DEFNODESSIZE);
    pE = resizeEdges((GraphEdge *)0, 0, DEFEDGESSIZE);
    sizeNodes = DEFNODESSIZE;
    sizeEdges = DEFEDGESSIZE;
    
    fLoading = 0;
}

GRAPH::~GRAPH(void)
{
    delete(pB);
    
    if (pN)
    {
        for (int i = 0; i < nNodes; i++)
        {
            if (pN[i].pvE) delete(pvE);
        }
        free(pN);
    }
    
    if (pE) free(pE);
}

int GRAPH::Load(char *filename)
{
    std::ifstream fs(filename);
    std::string s;
    
    fLoading = 1;
    for (int i = 0; getline(fs, s); i++)
    {
        std::istringstream ss(s);
        
        int src, dst;
        float w;
        
        ss >> src;
        ss >> dst;
        ss >> w;    
        
        addEdge(src, dst, w);
    }
    fLoading = 0;
    return i;
}

int GRAPH::addNode(int idNode)
{
    ASSERT(!pB->getBit(idNode), "GRAPH::addNode: node already added");
    
    if (nNodes >= sizeNodes)
        resizeNodes();
        
    pN[nNodes].id = idNode;
    pN[nNodes].pvE = new vector<int>(DEFNEIGHBORS);
    pB->setBit(idNode);
    nNodes++;
}

int GRAPH::addEdge(int idSrc, int idDst, float w)
{
    if (!pB->getBit(idSrc))
        addNode(idSrc);
        
    if (!pB->getBit(idDst))
        addNode(idDst);
        
    if (nEdges >= sizeEdges)
        resizeEdges();
        
    pE[nEdges].id = nEdges;
    pE[nEdges].idN1 = ((idSrc < idDst) ? idSrc : idDst);
    pE[nEdges].idN2 = ((idSrc < idDst) ? idDst : idSrc);
    pE[nEdges].w = w;
    
    if (!fLoading)
    {
        // if reading from file (loading), this will be done in bulk at the end of load
        recordEdge(idSrc, nEdges);
        recordEdge(idDst, nEdges);
    }
    
    nEdges++;
}

void GRAPH::resizeNodes(void)
{
    int newsize = ((sizeNodes == 0) ? DEFNODESSIZE : sizeNodes * 2);
    pN = resizeBlock((void *)pN, sizeNodes, newsize, sizeof(GraphNode));
}

void GRAPH::resizeEdges(void)
{
    int newsize = ((sizeEdges == 0) ? DEFEDGESSIZE : sizeEdges * 2);
    pE = resizeBlock((void *)pE, sizeEdges, newsize, sizeof(GraphEdge));
}

void GRAPH::recordEdge(int idNode, int idEdge)
{
}