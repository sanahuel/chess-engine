#include "board.hpp"
#include "engine.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(0));

    const int M = 15;  // number of games
    const int N = 200;  // max moves per game
    const int Depth1 = 4; // white engine depth
    const int Depth2 = 2; // black engine depth

    int whiteWins = 0;
    int blackWins = 0;
    int draws = 0;
    int earlyEnds = 0;

    for (int game = 0; game < M; ++game) {
        std::cout << std::endl << std::endl << "- - - - - - - - NEW GAME - - - - - - - -" << std::endl;

        Board board;
        Engine engineWhite(board);
        Engine engineBlack(board);

        int result = 0;

        for (int moveCount = 0; moveCount < N; ++moveCount) {
            std::cout << "- - - - - - - - - - - - - - -" << std::endl;
            board.printBoard();

            std::string move;
            if (board.isWhiteToMove()) {
                std::cout << "- WHITE TO MOVE" << std::endl;
                move = engineWhite.findBestMove(Depth1);
            } else {
                std::cout << "- BLACK TO MOVE" << std::endl;
                move = engineBlack.findBestMove(Depth2);
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
                std::cerr << "Invalid move made: " << move << "\n";
                earlyEnds++;
                break;
            }

            result = board.gameResult();
            if (result != 0) break;
        }

        // outcome evaluation
        if (result == 1) {
            whiteWins++;
        } else if (result == -1) {
            blackWins++;
        } else {
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
    std::cout << "White (depth " << Depth1 << ") wins: " << whiteWins << "\n";
    std::cout << "Black (depth " << Depth2 << ") wins: " << blackWins << "\n";
    std::cout << "Draws: " << draws << "\n";
    std::cout << "Games ended due to illegal moves: " << earlyEnds << "\n";

    return 0;
}
