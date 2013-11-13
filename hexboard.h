#ifndef _HEXBOARD_H_
#define _HEXBOARD_H_

#include <vector>
#include <string>

#include "unionfind.h"
#include "graph.h"


enum HexBoardColor { HEXNULL = 0, HEXBLANK, HEXBLUE, HEXRED };

static const int _HexErrBase = 0x122;
enum HexBoardError {
    HEXBOARD_ERR_OUTOFMEMORY = _HexErrBase,
    HEXBOARD_ERR_INVALIDSIZE,
    HEXBOARD_ERR_INVALIDCELL
};

enum HexMoveResult {
    HEXMOVE_OK,
    HEXMOVE_INVALIDTURN,
    HEXMOVE_INVALIDCELL,
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
    ~HexBoard(void);
    
    HexBoardColor Color(int row, int col); // return the color of the given cell (HEXBLANK, HEXBLUE, HEXRED, or HEXNULL if error)
    HexBoardColor Turn(void);           // return whose turn it is to play
    HexMoveResult Move(HexBoardColor turn, int row, int col);   // place chip of given color at (row, col)
    
    int dim(void);                      // dimension of the board (in cells per side)
    
    private:    
    Graph               board;          // Graph underlying structure for path management
    HexBoardCells       cells;          // cell-specific information, including color
    
    int size;                           // board size (number of rows, number of columns)
    HexBoardColor nextMove;             // who moves next
    
    int TOP, BOTTOM, LEFT, RIGHT;       // index of 4 virtual cells
    int BLUEHOME, REDHOME;              // starting location for blue and red
    int BLUEGOAL, REDGOAL;              // target location for blue and red
    
    int IDMAXCELL;                      // ID of max regular cell on the board (excluding virtual cells)
    
    int nBlank;                         // number of blank cells remaining
    
    UnionFind *pUF;                     // compute winners easily

    
    int cellIndex(int row, int col);    // given a (row, column) pair, compute index of node
    void connect(CellID c1, CellID c2); // create an edge between cells c1 and c2
    bool checkRowCol(int row, int col); // check that a given (row, column) pair is within board size
    
    
    friend class HexBoardIO;
    
};

class HexBoardIO {
    public:
    HexBoardIO(HexBoard &game);
    ~HexBoardIO(void);
    void print(void);
    void prompt(void);
    bool parse(const std::string r, const std::string c, int &row, int &col);
    void moveFeedback(HexMoveResult result);
    
    private:
    HexBoard &board;
};


#endif