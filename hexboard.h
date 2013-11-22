#ifndef _HEXBOARD_H_
#define _HEXBOARD_H_

#include <vector>
#include "unionfind.h"

/* ----------------------------------------------------------------------------
   define some useful types and constants
   ---------------------------------------------------------------------------- */
typedef enum enumHexColor {
    HEXNULL = -1, HEXBLANK, HEXBLUE, HEXRED
} HexColor;

typedef enum enumHexMoveResult {
    HEXMOVE_INVALIDCELL = -4,
    HEXMOVE_INVALIDTURN,
    HEXMOVE_INVALIDCOLOR,
    HEXMOVE_OCCUPIED,
    HEXMOVE_OK = 0,
    HEXMOVE_DRAW,
    HEXMOVE_WINNER
} HexMoveResult;

typedef std::vector<HexColor> HexCellSet;

typedef struct structHexPosition {
    int row;
    int col;
    HexColor color;
} HexPosition;

typedef std::vector<HexPosition> HexPositionSet;

typedef enum enumHexDirection {
    HEXDIRECTION_DOWNUP,
    HEXDIRECTION_LEFTRIGHT
} HexDirection;

const int HEXMINSIZE = 3;
const int HEXMAXSIZE = 25;

typedef enum enumHexGameError {
    HEXGAME_ERR_INVALIDSIZE = 0x111,
    HEXGAME_ERR_INVALIDCELL,
    HEXGAME_ERR_INVALIDCOLOR
} HexGameError;

typedef struct structHexNeighborOffset {
    int rowOffset;
    int colOffset;
} HexNeighborOffset;

/* ----------------------------------------------------------------------------
   class HexBoard
   
   ---------------------------------------------------------------------------- */
class HexBoard {
    public:
    HexBoard(int n);
    
    HexColor Color(int row, int col);
    HexMoveResult Move(int row, int col, HexColor color);
    void Adjacent(int row, int col, HexPositionSet &hps, HexColor color=HEXNULL);
    void AllCells(HexPositionSet &hps, HexColor color);
    int dim(void);
    void Reset(int n);
    
    private:
    HexCellSet cells;
    UnionFind UF;
    int nFree;
    int size;
    
    int BLUEHOME, BLUEGOAL, REDHOME, REDGOAL;
    
    void CheckRowCol(int row, int col);
    inline int cellIndex(int row, int col) { return row * size + col; }
    inline int rowFromIndex(int index) { return index / size; }
    inline int colFromIndex(int index) { return index % size; }
    friend class HexGame;
};

/* ----------------------------------------------------------------------------
   class HexGame
   
   Accepts player registration, starts a match, enforces game rules such as
   alternating turns.
   ---------------------------------------------------------------------------- */
class HexGame {
    public:
    HexGame(int n);
    HexRegisterResult RegisterPlayer(HexPlayer &player, HexColor color=HEXNULL);
    HexMoveResult Play(HexColor movesFirst=HEXNULL);

    private:
    HexBoard  board;
    HexPlayer bluePlayer;
    HexPlayer redPlayer;
    HexColor  movesFirst;
};

#endif