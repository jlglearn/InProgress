#ifndef _HEXGAME_H_
#define _HEXGAME_H_

#include <vector>
#include "unionfind.h"

// ----------------------------------------------------------------------------
// define chip colors
// ----------------------------------------------------------------------------
typedef enum enumHexColor {
    HEXNULL = 0, HEXBLANK, HEXBLUE, HEXRED
} HexColor;

// ----------------------------------------------------------------------------
// define Move results
// ----------------------------------------------------------------------------
typedef enum enumHexMoveResult {
    HEXMOVE_OK,
    HEXMOVE_INVALIDTURN,
    HEXMOVE_INVALIDCELL,
    HEXMOVE_OCCUPIED,
    HEXMOVE_WINNER,
    HEXMOVE_DRAW
} HexMoveResult;

// ----------------------------------------------------------------------------
// interface structure to communicate adjacency information between board
// and client (player)
// ----------------------------------------------------------------------------
class HexPosition {
    public:
    HexPosition(void);
    HexPosition(int row, int col, HexColor color);
    int Row(void);
    int Col(void);
    HexColor Color(void);
    
    private:
    int      row;
    int      col;
    HexColor color;
    
    friend class HexBoard;
};

// ----------------------------------------------------------------------------
// define collection (vector) of HexPosition objects
// ----------------------------------------------------------------------------
typedef std::vector<HexPosition> HexPositionSet;

// ----------------------------------------------------------------------------
// internal representation of the board (a vector of colors)
// ----------------------------------------------------------------------------
typedef std::vector<HexColor> HexCells;

typedef struct structNeighborOffset {
    int rowOffset;
    int colOffset;
    int indexOffset;
} NeighborOffset;

// ----------------------------------------------------------------------------
// HexBoard class:
// manages board representation as can be shared with client (player) code
// ----------------------------------------------------------------------------
class HexBoard {

    public:
    HexBoard(int n);
    ~HexBoard(void);
    
    // ------- return the color of a given cell
    HexColor Color(int row, int col);
    
    // ------- return the set of cells adjacent to the given cell
    //         client provides a reference to a HexPositionSet object that will
    //         be used to return adjacency information.
    //         an optional parameter (color) restricts adjacent cells that have
    //         specified color
    void Adjacent(int row, int col, HexPositionSet &hps, HexColor color = HEXNULL );
    
    // ------- return the dimension of the board
    inline int dim(void);
    
    private:
    int UP, DOWN, LEFT, RIGHT;
    const int NEIGHBORS = 6;
    
    int size;                                  // board dimension
    
    // offsets to access adjacent cells, pre-set rowOffset and colOffset.
    // indexOffset will be set in constructor, when we know the dimension of the board.
    NeighborOffset neighborOffsets[NEIGHBORS] = {
        { -1, 0, 0 }, { -1, 1, 0 },
        { 0, -1, 0 }, {  0, 1, 0 },
        { 1, -1, 0 }, { 1, -1, 0 }        
    };
    
    enum { OFFSET_UP = 0, OFFSET_UPRIGHT, 
           OFFSET_LEFT,   OFFSET_RIGHT,
           OFFSET_DOWN,   OFFSET_DOWNLEFT };
    
    HexCells cells;
    
    // ------- compute the index of given row, col
    inline int cellIndex(int row, int col) { return row * size + col; }
    
    // ------- check that the given row, col are valid
    void CheckRowCol(int row, int col);
};

// ----------------------------------------------------------------------------
// HexGame class
//    Maintains game internal states and enforces game turns and rules.
//
//    It uses a HexBoard structure to maintain the state of the board
//    It uses a UnionFind structure for easy detection of winning moves
// ----------------------------------------------------------------------------
class HexGame {

    public:
    HexGame(int n);
    ~HexGame(void);
    
    // ------- return the color of a given cell
    HexColor Color(int row, int col);
    
    // ------- return the color of the player whose turn it is to play
    HexColor Turn(void);
    
    // ------- accepts a player move, determine its validity and returns
    //         the result
    HexMoveResult Move(HexColor turn, int row, int col);
    
    // ------- returns the dimension of the board
    int dim(void);
    
    private:
    HexBoard  *pBoard;      // pointer to HexBoard structure to maintain board state
    UnionFind *pUF;         // pointer to UnionFind structure for quick detection of winning moves
    
    int       cFree;        // number of free cells
    
};

// ----------------------------------------------------------------------------
// Interface structure for player implementation
// ----------------------------------------------------------------------------
class HexPlayer {
    public:
    HexPlayer(void);
    ~HexPlayer(void);  
    
    // ------- Ask player to make a move
    virtual void Move(HexBoard &board, HexColor turn, int &row, int &col);
};

// ----------------------------------------------------------------------------
// HexGameDriver class
//
// Initiates and monitors a game between two registered players
// ----------------------------------------------------------------------------
class HexGameDriver {
    public:
    HexGameDriver(void);
    ~HexGameDriver(void);
    
    // -------- Register a player.  Returns the color assigned to the player
    //          (first registered player is assigned BLUE, second player, RED)
    //          HEXNULL is returned for any attempt to register players in
    //          excess of 2
    HexColor RegisterPlayer(HexPlayer &player);
    
    // -------- Play a game.  The computer will play the part of any unregistered
    //          player.
    //          returns the winner's color
    HexColor Play(void);
    
    private:
    HexPlayer &bluePlayer;
    HexPlayer &redPlayer;
    int nPlayers;           // players already registered
};

// ----------------------------------------------------------------------------
// define some error conditions
// ----------------------------------------------------------------------------
typedef enum enumHexGameError {
    HEXGAME_ERR_INVALIDCELL = 0x111,
    HEXGAME_ERR_INVALIDSIZE,
    HEXGAME_ERR_OUTOFMEMORY
    };