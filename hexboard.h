#ifndef _HEXBOARD_H_
#define _HEXBOARD_H_

#include <vector>
#include "graph.h"


enum HexBoardColor { HEXBLANK, HEXBLUE, HEXRED };

enum HexBoardError {
    HEXBOARD_ERR_OUTOFMEMORY,
    HEXBOARD_ERR_INVALIDSIZE
};

enum HexMoveResult {
    HEXMOVE_OK,
    HEXMOVE_INVALIDTURN,
    HEXMOVE_NOTADJACENT,
    HEXMOVE_OCCUPIED,
    HEXMOVE_WINNER,
    HEXMOVE_DRAW
};

typedef int CellID;

typedef struct structHexBoardCell {
    CellID idCell;
    HexBoardColor color;
} HexBoardCell;

typedef std::vector<HexBoardCell> HexBoardCells;
        

    
class HexBoard {

    public:
    HexBoard(int n);
    HexBoardColor color(CellID idCell);
    
    HexMoveResult Move(HexBoardColor turn, int row, int col);
    
    private:    
    Graph               board;          // Graph underlying structure for path management
    HexBoardCells       cells;          // cell-specific information, including color
    
    int size;                           // board size (number of rows, number of columns)
    HexBoardColor nextMove;             // who moves next
    
    int TOP, BOTTOM, LEFT, RIGHT;       // index of 4 virtual cells
    int BLUEHOME, REDHOME;              // starting location for blue and red
    int BLUEGOAL, REDGOAL;              // target location for blue and red

    
    int cellIndex(int row, int col);    // given a (row, column) pair, compute index of node
    void connect(CellID c1, CellID c2); // create an edge between cells c1 and c2
    
};


#endif