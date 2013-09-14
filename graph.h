#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>

#define DEFNUMNODES    1024
#define DEFNUMEDGES    1024


struct structGraphEdge {
    int id;
    int iN1;
    int iN2;
    double w;
};

typedef struct structGraphEdge GraphEdge;

struct structGraphNode {
    int id;
    int nNeighbors;
    std::vector<int> vEdges;
};

typedef struct structGraphNode GraphNode;

class Graph
{
public:
    Graph();
    ~Graph();
    
    int LoadFile(char *filename);
    int WriteFile(char *filename);
    int addNode(int idNode);
    int addEdge(int src, int dst, double w);
    int hasNode(int idNode);

private:
    std::vector<GraphNode> Nodes[DEFNUMNODES];
    std::vector<GraphEdge> Edges[DEFNUMEDGES];
    int nNodes;
    int nEdges;
    int bf[1024*1024/(8*sizeof(int))];
    
    void setBit(int id);
    int getBit(int id);
};

#endif