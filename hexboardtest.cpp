#include <iostream>
#include <sstream>
#include "hexboard.h"


static void readMove(unsigned int &row, unsigned int &col)
{
    while (true)
    {
        std::string s;
        getline(std::cin, s);
        
        std::istringstream ss(s);
        int t;
        ss >> t;
        
        if (ss.fail() || ss.eof())
        {
            std::cout << s << " is not a valid move.  Please try again: ";
            continue;
        }
        
        row = t;
        
        ss >> t;
        
        if (ss.fail())
        {
            std::cout << s << " is not a valid move.  Please try again: ";
            continue;
        }
        
        col = t;
        break;
    }
}

int main(int argc, char *argv[])
{

    HexBoard board(5);
    HexGameIO gameIO;
    HexColor turns[2] = {HEXBLUE, HEXRED};
    int iTurn = 0;
    
    for (int iTurn = 0; ; iTurn++)
    {
        HexColor turn = turns[iTurn % 2];
        unsigned int row, col;
        
        gameIO.PrintBoard(board);
        
        while (true)
        {
            gameIO.Prompt(turn);
        
            readMove(row, col);
        
            HexMoveResult result = board.SetColor(row, col, turn);
            
            gameIO.MoveFeedback(result, turn, row, col);
            
            if (result == HEXMOVE_OK) break;
        }
        
        HexColor winner = board.Winner();
        
        if (winner != HEXBLANK)
        {
            gameIO.AnnounceWinner(winner);
            break;
        }
    }    
}