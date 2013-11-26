#include <iostream>
#include <sstream>
#include <string>
#include "hexboard.h"
#include "hexmcplayer.hpp"

static int readInt(const char *prompt, int minVal, int maxVal)
{
    int t;
    while (true)
    {
        std::cout << prompt;
        
        std::string s;
        getline(std::cin, s);
        
        std::istringstream ss(s);
        ss >> t;
        
        if (ss.fail())
        {
            std::cout << s << "is not a valid input.  Please try again.\n";
            continue;
        }
        
        if ((t < minVal) || (t > maxVal))
        {
            std::cout << t << "is not valid.  Please enter a value in the range (" << minVal << "-" << maxVal <<")\n";
            continue;
        }        
        return t;
    }
}

int main(int argc, char *argv[])
{
    // first ask user to enter board size
    const char *sizePrompt = "Please enter the board size (3-15): ";
    int size = readInt(sizePrompt, HEXMINSIZE, HEXMAXSIZE);
    int redWins = 0;
    int blueWins = 0;
    
    HexMC1Player *pBlue = new HexMC1Player;
    HexMC2Player *pRed  = new HexMC2Player;
    
    for (int iTrial = 0; iTrial < 100; iTrial++)
    {    
        HexGame game(size);
        game.RegisterPlayer(pBlue, HEXBLUE);
        game.RegisterPlayer(pRed, HEXRED);
        HexColor winner = game.Play(HEXBLANK);
        
        if (winner == HEXBLUE)
            blueWins++;
        else
            redWins++;      
    }
    
    std::cout << "Blue (win-maximizing strategy): " << blueWins << "\n"
              << "Red (loss-maximizing strategy): " << redWins << "\n";
}