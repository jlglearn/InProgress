#include "hexboard.h"

HexBoard::HexBoard(int n)
{
    if (n < 2) throw HEXBOARD_ERR_INVALIDSIZE;
    
    TOP    = n * n + 0;                 // virtual TOP cell
    BOTTOM = n * n + 1;                 // virtual BOTTOM cell
    LEFT   = n * n + 2;                 // virtual LEFT cell
    RIGHT  = n * n + 3;                 // virtual RIGHT cell
    
    board.Reset(n * n + 4);             // board has size (n * n) + 4 virtual nodes
    cells.Resize(n * n + 4);            // cells array has same size as board
    
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            int idCell = cellIndex(r, c);
            
            // initialize cell to BLANK
            cells[idCell].idCell = idCell;
            cells[idCell].color = HEXBLANK;
            
            // unless left-most column, connect left and down-left
            if (c > 0)
            {
                // connect with cell to the left
                connect(idCell, cellIndex(r, c-1));
                
                // unless bottom row, connect with cell down-left
                if (r < (size - 1)) connect(idCell, cellIndex(r+1, c-1));
            }
            
            // unless right-most column, connect right
            if (c < (size - 1)) connect(idCell, cellIndex(r, c+1));
            
            // unless bottom row, connect down
            if (r < (size - 1)) connect(cellIndex(r,c), cellIndex(r+1, c));
            
            // if leftmost column, connect with LEFT virtual cell
            if (c == 0) connect(idCell, LEFT);
            
            // if rightmost column, connect with RIGHT virtual cell
            if (c == (size - 1)) connect(idCell, RIGHT);
            
            // if topmost row, connect with TOP virtual cell
            if (r == 0) connect(idCell, TOP);
            
            // if bottommost row, connect with BOTTOM cell
            if (r == (size - 1)) connect(idCell, BOTTOM);
        }
    }
    
    // initialize BLUEHOME, REDHOME, BLUEGOAL, REDGOAL
    BLUEHOME = BOTTOM;
    BLUEGOAL = TOP;
    REDHOME  = LEFT;
    REDGOAL  = RIGHT;
    
    nextMove = HEXBLUE;         // blue starts
}

inline int HexBoard::cellIndex(int row, int col) 
{   return (row * size) + col;  }

void HexBoard::connect(CellID c1, CellID c2)
{   board.AddEdge((VertexID) c1, (VertexID) c2);    }

HexMoveResult HexBoard::Move(HexBoardColor turn, int row, int col)
{
}

    
