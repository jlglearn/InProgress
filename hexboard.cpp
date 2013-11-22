#include "hexboard.h"

/* ---- HexBoard::HexBoard(int n) ---------------------------------------------
        Allocates a board of n x n cells
   ---------------------------------------------------------------------------- */
HexBoard::HexBoard(int n)
{   Reset(n);   }

/* ---- void HexBoard::Reset(int n) -------------------------------------------
        Clear board and all internal states, and initialize to empty n x n board
   ---------------------------------------------------------------------------- */
void HexBoard::Reset(int n)
{
    if ((n < HEXMINSIZE) || (n > HEXMAXSIZE))
        throw HEXGAME_ERR_INVALIDSIZE;
        
    int n2 = n * n;
    cells.clear();                  // reset cells
    cells.resize(n2 + 4, HEXBLANK); // allocate n x n + 4 virtual cells
    UF.Reset(n2 + 4);               // and UnionFind structure same size
    nFree = n2;                     // initially, all cells are free
    size = n;                       // remember size
    
    // compute indices for BLUEHOME, BLUEGOAL, REDHOME, REDGOAL
    BLUEHOME = n2 + 0;
    BLUEGOAL = n2 + 1;
    REDHOME  = n2 + 2;
    REDGOAL  = n2 + 3;
}

/* ---- HexColor HexBoard::Color(int row, int col) ---------------------------
        Returns the color (HEXBLUE, HEXRED, HEXBLANK) of the given cell.
        throws HEXGAME_ERR_INVALIDCELL exception if row, col invalid.
   ---------------------------------------------------------------------------- */
HexColor HexBoard::Color(int row, int col)
{
    CheckRowCol(row, col);
    return cells[cellIndex(row, col)];
}


/* ---- int dim(void) ---------------------------------------------------------
        Returns the dimension of the board (number of cells per side)
   ---------------------------------------------------------------------------- */
int HexBoard::dim(void)
{   return size;    }


/* ---- void Adjacent(int row, int col, HexPositionSet &hps, HexColor color=HEXNULL)
        Return the cells adjacent to the given (row, col) cell.
        An optional color parameter will restrict the results to adjacent cells
        of the same color as the given (row, col) cell.
   
        Client code must provide a valid reference to a HexPositionSet object
        that will be used to return information about the adjacent cells.
        
        Throws HEXGAME_ERR_INVALIDCELL if row,col are invalid.
   ---------------------------------------------------------------------------- */
void HexBoard::Adjacent(int row, int col, HexPositionSet &hps, HexColor color)
{
    HexNeighborOffset NO[6] = {
        {-1,  0},     // offset to UP neighbor
        {-1,  1},     // offset to UP-RIGHT neighbor
        { 0, -1},     // offset to LEFT neighbor
        { 0,  1},     // offset to RIGHT neighbor
        { 1,  0},     // offset to DOWN neighbor
        { 1, -1}      // offset to DOWN-LEFT neighbor
    };
    
    CheckRowCol(row, col);
    
    int index = cellIndex(row, col);    // compute the index for given (row, col)
    int iMax  = size * size;            // compute max valid index
    
    hps.clear();                        // clear results
    
    // examine each of the 6 possible neighbors, and copy the information
    // for all qualifying cells
    for (int iNeighbor = 0; iNeighbor < 6; iNeighbor++)
    {
        int thisRow = row + NO[iNeighbor].rowOffset;
        int thisCol = col + NO[iNeighbor].colOffset;
        
        if ((thisRow < 0) || (thisRow >= size) || (thisCol < 0) || (thisCol >= size))
        {
            // out of bounds, ignore
            continue;
        }
        
        // index for this adjacent cell
        int thisIndex = cellIndex(thisRow, thisCol);
        
        HexColor thisColor = cells[thisIndex];      // color of adjacent cell
        
        if ((color == HEXNULL) || (color == thisColor))
        {
            // if don't care about color, or cell is of correct color, copy it
            HexPosition p;
            p.row = thisRow;
            p.col = thisCol; 
            p.color = thisColor;
            hps.push_back(p);
        }
    }    
}

/* ---- void AllCells(HexPositionSet &hps, HexColor color) --------------------
        Return all the board cells of the given color.
        Valid color options are HEXBLUE, HEXRED, HEXBLANK
        
        Client code must provide a valid reference to a HexPositionSet object
        that will be used to return information about the adjacent cells.
        
        Throws HEXGAME_ERR_INVALIDCOLOR if given color is invalid
   ---------------------------------------------------------------------------- */
void HexBoard::AllCells(HexPositionSet &hps, HexColor color)
{
    int iMax  = size * size;            // compute max valid index    
    hps.clear();                        // clear results
    
    // examine each board cell, and copy the information of all qualifying cells
    for (int i = 0; i < iMax; i++)
    {        
        HexColor thisColor = cells[i];  // color of current cell
        
        if (color == thisColor)
        {
            // if matching color, copy it
            HexPosition p;
            p.row = rowFromIndex(i);
            p.col = colFromIndex(i);
            p.color = thisColor;
            hps.push_back(p);
        }
    }    
}       

/* ---- HexMoveResult Move(int row, int col, HexColor color) ------------------
        Place a colored chip on the given row, col.
        
        HEXBLUE moves from DOWN to UP, 
        HEXRED moves from LEFT to RIGHT
        
        The function does not enforce any other rules.  In particular, it does
        not enforce alternate turns.
        
        Return value is one of the following:
        HEXMOVE_INVALIDCOLOR : given color is invalid
        HEXMOVE_INVALIDCELL:   given (row, col) is invalid
        HEXMOVE_OCCUPIED:      given cell already occupied
        HEXMOVE_WINNER:        move results in a win
        HEXMOVE_DRAW:          move results in a draw
        HEXMOVE_OK:            move accepted
   ---------------------------------------------------------------------------- */
HexMoveResult HexBoard::Move(int row, int col, HexColor color)
{
    if ((row < 0) || (row >= size) || (col < 0) || (col >= size))
        return HEXMOVE_INVALIDCELL;
        
    if ((color != HEXBLUE) && (color != HEXRED))
        return HEXMOVE_INVALIDCOLOR;

    int thisIndex = cellIndex(row, col);
    if (cells[thisIndex] != HEXBLANK)
        return HEXMOVE_OCCUPIED;

    cells[thisIndex] = color;               // place colored chip on given cell
    nFree--;                                // one fewer free cell

    // link to any adjacent cells of same color
    HexPositionSet hps;
    Adjacent(row, col, hps, color);    
    for (int i = 0; i < hps.size(); i++)
    {
        UF.Join(thisIndex, cellIndex(hps[i].row, hps[i].col));
    }
    
    // maintain 4 virtual cells
    if (color == HEXBLUE)
    {
        // BLUE moves from DOWN to UP
        if (row == (size - 1)) 
            UF.Join(thisIndex, BLUEHOME);
        if (row == 0) 
            UF.Join(thisIndex, BLUEGOAL);
            
        if (UF.Find(BLUEHOME) == UF.Find(BLUEGOAL))
        {
            // move results in BLUE winning game
            return HEXMOVE_WINNER;
        }
    }
    
    if (color == HEXRED)
    {
        // RED moves from LEFT to RIGHT
        if (col == 0)
            UF.Join(thisIndex, REDHOME);
        if (col == (size - 1))
            UF.Join(thisIndex, REDGOAL);
            
        if (UF.Find(REDHOME) == UF.Find(REDGOAL))
        {
            // move results in RED winning game
            return HEXMOVE_WINNER;
        }
    }
    
    if (nFree == 0)
    {
        // no more free cells
        return HEXMOVE_DRAW;
    }
    
    return HEXMOVE_OK;
}


/* ---- void CheckRowCol(int row, int col) ------------------------------------
        Verifies given row, col are valid cell coordinates.
        Throws HEXGAME_ERR_INVALIDCELL if invalid.
   ---------------------------------------------------------------------------- */
void HexBoard::CheckRowCol(int row, int col)
{
    if ((row < 0) || (row >= size) || (col < 0) || (col >= size))
        throw HEXGAME_ERR_INVALIDCELL;
}

