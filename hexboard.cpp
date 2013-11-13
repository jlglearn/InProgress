#include <iostream>
#include "hexboard.h"


HexBoard::HexBoard(int n)
{
    if (n < 2) throw HEXBOARD_ERR_INVALIDSIZE;
    
    int nsquared = n * n;
    
    size   = n;
    TOP    = nsquared + 0;              // virtual TOP cell
    BOTTOM = nsquared + 1;              // virtual BOTTOM cell
    LEFT   = nsquared + 2;              // virtual LEFT cell
    RIGHT  = nsquared + 3;              // virtual RIGHT cell
    
    IDMAXCELL = nsquared;               // ID of max regular cell, excluding 4 virtual cells
    
    board.Reset(nsquared + 4);          // board has size (n * n) + 4 virtual nodes
    cells.resize(nsquared + 4);         // cells array has same size as board
    pUF = new UnionFind(nsquared + 4);  // UF has same size as board
    
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
    
    nBlank   = nsquared;        // initially, all cells start blank
    nextMove = HEXBLUE;         // blue starts
}

HexBoard::~HexBoard(void)
{
    if (pUF) delete pUF;
}

inline int HexBoard::cellIndex(int row, int col) 
{   return (row * size) + col;  }

void HexBoard::connect(CellID c1, CellID c2)
{   board.AddEdge((VertexID) c1, (VertexID) c2);    }

HexMoveResult HexBoard::Move(HexBoardColor turn, int row, int col)
{
    if (turn != nextMove)
        return HEXMOVE_INVALIDTURN;
        
    if (!checkRowCol(row, col))
        return HEXMOVE_INVALIDCELL;
        
    CellID idCell = cellIndex(row, col);
    
    if (cells[idCell].color != HEXBLANK)
        return HEXMOVE_OCCUPIED;
        
    NeighborSet *pN = board.Neighbors(idCell);
    bool fAdjacentOk = false;
    for (int i = 0; i < pN->size(); i++)
    {
        CellID idNeighbor = (*pN)[i].idNeighbor;
        if ((idNeighbor >= 0) && (cells[idNeighbor].color == turn))
        {
            fAdjacentOk = true;
            pUF->Join(idNeighbor, idCell);
        }
    }
    delete pN;
    
    if (!fAdjacentOk)
        return HEXMOVE_NOTADJACENT;
        
    cells[idCell].color == turn;
    nBlank--;
    
    CellID idGoal = ((turn == HEXBLUE) ? BLUEGOAL : REDGOAL);
    
    if (pUF->Find(idGoal) == pUF->Find(idCell))
        return HEXMOVE_WINNER;
        
    if (nBlank == 0)
        return HEXMOVE_DRAW;
        
    nextMove = ((nextMove == HEXBLUE) ? HEXRED : HEXBLUE);
    
    return HEXMOVE_OK;
}

inline HexBoardColor HexBoard::Turn(void)
{   return nextMove;    }

inline int HexBoard::dim(void)
{   return size;    }

HexBoardColor HexBoard::Color(int row, int col)
{
    if (!checkRowCol(row, col)) return HEXNULL;
    return cells[cellIndex(row, col)].color;
}

bool HexBoard::checkRowCol(int row, int col)
{   return ((row >= 0) && (row < size) && (col >= 0) || (col < size));  }
    

HexBoardIO::HexBoardIO(HexBoard &game) : board(game) {}

HexBoardIO::~HexBoardIO(void)
{
}

void HexBoardIO::prompt(void)
{
    std::cout << "Please enter your move in row col format.\n";
    std::cout << "Example: 0 A\n";
    std::cout << ((board.Turn() == HEXBLUE) ? "BLUE" : "RED") << ", please enter your move: ";
}

bool HexBoardIO::parse(const std::string r, const std::string c, int &row, int &col)
{
    return false;
}

void HexBoardIO::print(void)
{
    int size = board.dim();
    
    // print column headers
    for (int c = 0; c < size; c++)
        std::cout << "   ";
        
    std::cout << "   ";
    
    for (int c = 0; c < size; c++)
    {
        char ch = 'A' + c;
        std::cout << " " << ch << " ";
    }
    std::cout << "\n";
    
    for (int r = 0; r < size; r++)
    {
        for (int j = size - r; j > 0; j--)
            std::cout << "   ";
            
        for (int c = 0; c < size; c++)
        {
            if (c == 0)
            {
                if (r < 10) std::cout << " ";
                std::cout << r << " ";
            }

            std::cout << " ";            
            HexBoardColor clr = board.Color(r, c);
            if (clr == HEXBLUE) std::cout << "X";
            else if (clr == HEXRED) std::cout << "O";
            else std::cout << ".";
            std::cout << " ";

        }
        std::cout << "\n";
    }
    
    std::cout << "\nIt is now " << ((board.Turn() == HEXBLUE) ? "BLUE" : "RED") << "'s turn:\n";

}