#include "board.hpp"
#include "engine.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>


int main() {
    srand(time(0));

    const int M = 100; // number of games
    const int N = 200; // max number of moves per game
    const int Depth = 3; // engine depth search
    int whiteWins = 0;
    int blackWins = 0;
    int draws = 0;
    int captures = 0;

    for (int game = 0; game < M; game += 1) {
        std::cout << std::endl << std::endl << "- - - - - - - - NEW GAME - - - - - - - -" << std::endl;
        Board board;
        Engine engine(board);
        int result = 0;

        for (int moveCount = 0; moveCount < N; moveCount += 1) {
            std::cout << "- - - - - - - - - - - - - - -" << std::endl;
            board.printBoard();
            std::string move;
            if (board.isWhiteToMove()) {
                std::cout << "- WHITE TO MOVE" << std::endl;
                move = engine.findBestMove(Depth);
            } else {
                std::cout << "- BLACK TO MOVE" << std::endl;
                std::vector<std::string> moves = board.generateMoves();
                if (moves.empty()) break;
                move = moves[rand() % moves.size()];
            }

            std::cout << "About to make move: " << move << std::endl;
            
            std::vector<std::string> moves = board.generateMoves();
            std::cout << "Possible moves according to board:" << std::endl;
            for (const auto& move : moves) {
                std::cout << move << " ";
            }

            bool success = board.makeMove(move);
            std::cout << "Success bool: " << success << std::endl;
            if (!success) {
                std::cerr << "Invalid move made: " << move << " in move " << moveCount+1 << std::endl;
                sleep(60);
                break;
            }

            result = board.gameResult();
            if(result != 0) break;
        }

        // outcome evaluation
        if(result == 1){
            captures++;
            whiteWins++;
        } else if(result == -1){
            blackWins++;
        }else{
            int eval = board.evaluateBoard();
            if (eval > 200) {
                whiteWins++;
            } else if (eval < -200) {
                blackWins++;
            } else {
                draws++;
            }
        }
    }

    std::cout << "Results after " << M << " games:\n";
    std::cout << "White wins (engine): " << whiteWins << "\n";
    std::cout << "White wins by capture: " << captures << "\n";
    std::cout << "Black wins (random): " << blackWins << "\n";
    std::cout << "Draws: " << draws << "\n";

    return 0;
}
