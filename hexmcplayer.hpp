#include <iostream>
#include <cstdlib>
#include <time.h>
#include "hexboard.h"


/* ----------------------------------------------------------------------------
   utility functions:
   shuffleVector(vector, start, end);
        shuffles the given vector in the range start (inclusive) to end (exclusive)
        
   swapEntries(vector, i, j)
        swaps the given entries
   ---------------------------------------------------------------------------- */
template <class T>
static void swapEntries(std::vector<T> &v, unsigned int i, unsigned int j)
{
    T t = v[i];
    v[i] = v[j];
    v[j] = t;
}

template <class T>
static void shuffleVector(std::vector<T> &v, unsigned int start, unsigned int end)
{
    for (unsigned int i = start + 1; i < end; i++)
    {
        unsigned int range = i - start;
        unsigned int j = ((unsigned int) (start + (range * ((double)rand()/RAND_MAX)))) % range;
        swapEntries(v, i, j);
    }
}

/* ============================================================================ *
   HexMC1Player class
   
   Implements an automatic Hex player that uses MonteCarlo simulation to 
   determine next move.
   
   At each move, it picks the cell with larger win count.
   ============================================================================ */
class HexMC1Player : public HexPlayer {
    virtual void Move(HexBoard &board, HexColor turn, unsigned int &row, unsigned int &col);
};


void HexMC1Player::Move(HexBoard &board, HexColor turn, unsigned int &row, unsigned int &col)
{    
    clock_t tStart = clock();
    
    const unsigned int nTrials = 1000;
    
    // keep track of best play and best score
    int bestScore = -1;
    unsigned int bestPlay;
        
    // obtain the currently open cells
    HexCellSet hcs;
    board.GetCells(hcs, HEXBLANK);
    unsigned int nCells = hcs.size();
    
    // shuffle cells to examine them in random order
    shuffleVector(hcs, 0, nCells);   
        
    // precompute the order of moves during simulation
    HexColor turns[2];
    turns[0] = turn;
    turns[1] = ((turn == HEXBLUE) ? HEXRED : HEXBLUE);    
    
    // now evaluate each cell in turn
    for (unsigned int iCell = 0; iCell < nCells; iCell++)
    {        
        // make a working copy of the cell set
        HexCellSet hcsCopy(hcs);
        
        // store away the current cell by swapping with last cell
        swapEntries(hcsCopy, iCell, nCells - 1);
                
        // run MC simulation
        int nWins = 0;
        for (unsigned int iTrial = 0; iTrial < nTrials; iTrial++)
        {        
            // shuffle the first n-1 entries (keeping the current cell safely at the end)
            // to randomize the order in which cells are played in simulation
            shuffleVector(hcsCopy, 0, nCells-1);
            
            // make a copy of the board to run simulated game
            HexBoard boardCopy(board);
            
            // alternate turns until board is full (until no more cells to play)
            for (unsigned int iTurn = 0; iTurn < nCells; iTurn++)
            {
                // for the first move, play the cell currently under evaluation,
                // which is stored at the end of the vector.
                // for subsequent moves, play each cell in succession, starting from 0th cell
                
                unsigned int thisMove = ((iTurn == 0) ? (nCells - 1) : (iTurn - 1));
                HexColor thisColor = turns[iTurn % 2];
                
                boardCopy.SetColor(hcsCopy[thisMove].row, hcsCopy[thisMove].col, thisColor);                
            }
            
            // find out who won this simulated game
            HexColor winner = boardCopy.Winner();
            
            // if we won, update stats
            if (winner == turn)
            {
                nWins++;
            }
        }
        
        if (nWins > bestScore)
        {
            bestScore = nWins;
            bestPlay = iCell;
            
            if (nWins == nTrials)
            {
                // if this cell won all its trials, then play it
                break;
            }            
        }        
    }
    
    // return the best play we found
    clock_t tEnd = clock();
    clock_t tElapsed = (tEnd - tStart) / CLOCKS_PER_SEC;
    std::cout << "Time elapsed: " << tElapsed << "(secs) \n";
    row = hcs[bestPlay].row;
    col = hcs[bestPlay].col;
}


/* ============================================================================ *
   HexMC2Player class
   
   Implements an automatic Hex player that uses MonteCarlo simulation to 
   determine next move.
   
   At each move, it picks the cell with minimum loss count.
   ============================================================================ */
class HexMC2Player : public HexPlayer {
    virtual void Move(HexBoard &board, HexColor turn, unsigned int &row, unsigned int &col);
};


void HexMC2Player::Move(HexBoard &board, HexColor turn, unsigned int &row, unsigned int &col)
{    
    clock_t tStart = clock();
    
    const unsigned int nTrials = 1000;
    
    // keep track of best play and best score
    int bestScore = nTrials * 10;   // a big number, larger than any possible result
    unsigned int bestPlay;
        
    // obtain the currently open cells
    HexCellSet hcs;
    board.GetCells(hcs, HEXBLANK);
    unsigned int nCells = hcs.size();
    
    // shuffle cells to examine them in random order
    shuffleVector(hcs, 0, nCells);   
        
    // precompute the order of moves during simulation
    HexColor turns[2];
    turns[0] = turn;
    turns[1] = ((turn == HEXBLUE) ? HEXRED : HEXBLUE);    
    
    // now evaluate each cell in turn
    for (unsigned int iCell = 0; iCell < nCells; iCell++)
    {        
        // make a working copy of the cell set
        HexCellSet hcsCopy(hcs);
        
        // store away the current cell by swapping with last cell
        swapEntries(hcsCopy, iCell, nCells - 1);
                
        // run MC simulation
        int nLosses = 0;
        for (unsigned int iTrial = 0; iTrial < nTrials; iTrial++)
        {        
            // shuffle the first n-1 entries (keeping the current cell safely at the end)
            // to randomize the order in which cells are played in simulation
            shuffleVector(hcsCopy, 0, nCells-1);
            
            // make a copy of the board to run simulated game
            HexBoard boardCopy(board);
            
            // alternate turns until board is full (until no more cells to play)
            for (unsigned int iTurn = 0; iTurn < nCells; iTurn++)
            {
                // for the first move, play the cell currently under evaluation,
                // which is stored at the end of the vector.
                // for subsequent moves, play each cell in succession, starting from 0th cell
                
                unsigned int thisMove = ((iTurn == 0) ? (nCells - 1) : (iTurn - 1));
                HexColor thisColor = turns[iTurn % 2];
                
                boardCopy.SetColor(hcsCopy[thisMove].row, hcsCopy[thisMove].col, thisColor);                
            }
            
            // find out who won this simulated game
            HexColor winner = boardCopy.Winner();
            
            // if we lost, update stats
            if (winner != turn)
            {
                nLosses++;
            }
        }
        
        if (nLosses < bestScore)
        {
            bestScore = nLosses;
            bestPlay = iCell;
            
            if (nLosses == 0)
            {
                // if this cell never lost, play it
                break;
            }            
        }        
    }
    
    // return the best play we found
    clock_t tEnd = clock();
    clock_t tElapsed = (tEnd - tStart) / CLOCKS_PER_SEC;
    std::cout << "Time elapsed: " << tElapsed << "(secs) \n";
    row = hcs[bestPlay].row;
    col = hcs[bestPlay].col;
}
