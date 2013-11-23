void HexPlayer::Move(HexBoard &board, HexColor turn, int &row, int &col)
{
    // retrieve blank cells
    HexPositionSet hps;
    board.AllCells(hps, HEXBLANK);
    int nCells     = hps.size();
    int nBestScore = -1;
    int iBestCell  = -1;
    
    HexColor turns[2] = {turn, ((turn == HEXBLUE) ? HEXRED : HEXBLUE)}; // remember order of turns
    
    for (int iCell = 0; iCell < nCells; iCell++)
    {
        // make a working copy of the set of free cells
        HexPositionSet hpsCopy(hps);
        
        // put away current cell by exchanging with last cell
        swapEntries(hpsCopy, iCell, nCells - 1);
                
        // repeat experiment nTrials times
        int nWins = 0;
        for (int iTrial = 0; iTrial < nTrials; iTrial++)
        {
            // shuffle first hps.size()-1 entries to randomize turns
            shuffleEntries(hpsCopy, 0, nCells - 1);
            
            // take turns until a player wins
            for (int iMove = 0; ; iMove++)
            {
                HexMoveResult result;
                HexColor thisTurn = turns[iMove % 2];
                
                if (iMove == 0)
                {
                    // first play the cell we are evaluating
                    result = board.Move(hpsCopy[nCells-1].row, hpsCopy[nCells-1].col, turn);
                }
                else
                {
                    // afterward, play each (shuffled) cell sequentially
                    result = board.Move(hpsCopy[i-1].row, hpsCopy[i-1].col, thisTurn);
                }
                
                if (result == HEXMOVE_WINNER)
                {
                    if (thisTurn == turn)
                    {
                        // we won, record win to compute stats
                        nWins++;
                    }
                    break;  // break out of turns loop, continue with trials loop
                }
            }
        }
        
        if (nWins > nBestScore)
        {
            nBestScore = nWins;
            iBestCell = iCell;
        }
    }
    
    // play the cell with best score
    row = hps[iBestCell].row;
    col = hps[iBestCell].col;    
}