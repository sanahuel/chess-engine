#include "board.hpp"
#include "engine.hpp"
#include <iostream>

Engine::Engine(Board& b) : board(b) {}

std::string Engine::findBestMove(int depth) {
    bool isMaximizingPlayer = board.isWhiteToMove();
    int bestScore = isMaximizingPlayer ? -INF : INF;
    std::string bestMove;

    auto moves = board.generateMoves();
    for (const auto& move : moves) {
        // Try to make the move; skip if invalid
        if (!board.makeMove(move)) {
            std::cerr << "Invalid root move: " << move << std::endl;
            continue;
        }

        // Fresh alpha/beta for each root move
        int alpha = -INF;
        int beta  =  INF;
        int score = minimax(depth - 1, alpha, beta, !isMaximizingPlayer);

        // Always unmake the move
        if (!board.unmakeMove()) {
            std::cerr << "UnmakeMove failed at root for move: " << move << std::endl;
        }

        // Update best
        if (isMaximizingPlayer) {
            if (score > bestScore) {
                bestScore = score;
                bestMove  = move;
            }
        } else {
            if (score < bestScore) {
                bestScore = score;
                bestMove  = move;
            }
        }
    }

    return bestMove;
}

int Engine::minimax(int depth, int alpha, int beta, bool isMaximizingPlayer) {
    // Terminal or leaf node
    if (depth == 0) {
        return board.evaluateBoard();
    }

    auto moves = board.generateMoves();
    // No moves => checkmate or stalemate
    if (moves.empty()) {
        // if (board.isInCheck(isMaximizingPlayer)) {
        //     // Checkmate: worst for side to move
        //     return isMaximizingPlayer ? -MATE_SCORE : +MATE_SCORE;
        // } else {
        //     // Stalemate: draw
        //     return 0;
        // }
        return 0;
    }

    if (isMaximizingPlayer) {
        int maxEval = -INF;
        for (const auto& move : moves) {
            if (!board.makeMove(move)) continue;

            int eval = minimax(depth - 1, alpha, beta, false);

            if (!board.unmakeMove()) {
                std::cerr << "UnmakeMove failed in max at move: " << move << std::endl;
            }

            maxEval = std::max(maxEval, eval);
            alpha   = std::max(alpha, eval);
            if (alpha >= beta) break; // Beta cutoff
        }
        return maxEval;
    } else {
        int minEval = INF;
        for (const auto& move : moves) {
            if (!board.makeMove(move)) continue;

            int eval = minimax(depth - 1, alpha, beta, true);

            if (!board.unmakeMove()) {
                std::cerr << "UnmakeMove failed in min at move: " << move << std::endl;
            }

            minEval = std::min(minEval, eval);
            beta    = std::min(beta, eval);
            if (alpha >= beta) break; // Alpha cutoff
        }
        return minEval;
    }
}
