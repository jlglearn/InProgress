#ifndef _HEXBOARD_H_
#define _HEXBOARD_H_

#include "graph.h"

enum HEXCOLOR { HEXBLUE, HEXRED };

class HexBoard {

    public:
    HexBoard(int n);
    
    private:
    
    Graph board;
    int   size;                         // board size (number of rows, number of columns)
    int TOP, BOTTOM, LEFT, RIGHT;       // index of 4 virtual cells
    
    int cellIndex(int row, int col);    // given a (row, column) pair, compute index of node
};

#endif

HexBoard::HexBoard(int n)
{
    if (n < 2) throw HEXBOARD_ERR_INVALIDSIZE;
    
    TOP    = n * n + 0;                 // virtual TOP cell
    BOTTOM = n * n + 1;                 // virtual BOTTOM cell
    LEFT   = n * n + 2;                 // virtual LEFT cell
    RIGHT  = n * n + 3;                 // virtual RIGHT cell
    
    board.Reset(n * n + 4);             // board has size (n * n) + 4 virtual nodes
    
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            int thisCell = index(r, c);
            
            // unless left-most column, connect left and down-left
            if (c > 0)
            {
                // connect with cell to the left
                connect(thisCell, cellIndex(r, c-1));
                
                // unless bottom row, connect with cell down-left
                if (r < (size - 1)) connect(thisCell, index(r+1, c-1));
            }
            
            // unless right-most column, connect right
            if (c < (size - 1)) connect(thisCell, index(r, c+1));
            
            // unless bottom row, connect down
            if (r < (size - 1)) connect(index(r,c), index(r+1, c));
            
            // if leftmost column, connect with LEFT virtual cell
            if (c == 0) connect(thisCell, LEFT);
            
            // if rightmost column, connect with RIGHT virtual cell
            if (c == (size - 1)) connect(thisCell, RIGHT);
            
            // if topmost row, connect with TOP virtual cell
            if (r == 0) connect(thisCell, TOP);
            
            // if bottommost row, connect with BOTTOM cell
            if (r == (size - 1)) connect(thisCell, BOTTOM);
        }
    }
}

inline int HexBoard::index(int row, int col) 
{   return (r * size) + c;  }
    
