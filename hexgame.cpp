

/* -------------------------------------------------------------------
   HexGame class
   
   Manage turns, enforce rules, determine winner.
   ------------------------------------------------------------------- */
HexGame::HexGame(int n)
{
    int n2 = n * n;
    
    pBoard = new HexBoard(n);           // create board    
    pUF = new UnionFind(n2 + 4);        // create UnionFind for easy winner detection
    cFree = n2;                         // start with all cells free    
    turn = HEXBLUE;                     // BLUE moves first
    
    // set the indices of the 4 virtual cells
    BLUEHOME = n2 + 0;
    BLUEGOAL = n2 + 1;
    REDHOME  = n2 + 2;
    REDGOAL  = n2 + 3;
    
}

HexGame::~HexGame(void)
{
    if (pBoard) delete pBoard;
    if (pUF) delete pUF;
}

// ------ return the color of the given cell
HexColor HexGame::Color(int row, int col)
{  return pBoard->Color(row, col);  }

// ------ return whose turn it is to play
HexColor HexGame::Turn(void)
{  return turn; }

// ------ return the board dimension
int HexGame::dim(void)
{   return pBoard->dim();   }

// ------ accept a move, determine whether it's legal and whether it leads to a win
HexMoveResult HexGame::Move(HexColor turn, int row, int col)
{
    if (turn != this->turn) 
        return HEXMOVE_INVALIDTURN;
        
    if (!CheckRowCol(row, col)) 
        return HEXMOVE_INVALIDCELL;
    
    if (pBoard->Color(row, col) != HEXBLANK)
        return HEXMOVE_OCCUPIED;

    HexPositionSet hps;
    int iHome, iGoal;
    int iCell = pBoard->cellIndex(row, col);
    int size  = pBoard->dim();

    // set current cell's color
    pBoard->SetColor(row, col, turn);
    
    // retrieve adjacent cells of same color
    pBoard->Adjacent(row, col, hps, turn);
    
    // and connect to them        
    for (int i = 0; i < hps.size(); i++)
        pUF->Join(iCell, pBoard->cellIndex(hps.row, hps.col));
    
    // BLUE moves DOWN-UP
    if (turn == HEXBLUE)
    {
        if (row == 0) 
            pUF->Join(iCell, BLUEGOAL);
            
        if (row == (size - 1)) 
            pUF->Join(iCell, BLUEHOME);
            
        iHome = BLUEHOME;
        iGoal = BLUEGOAL;
    }
    else // RED moves LEFT-RIGHT
    {
        if (col == 0)
            pUF->Join(iCell, REDHOME);
            
        if (col == (size - 1))
            pUF->Join(iCell, REDGOAL);
            
        iHome = REDHOME;
        iGoal = REDGOAL;
    }
    
    // determine whether player has won
    if (pUF->Find(iHome) == pUF->Find(iGoal))
        return HEXMOVE_WINNER;
        
    // determine whether any empty cell remains
    if (--cFree == 0)
        return HEXMOVE_DRAW;
        
    // record whose next turn
    this->turn = ((turn == HEXBLUE) ? HEXRED : HEXBLUE);
        
    return HEXMOVE_OK;
}


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
    
    if (!CheckRowCol(row, col)) throw HEXGAME_ERR_INVALIDCELL;
    
    iMax = size * size;
    hps.clear();
    
    iCell = cellIndex(row, col);

    for (int i = 0; i < NEIGHBORS; i++)
    {
        int iNeighbor = iCell + neighborOffsets[i].indexOffset;
        
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

void HexBoard::AllCells(HexPositionSet &hps, HexColor color = HEXBLANK )
{
    int iMax = size * size;
    
    hps.clear();
    
    // iterate through all cells and return those that are of the requested color
    for (int i = 0; i < iMax; i++)
    {
        if (cells[i] == color)
        {
            HexPosition p;
            p.row = rowFromIndex(i);
            p.col = colFromIndex(i);
            p.color = color;
            hps.push_back(p);
        }
    }
}

inline int HexBoard::dim(void)
{   return size;    }

HexColor HexBoard::Color(int row, int col)
{
    if (!CheckRowCol(row, col)) throw HEXGAME_ERR_INVALIDCELL;
    return cells[cellIndex(row, col)];
}

void HexBoard::SetColor(int row, int col, HexColor color)
{
    if (!CheckRowCol(row, col)) throw HEXGAME_ERR_IVALIDCELL;
    
    int iCell = cellIndex(row, col);        
    if (cells[iCell] != HEXBLANK) throw HEXGAME_ERR_CELLOCCUPIED;
    
    cells[iCell] = color;
}

bool HexBoard::CheckRowCol(int row, int col)
{   return ((row >= 0) && (row < size) && (col >= 0) && (col < size));  }

/* -------------------------------------------------------------------
   HexPosition class   
   ------------------------------------------------------------------- */
HexPosition::HexPosition(void) { }
inline int HexPosition::Row(void) { return row; }
inline int HexPosition::Col(void) { return col; }
inline HexColor HexPosition::Color(void) { return color; }
