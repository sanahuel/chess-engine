#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include <utility>

class Board {
private:
    char board[8][8];   
    bool whiteToMove;

    bool whiteKingsideCastling;
    bool whiteQueensideCastling;
    bool blackKingsideCastling;
    bool blackQueensideCastling;    

    std::pair<int, int> enPassantWhite;     // rank, file of target = pawn position
    std::pair<int, int> enPassantBlack;     // rank, file of target = pawn position

public:
    Board();            
    void printBoard();

    bool checkMove(int startRank, int startFile, int endRank, int endFile);
    bool checkFreePath(int startRank, int startFile, int endRank, int endFile);
    
    bool makeMove(std::string move);
    bool unmakeMove();

    std::vector<std::string> generateMoves();

    int evaluateBoard();

    bool isWhiteToMove() const {
        return whiteToMove;
    }

    bool isEqual(const Board& other) const;
    bool operator==(const Board& other) const;

    int gameResult();
};

struct Move {
    int startRank, startFile;
    int endRank, endFile;
    char movedPiece;
    char capturedPiece;
    bool wasWhiteTurn;
    bool whiteKingsideCastling;
    bool whiteQueensideCastling;
    bool blackKingsideCastling;
    bool blackQueensideCastling;
    std::pair<int, int> enPassantWhite;
    std::pair<int, int> enPassantBlack;
};

#endif
