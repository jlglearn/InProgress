#include <iostream>

void printboard()
{
    int size = 11;
    char sMargin[] = "    ";
    
    std::cout << sMargin;
    for (int i = 0; i < (3 + 1); i++)
        std::cout << " ";    
    for (int i = 0; i < size; i++)
        std::cout << " " << i << " ";
    std::cout << "\n";
    
    std::cout << sMargin;
    for (int i = 0; i < (3 + 1); i++)
        std::cout << " ";        
    for (int i = 0; i < (2 + 3 * size + 1); i++)
        std::cout << "-";        
    std::cout << "\n";
    
    for (int row = 0; row < size; row++)
    {
        std::cout << sMargin;
        for (int s = 0; s < row; s++)
            std::cout << " ";
        std::cout << ((row < 10) ? "  " : " ") << row << " " << "\\ ";
        for (int col = 0; col < size; col++)
            std::cout << " . ";
        std::cout << " \\ " << row << "\n";
    }
    
    std::cout << sMargin;
    for (int i = 0; i < (size + 3 + 1); i++)
        std::cout << " ";
    for (int i = 0; i < (2 + 3 * size + 1); i++)
        std::cout << "-";
    std::cout << "\n";   

    std::cout << sMargin;
    for (int i = 0; i < (size + 3 + 1 + 2); i++)
        std::cout << " ";
    for (int i = 0; i < size; i++)
        std::cout << " " << i << " ";
    std::cout << "\n";
}

int main(int argc, char *argv[])
{
    printboard();
    return 0;
}