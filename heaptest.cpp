#include <iostream>
#include <cstdlib>
#include "debug.h"
#include "heap.h"

int main(int argc, char *argv[])
{
    HEAP H;
    
    for (int i = 0; i < 1000; i++)
    {
        H.insertElement( rand(), (void *) 0);
    }
    
    int current, previous;
    
    for (int i = 0; i < 1000; i++)
    {
        current = H.Pop();
        std::cout << "[" << i << "]: " << current;
        
        if ((i > 0) && (previous > current))
        {
            std::cout << " previous: " << previous << " ERROR!";
        }
        
        std::cout << "\n";
        previous = current;
    }
}