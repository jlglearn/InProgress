#include <iostream>
#include <cstring>

void printBoard(int size)
{
    std::string s = "";
    std::string sMargin = "      ";
    std::string sShift  = "  ";

    // ------------ board header ----------------------------------
    std::cout << "\n\n" << sMargin
              << "BLUE moves left to right, RED moves bottom to top\n\n";
    
    // ------------ column header ---------------------------------
    
    int margin = ((4 * size) - strlen("RED GOAL")) / 2;
    
    s = "";
    for (int i = 0; i < margin; i++)
        s = s + " ";
        
    std::cout << "\n\n" << sMargin;    
    std::cout << s << "RED GOAL\n";
    
    std::cout << sMargin;
    for (int i = 0; i < size; i++)
    {
        std::cout << " "
                  << ((i < 10) ? " " : "")
                  << i
                  << " ";
    }
    std::cout << "\n";
    
    // ------------ top border ------------------------------------
    std::cout << sMargin << "  ";
    
    for (int i = 0; i < size; i++)
        std::cout << "----";
        
    std::cout << "\n";    
    
    // ------------ board rows ------------------------------------
    
    int k = size / 2;
    
    s = "";
    for (int i = 0; i < size; i++)
    {
        std::cout << s
                  << ((i == k) ? "BLUE" : 
                      ((i == (k + 1)) ? "HOME" : "    " ))
                  << "  "
                  << ((i < 10) ? " " : "")
                  << i
                  << " ";
                  
        for (int j = 0; j < size; j++)
        {
            std::cout << "  "
                      << "."        // chip
                      << " ";
        }
        
        std::cout << " " 
                  << i << "   "
                  << ((i == k) ? "BLUE" :
                      ((i == (k + 1)) ? "GOAL" : ""))
                  << "\n";
        s = s + sShift;
    }
    
    // ------------ bottom border -----------------------------------
    std::cout << sMargin << s << sShift;
    
    for (int i = 0; i < size; i++)
        std::cout << "----";
        
    std::cout << "\n";  
    
    // ------------ column footer -----------------------------------
    std::cout << sMargin << s << "   ";              
    for (int i = 0; i < size; i++)
    {
        std::cout << " "
                  << ((i < 10) ? " " : "")
                  << i
                  << " ";
    }
    std::cout << "\n";    
    
    std::cout << sMargin << s << "   ";
    
    s = "";
    for (int i = 0; i < margin; i++)
        s = s + " ";
        
    std::cout << s << "RED HOME\n\n\n";    
    
}

int main(int argc, char *argv[])
{
    printBoard(11);
    return 0;
}