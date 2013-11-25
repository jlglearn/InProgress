#ifndef _HEXBOARD_H_
#define _HEXBOARD_H_

#include "mingraph.hpp"


typedef enum enumHexColor {
    HEXNULL, HEXBLANK, HEXBLUE, HEXRED
} HexColor;

typedef enum enumHexBoardError {
    HEXBOARD_ERR_INVALIDSIZE = 0x200,
    HEXBOARD_ERR_INVALIDCELL
} HexBoardError;

typedef enum enumHexGameError {
    HEXGAME_ERR_INVALIDTURN = 0x300,
    HEXGAME_ERR_INVALIDCOLOR
} HexGameError;

typedef enum enumHexMoveResult {
    HEXMOVE_OK,
    HEXMOVE_INVALIDCELL,
    HEXMOVE_INVALIDCOLOR,
    HEXMOVE_OCCUPIED
} HexMoveResult;

const int HEXMINSIZE = 3;
const int HEXMAXSIZE = 15;

/* ============================================================================ *
 * HexBoard class                                                               *
 * ============================================================================ */
 
class HexBoard {
    public:
    HexBoard(unsigned int n);    
    HexMoveResult SetColor(unsigned int row, unsigned int col, HexColor color);
    HexColor GetColor(unsigned int row, unsigned int col);
    unsigned int Size(void);
    HexColor Winner(void);

    private:
    unsigned int size;    
    unsigned int BLUEHOME, BLUEGOAL, REDHOME, REDGOAL;
    
    MinGraph<HexColor> G;    
    
    inline unsigned int cellIndex(unsigned int row, unsigned int col) { return row * size + col; }
    inline unsigned int rowFromIndex(unsigned int index) { return index / size; }
    inline unsigned int colFromIndex(unsigned int index) { return index % size; }    
    void Reset(unsigned int n);

};


/* ============================================================================ *
 * HexGameIO class                                                              *
 * ============================================================================ */
class HexGameIO {
    public:
    char chip(HexColor c);
    const char *name(HexColor c);
    void Prompt(HexColor turn);
    void MoveFeedback(HexMoveResult result, HexColor turn, int row, int col);
    void AnnounceWinner(HexColor winner);
    void PrintBoard(HexBoard &board);
};

#endif