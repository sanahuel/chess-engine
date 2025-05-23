#include "board.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>

int main() {
    srand(time(0));

    const int M = 100000; // Number of test iterations
    const int N = 50;  // Number of moves per test

    for (int test = 0; test < M; ++test) {
        Board board;

        // Save initial board state
        Board initialBoard = board;

        std::vector<std::string> moveSequence;

        for (int i = 0; i < N; ++i) {
            std::vector<std::string> moves = board.generateMoves();
            if (moves.empty()) break;

            std::string chosenMove = moves[rand() % moves.size()];
            bool success = board.makeMove(chosenMove);
            if (!success) break;

            moveSequence.push_back(chosenMove);
        }

        //board.printBoard();

        // Unmake all moves
        for (size_t i = 0; i < moveSequence.size(); ++i) {
            bool success = board.unmakeMove();
            assert(success); // Ensure unmake worked
        }

        // Compare board with initialBoard
        if (!(board == initialBoard)) {
            std::cerr << "Board state mismatch after undo in test " << test << std::endl;
            return 1;
        }
    }

    std::cout << "All " << M << " tests passed successfully." << std::endl;
    return 0;
}
