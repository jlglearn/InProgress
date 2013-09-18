#include <iostream>
#include "graph.h"

int main(int argc, char *argv[])
{
    GRAPH g;
    char filename[] = "graph.txt";
    
    g.Load(filename);
    
    std::cout << "Done\n";
}