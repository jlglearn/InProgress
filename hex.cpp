#include <iostream>
#include "hexboard.h"

void GetMove(HexBoard &Game, HexBoardIO &GameIO, HexBoardColor turn, int &row, int &col)
{
}

int main(int argc, char *argv[])
{
    HexBoard Game(11);
    std::cout << "main(): board (Game) size: " << Game.dim() << "\n";
    
    HexBoardIO GameIO(Game);
    
    for (bool finished = false; !finished; )
    {
        int row, col;
        
        HexBoardColor turn = Game.Turn();

        GameIO.print();        

        GetMove(Game, GameIO, turn, row, col);
        
        HexMoveResult result = Game.Move(turn, row, col);
        
        GameIO.moveFeedback(result);
        
        if ((result == HEXMOVE_WINNER) || (result == HEXMOVE_DRAW))
            finished = true;        
    }
}