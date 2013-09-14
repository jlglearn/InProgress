#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include "graph.h"

Graph::Graph()
{
    nNodes = 0;
    nEdges = 0;
    memset((void *)&bf[0], 0, sizeof(bf));
}

Graph::~Graph()
{
}

int Graph::addNode(int idNode)
{
    if ( !hasNode(idNode))
    {
        Nodes[nNodes].id = idNode;
        Nodes[nNodes].nNeighbors = 0;
        setBit(idNode);
        nNodes++;
    }
}

int Graph::addEdge(int src, int dst, double w)
{
}

int Graph::hasNode(int idNode)
{
    return (getBit(idNode) != 0);
}

int Graph::LoadFile(char *filename)
{
    ifstream f(filename);
    std::string s;
    
    int cNodes;
    
    for (int i = 0; getline(f, s); i++)
    {
        int idNode, neighbor;
        isstream ss(s);
        
        if (i > 0)
        {
            // first item is node id
            ss >> idNode;
            
            if (!hasNode(idNode))
                addNode(idNode);
            
            // remaining items are its neighbors
            while ( ss >> neighbor )
            {
                if (!hasNode(neighbor))
                    addNode(neighbor);
                    
                addEdge(idNode, neighbor, 1.0);
            }
            
        }
        else
        {
            // read number of nodes
            s >> cNodes;
        }
    
    
    return 1;
}

int Graph::WriteFile(char *filename)
{
    return 1;
}

void Graph::setBit(int id)
{
    ASSERT(id <= (8*sizeof(bf)), "setBit::id out of range");
    int i = (id / (8*sizeof(int)));
    int k = (1 << (id % (8*sizeof(int))));
    bf[i] |= k;
}

int Graph::getBit(int id)
{
    ASSERT(id <= (8*sizeof(bf)), "getBit::id out of range");
    int i = (id / (8*sizeof(int)));
    int k = (1 << (id % (8*sizeof(int))));
    return bf[i] & k;
}
