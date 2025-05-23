#include "board.hpp"
#include <iostream>
#include <string>

using namespace std;

int main() {
    Board board;
    // std::string move;
    // bool result;

    // while(true) {
    //     board.printBoard();
    //     cout << "Enter move: ";
    //     cin >> move;
    //     if(move=="unmake") board.unmakeMove();
    //     else {
    //         result = board.makeMove(move);
    //         if(!result) cout << "Invalid move..." << endl;
    //     }
    // }
    
    board.printBoard();
    
    std::vector<std::string> moves = board.generateMoves();

    for(size_t i = 0; i < moves.size(); i++){
        std::cout << moves[i] << std::endl;
    }
    
    return 0;
}
