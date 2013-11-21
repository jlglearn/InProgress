



/* -------------------------------------------------------------------
   HexBoard class
   
   Manage the internal board state.
   ------------------------------------------------------------------- */
HexBoard::HexBoard(int n)
{
    if ((n < 0) || (n > HEX_MAXSIZE))
        throw HEXGAME_ERR_INVALIDSIZE;
        
    int n2 = n * n;
    int ncells = n2 + 4;

    size  = n;                          // remember board dimension
    UP    = n2 + 0;                     // define indices for TOP, DOWN, LEFT, RIGHT
    DOWN  = n2 + 1;
    LEFT  = n2 + 2;
    RIGHT = n2 + 3;
    
    cells.clear();
    cells.resize(ncells, HEXBLANK);     // make room for n x n board + 4 virtual cells
                                        // and initialize to BLANK (empty)
                   
    // precompute offset table for easy access of adjacent cells                   
    neighborOffsets[OFFSET_UP].indexOffset       = -size;
    neighborOffsets[OFFSET_UPRIGHT].indexOffset  = -size + 1;
    neighborOffsets[OFFSET_LEFT].indexOffset     = -1;
    neighborOffsets[OFFSET_RIGHT].indexOffset    = 1;
    neighborOffsets[OFFSET_DOWN].indexOffset     = size;
    neighborOffsets[OFFSET_DOWNLEFT].indexOffset = size - 1;
}


void HexBoard::Adjacent(int row, int col, HexPositionSet &hps, HexColor color = HEXNULL )
{
    int i, iCell, iNeighbor, iMax;
    
    CheckRowCol(row, col);
    
    iMax = size * size;
    hps.clear();
    
    iCell = cellIndex(row, col);

    for (int i = 0; i < NEIGHBORS; i++)
    {
        int iNeighbor = iCell + neigborOffsets[i].indexOffset;
        
        if ((iNeighbor < 0) || (iNeighbor >= iMax))
        {
            // out of bounds
            continue;
        }
        
        // if need to filter on color, make sure they're same color
        if ((color != HEXNULL) && (cells[iNeighbor].color != color))
            continue;
        
        // return information for current cell
        HexPosition p;        
        p.row = row + neighborOffsets[i].rowOffset;
        p.col = col + neighborOffsets[i].colOffset;
        p.color = cells[iNeighbor];
        hps.push_back(p);
    }
}

inline int HexBoard::dim(void)
{   return size;    }

HexColor HexBoard::Color(int row, int col)
{
    CheckRowCol(row, col);
    return cells[cellIndex(row, col)];
}



void HexBoard::CheckRowCol(int row, int col)
{   
    if ((row < 0) || (row >= size) || (col < 0) || (col >= size))
        throw HEXGAME_ERR_INVALIDCELL;
}

/* -------------------------------------------------------------------
   HexPosition class
   
   ------------------------------------------------------------------- */
HexPosition::HexPosition(void) { }

HexPosition::HexPosition(int row, int col, HexColor color)
{
    if ((row < 0) || (col < 0))
        throw HEXGAME_ERR_INVALIDCELL;
    this->row = row;
    this->col = col;
    this->color = color;
}

inline int HexPosition::Row(void) { return row; }
inline int HexPosition::Col(void) { return col; }
inline HexColor HexPosition::Color(void) { return color; }
