#include "board.hpp" 
#include <iostream>
#include <unordered_map>
#include <stack>
#include <vector>

std::unordered_map<char, std::string> pieceToUnicode = {
    {'K', "♔"}, {'Q', "♕"}, {'R', "♖"}, {'B', "♗"}, {'N', "♘"}, {'P', "♙"},
    {'k', "♚"}, {'q', "♛"}, {'r', "♜"}, {'b', "♝"}, {'n', "♞"}, {'p', "♟"},
    {'.', "·"} 
};

std::unordered_map<char, int> letterToIndex = {
    {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7},
};

std::unordered_map<int, char> indexToLetter = {
    {0, 'a'}, {1, 'b'}, {2, 'c'}, {3, 'd'}, {4, 'e'}, {5, 'f'}, {6, 'g'}, {7, 'h'},
};

// for move generation: piece -> {{possible moves in board indices}, max distance}
std::unordered_map<char, std::pair<std::vector<std::vector<int>>, int>> movePatterns = {
    {'R', {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}, 8}},
    {'B', {{{1, 1}, {-1, 1}, {-1, -1}, {1, -1}}, 8}},
    {'N', {{{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}}, 1}}, 
    {'Q', {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}}, 8}}, 
    {'K', {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}}, 1}},
    {'P', {{{1, 0}}, 1}},
    {'p', {{{-1, 0}}, 1}}  
};

// for board evaluation: piece -> score by board position
// score boards and piece values are the same that in sunfish
std::unordered_map<char, std::array<std::array<int, 8>, 8>> scoreBoards = {
    {'R', {{
        { 35,  29,  33,   4,  37,  33,  56,  50},
        { 55,  29,  56,  67,  55,  62,  34,  60},
        { 19,  35,  28,  33,  45,  27,  25,  15},
        {  0,   5,  16,  13,  18,  -4,  -9,  -6},
        {-28, -35, -16, -21, -13, -29, -46, -30},
        {-42, -28, -42, -25, -25, -35, -26, -46},
        {-53, -38, -31, -26, -29, -43, -44, -53},
        {-30, -24, -18,   5,  -2, -18, -31, -32}
    }}},
    {'B', {{
        {-59, -78, -82, -76, -23, -107, -37, -50},
        {-11,  20,  35, -42, -39,   31,   2, -22},
        { -9,  39, -32,  41,  52,  -10,  28, -14},
        { 25,  17,  20,  34,  26,   25,  15,  10},
        { 13,  10,  17,  23,  17,   16,   0,   7},
        { 14,  25,  24,  15,   8,   25,  20,  15},
        { 19,  20,  11,   6,   7,    6,  20,  16},
        { -7,   2, -15, -12, -14,  -15, -10, -10}
    }}},
    {'N', {{
        {-66, -53, -75, -75, -10, -55, -58, -70},
        { -3,  -6, 100, -36,   4,  62,  -4, -14},
        { 10,  67,   1,  74,  73,  27,  62,  -2},
        { 24,  24,  45,  37,  33,  41,  25,  17},
        { -1,   5,  31,  21,  22,  35,   2,   0},
        {-18,  10,  13,  22,  18,  15,  11, -14},
        {-23, -15,   2,   0,   2,   0, -23, -20},
        {-74, -23, -26, -24, -19, -35, -22, -69}
    }}},
    {'Q', {{
        {  6,   1,  -8,-104,  69,  24,  88,  26},
        { 14,  32,  60, -10,  20,  76,  57,  24},
        { -2,  43,  32,  60,  72,  63,  43,   2},
        {  1, -16,  22,  17,  25,  20, -13,  -6},
        {-14, -15,  -2,  -5,  -1, -10, -20, -22},
        {-30,  -6, -13, -11, -16, -11, -16, -27},
        {-36, -18,   0, -19, -15, -15, -21, -38},
        {-39, -30, -31, -13, -31, -36, -34, -42}
    }}},
    {'K', {{
        {  4,  54,  47, -99, -99,  60,  83, -62},
        {-32,  10,  55,  56,  56,  55,  10,   3},
        {-62,  12, -57,  44, -67,  28,  37, -31},
        {-55,  50,  11,  -4, -19,  13,   0, -49},
        {-55, -43, -52, -28, -51, -47,  -8, -50},
        {-47, -42, -43, -79, -64, -32, -29, -32},
        { -4,   3, -14, -50, -57, -18,  13,   4},
        { 17,  30,  -3, -14,   6,  -1,  40,  18}
    }}},
        {'P', {{
        {  0,   0,   0,   0,   0,   0,   0,   0},
        { 78,  83,  86,  73, 102,  82,  85,  90},
        {  7,  29,  21,  44,  40,  31,  44,   7},
        {-17,  16,  -2,  15,  14,   0,  15, -13},
        {-26,   3,  10,   9,   6,   1,   0, -23},
        {-22,   9,   5, -11, -10,  -2,   3, -19},
        {-31,   8,  -7, -37, -36, -14,   3, -31},
        {  0,   0,   0,   0,   0,   0,   0,   0}
    }}},
    {'p', {{
        {  0,   0,   0,   0,   0,   0,   0,   0},
        {-31,   3, -14, -36, -37,  -7,   8, -31},
        {-19,   3,  -2, -10, -11,   5,   9, -22},
        {-23,   0,   1,   6,   9,  10,   3, -26},
        {-13,  15,   0,  14,  15,  -2,  16, -17},
        {  7,  44,  31,  40,  44,  21,  29,   7},
        { 90,  85,  82, 102,  73,  86,  83,  78},
        {  0,   0,   0,   0,   0,   0,   0,   0}
    }}},
};

std::unordered_map<char, int> pieceValues = {
    {'K', 60000}, {'Q', 929}, {'R', 479}, {'B', 320}, {'N', 280}, {'P', 100}
};

std::stack<Move> moveHistory;

Board::Board() {
    // the board is vertically flipped to make the file index and the file notation coincide
    char initialBoard[8][8] = {
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    };

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board[i][j] = initialBoard[i][j];

    whiteToMove = true;

    whiteKingsideCastling = true;
    whiteQueensideCastling = true;
    blackKingsideCastling = true;
    blackQueensideCastling = true;

    enPassantWhite = {-1, -1};
    enPassantBlack = {-1, -1};
}

void Board::printBoard() {
    std::cout << std::endl;
    
    for (int i = 7; i >= 0; i--) { // files are inverted back for display
        std::cout << i+1 << "  ";
        for (int j = 0; j < 8; j++) {
            std::cout << pieceToUnicode[board[i][j]]<< " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "   a b c d e f g h" << std::endl;
    std::cout << std::endl;
}

bool Board::checkFreePath(int startRank, int startFile, int endRank, int endFile){
    int minFile = std::min(endFile,startFile);
    int maxFile = std::max(endFile,startFile);
    int minRank = std::min(endRank,startRank);
    int maxRank = std::max(endRank,startRank);
    if(minFile == maxFile){
        for(int i = (minRank + 1); i < maxRank; i++){
            if(board[i][startFile] != '.') return false;
        }
    } else if(minRank == maxRank){
        for(int i = (minFile + 1); i < maxFile; i++){
            if(board[startRank][i] != '.') return false;
        }
    } else {
        int i = (minRank + 1);
        int j = (minFile + 1);
        while((i < maxRank) && (j < maxFile)){
            if(board[i][j] != '.') return false;
            i ++;
            j ++;
        }
    }

    return true;
}

bool Board::checkMove(int startRank, int startFile, int endRank, int endFile){
    //  check that destination is free or has oponent piece
    if((board[endRank][endFile] != '.') && (isupper(board[startRank][startFile]) == isupper(board[endRank][endFile]))) return false;

    // check move by piece type
    int fileIncrement = endFile - startFile;
    int rankIncrement = endRank - startRank;
    char piece = board[startRank][startFile];

    if(piece == 'R' || piece == 'r'){
        if(((fileIncrement != 0) != (rankIncrement != 0)) && checkFreePath(startRank, startFile, endRank, endFile)) return true;
    } else if(piece == 'B' || piece == 'b'){
        if((abs(fileIncrement) == abs(rankIncrement)) && checkFreePath(startRank, startFile, endRank, endFile)) return true;
    } else if(piece == 'N' || piece == 'n'){
        if((abs(fileIncrement) == 2) && (abs(rankIncrement) == 1)) return true;
        if((abs(fileIncrement) == 1) && (abs(rankIncrement) == 2)) return true;
    } else if(piece == 'Q' || piece == 'q'){
        if(((fileIncrement != 0) != (rankIncrement != 0)) && checkFreePath(startRank, startFile, endRank, endFile)) return true;
        if((abs(fileIncrement) == abs(rankIncrement)) && checkFreePath(startRank, startFile, endRank, endFile)) return true;
    } else if(piece == 'K' || piece == 'k'){
        if((abs(fileIncrement) <= 1) && (abs(rankIncrement) <= 1)) return true;
    } else if(piece == 'P'){
        if((startRank == 1) && (endRank <= (startRank + 2)) && (endFile == startFile) && (board[endRank][endFile] == '.')) return true;                     // initial move
        if((endRank == (startRank + 1)) && (endFile == startFile) && (board[endRank][endFile] == '.')) return true;                                         // one forward with no capture
        if((endRank == (startRank + 1)) && ((endFile == (startFile + 1)) || (endFile == (startFile - 1))) && (board[endRank][endFile] != '.')) return true; // capture in diagonal
        if(endRank == (enPassantBlack.first + 1) && endFile == enPassantBlack.second) return true;                                                          // en passant capture
    } else if(piece == 'p'){
        if((startRank == 6) && (endRank >= (startRank - 2)) && (endFile == startFile) && (board[endRank][endFile] == '.')) return true;                     // initial move
        if((endRank == (startRank - 1)) && (endFile == startFile) && (board[endRank][endFile] == '.')) return true;                                         // one forward with no capture
        if((endRank == (startRank - 1)) && ((endFile == (startFile + 1)) || (endFile == (startFile - 1))) && (board[endRank][endFile] != '.')) return true; // capture in diagonal
        if(endRank == (enPassantWhite.first - 1) && endFile == enPassantWhite.second) return true;                                                          // en passant capture

    }

    return false;
}

bool Board::makeMove(std::string move){
    // move in format (start)(finish) -> b2b3, g8f6, etc.
    if(move.length() != 4) return false;

    int startFile = letterToIndex[move[0]];
    int startRank = move[1] - '0' - 1;
    int endFile = letterToIndex[move[2]];
    int endRank = move[3] - '0'  - 1;

    if(board[startRank][startFile] == '.') return false;

    // check if piece has correct color for turn    
    bool isPieceWhite = isupper(board[startRank][startFile]);
    if(whiteToMove != isPieceWhite) return false;

    // check if move is legal
    bool isLegalMove = checkMove(startRank, startFile, endRank, endFile);
    if(!isLegalMove) return false;

    // detect promotion
    bool isPromotion = (move.size() == 5);

    // update move history
    Move currentMove = {
        startRank, startFile,
        endRank, endFile,
        board[startRank][startFile],    //movedPiece
        board[endRank][endFile],        //capturedPiece
        whiteToMove,
        whiteKingsideCastling,
        whiteQueensideCastling,
        blackKingsideCastling,
        blackQueensideCastling,    
        enPassantWhite,
        enPassantBlack
    };

    moveHistory.push(currentMove);

    // update board
    board[endRank][endFile] = board[startRank][startFile];
    board[startRank][startFile] = '.';
    
    if(isPromotion) {
        char promotionPiece  = move[4];
        board[endRank][endFile] = promotionPiece;
    }

    // update rook if castling
    if(currentMove.movedPiece == 'K' || currentMove.movedPiece == 'k'){
        int movedFiles = endFile - startFile;
        // kingside 
        if (movedFiles == +2) {
            board[endRank][5] = board[endRank][7];
            board[endRank][7] = '.';
        }
        // queenside 
        else if (movedFiles == -2) {
            board[endRank][3] = board[endRank][0];
            board[endRank][0] = '.';
        }
    }


    // check en passant capture
    if(currentMove.movedPiece == 'P' && currentMove.endRank == (enPassantBlack.first + 1) && currentMove.endFile == enPassantBlack.second){
        board[enPassantBlack.first][enPassantBlack.second] = '.';
    } else if(currentMove.movedPiece == 'p' && currentMove.endRank == (enPassantWhite.first - 1) && currentMove.endFile == enPassantWhite.second){
        board[enPassantWhite.first][enPassantWhite.second] = '.';
    }

    // update castling rights
    if(whiteToMove){
        if(whiteKingsideCastling){
            if(currentMove.movedPiece == 'K' || (currentMove.movedPiece == 'R' && currentMove.startFile == 7 && currentMove.startRank == 7)){
                whiteKingsideCastling = false;
            }
        }  
        if(whiteQueensideCastling){
            if(currentMove.movedPiece == 'K' || (currentMove.movedPiece == 'R' && currentMove.startFile == 0 && currentMove.startRank == 7)){
                whiteQueensideCastling = false;
            }
        }
    } else {
        if(blackKingsideCastling){
            if(currentMove.movedPiece == 'k' || (currentMove.movedPiece == 'r' && currentMove.startFile == 7 && currentMove.startRank == 0)){
                blackKingsideCastling = false;
            }
        }
        if(blackQueensideCastling){
            if(currentMove.movedPiece == 'k' || (currentMove.movedPiece == 'r' && currentMove.startFile == 0 && currentMove.startRank == 0)){
                blackQueensideCastling = false;
            }
        }
    }

    // update castling rights when a rook is captured
    if(!whiteToMove && currentMove.capturedPiece == 'R' && currentMove.endRank == 7 && currentMove.endFile == 7) whiteKingsideCastling = false;
    if(!whiteToMove && currentMove.capturedPiece == 'R' && currentMove.endRank == 7 && currentMove.endFile == 0) whiteQueensideCastling = false;
    if(whiteToMove && currentMove.capturedPiece == 'r' && currentMove.endRank == 0 && currentMove.endFile == 7) blackKingsideCastling = false;
    if(whiteToMove && currentMove.capturedPiece == 'r' && currentMove.endRank == 0 && currentMove.endFile == 0) blackQueensideCastling = false;

    // update en passant targets
    if(whiteToMove){
        enPassantBlack = {-1, -1};
        if(currentMove.movedPiece == 'P' && currentMove.startRank == 1 && currentMove.endRank == 3){
                enPassantWhite = {currentMove.endRank, currentMove.endFile};
            }
    } else {
        enPassantWhite = {-1, -1};
        if(currentMove.movedPiece == 'p' && currentMove.startRank == 6 && currentMove.endRank == 4){
                enPassantBlack = {currentMove.endRank, currentMove.endFile};
        }
    }

    // update turn
    whiteToMove = !whiteToMove;

    return true;
}

bool Board::unmakeMove(){
    if (moveHistory.empty()) return false;

    Move lastMove = moveHistory.top();
    moveHistory.pop();

    board[lastMove.startRank][lastMove.startFile] = lastMove.movedPiece;
    board[lastMove.endRank][lastMove.endFile] = lastMove.capturedPiece;
    
    // en passant pawn restoration
    if(lastMove.movedPiece == 'P' &&
       lastMove.endRank == (lastMove.enPassantBlack.first + 1) &&
       lastMove.endFile == lastMove.enPassantBlack.second &&
       lastMove.capturedPiece == '.') {
        // restore black pawn
        board[lastMove.enPassantBlack.first][lastMove.enPassantBlack.second] = 'p';
    }
    else if(lastMove.movedPiece == 'p' &&
            lastMove.endRank == (lastMove.enPassantWhite.first - 1) &&
            lastMove.endFile == lastMove.enPassantWhite.second &&
            lastMove.capturedPiece == '.') {
        // restore white pawn
        board[lastMove.enPassantWhite.first][lastMove.enPassantWhite.second] = 'P';
    }

    whiteToMove = lastMove.wasWhiteTurn;

    whiteKingsideCastling = lastMove.whiteKingsideCastling;
    whiteQueensideCastling = lastMove.whiteQueensideCastling;
    blackKingsideCastling = lastMove.blackKingsideCastling;
    blackQueensideCastling = lastMove.blackQueensideCastling;
    
    enPassantWhite = lastMove.enPassantWhite;
    enPassantBlack = lastMove.enPassantBlack;

    return true;
}

std::vector<std::string>  Board::generateMoves(){
    std::vector<std::string> moves;
    std::pair<std::vector<std::vector<int>>, int>  possibleMoves;
    int distanceMoved;
    int fileIncrement;
    int rankIncrement;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(board[i][j] == '.') continue;
            // only for the color that has the turn to move
            if(whiteToMove != (isupper(board[i][j]) != 0)) continue;
            
            // black pawns are the only pieces affected by color in the movePatterns
            if(board[i][j] == 'p') possibleMoves = movePatterns['p'];
            else possibleMoves = movePatterns[toupper(board[i][j])];

            auto directions = possibleMoves.first;
            int maxDistance = possibleMoves.second;
            
            // standard movements
            for(size_t k = 0; k < directions.size(); k++){
                distanceMoved = 1;
                fileIncrement = 0;
                rankIncrement = 0;

                while(distanceMoved <= maxDistance){
                    fileIncrement = directions[k][1]*distanceMoved;
                    rankIncrement = directions[k][0]*distanceMoved;

                    // detect outside of bord moves
                    if((i + rankIncrement) > 7 || (j + fileIncrement) > 7 || (i + rankIncrement) < 0 || (j + fileIncrement) < 0){
                        break;
                    }

                    bool isLegal = checkMove(i, j, i + rankIncrement, j + fileIncrement);
                    if(isLegal){
                        std::string move = indexToLetter[j] + std::to_string(i+1) + indexToLetter[j + fileIncrement] + std::to_string(i + 1 + rankIncrement);
                        // handle pawn promotion
                        if((board[i][j] == 'P') && (i + rankIncrement == 7)){
                            for (char promo : {'Q','R','B','N'}) {
                                moves.push_back(move + promo);
                            }
                        } else if((board[i][j] == 'P') && (i + rankIncrement == 0)){
                            for (char promo : {'q','r','b','n'}) {
                                moves.push_back(move + promo);
                            }
                        } else {
                            // normal moves
                            moves.push_back(move);
                        }
                        distanceMoved ++;
                    }else{
                        std::string move = indexToLetter[j] + std::to_string(i+1) + indexToLetter[j + fileIncrement] + std::to_string(i + 1 + rankIncrement);
                        break;
                    }
                }
            }

            // pawn double move
            if((board[i][j] == 'p') && (i == 6)){
                bool isLegal = checkMove(i, j, i - 2, j);
                if(isLegal){
                    std::string move = indexToLetter[j] + std::to_string(i+1) + indexToLetter[j] + std::to_string(i + 1 - 2);
                    moves.push_back(move);
                }
            } else if((board[i][j] == 'P') && (i == 1)){
                bool isLegal = checkMove(i, j, i + 2, j);
                if(isLegal){
                    std::string move = indexToLetter[j] + std::to_string(i+1) + indexToLetter[j] + std::to_string(i + 1 + 2);
                    moves.push_back(move);
                }
            }

            // castling
            if(whiteToMove){
                if(whiteKingsideCastling && board[7][4]=='K' && board[7][7]=='R' && board[7][5] == '.' && board[7][6] == '.'){
                    std::string move = "e1g1";
                    moves.push_back(move);
                }
                if(whiteQueensideCastling && board[7][4]=='K' && board[7][0]=='R'  && board[7][3] == '.' && board[7][2] == '.' && board[7][1]=='.'){
                    std::string move = "e1c1";
                    moves.push_back(move);
                }
            } else{
                if(blackKingsideCastling && board[0][4]=='k' && board[0][7]=='r'  && board[0][5] == '.' && board[0][6] == '.'){
                    std::string move = "e8g8";
                    moves.push_back(move);
                }
                if(blackQueensideCastling && board[0][4]=='k' && board[0][0]=='r'  && board[0][3] == '.' && board[0][2] == '.' && board[0][1]=='.'){
                    std::string move = "e8c8";
                    moves.push_back(move);
                }
            }

            // en passant capture
            if((board[i][j] == 'p') && (i == enPassantWhite.first) && (j == (enPassantWhite.second + 1) || j == (enPassantWhite.second - 1))){
                std::string move = indexToLetter[j] + std::to_string(i+1) + indexToLetter[enPassantWhite.second] + std::to_string((i + 1) - 1);
                moves.push_back(move);
            } else if((board[i][j] == 'P') && (i == enPassantBlack.first) && (j == (enPassantBlack.second + 1) || j == (enPassantBlack.second - 1))){
                std::string move = indexToLetter[j] + std::to_string(i+1) + indexToLetter[enPassantBlack.second] + std::to_string((i + 1) + 1);
                moves.push_back(move);
            }

        }
    }
    
    return moves;
}

int Board::evaluateBoard(){
    int score = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            char piece = board[i][j];
            if(piece != '.'){
                
                if(isupper(piece)) {
                    score += pieceValues[toupper(piece)] + scoreBoards[toupper(piece)][7-i][7-j];   //reversed for white because sunfish uses the board without a vertical flip
                    }
                else {
                    score -= (pieceValues[toupper(piece)] + scoreBoards[toupper(piece)][i][j]);
                    };
            }
        }
    }

    return score;
}

// Compare boards -> Tests
bool Board::isEqual(const Board& other) const {
    // Compare board pieces
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (board[i][j] != other.board[i][j])
                return false;

    // Compare player turn
    if (whiteToMove != other.whiteToMove)
        return false;

    // Compare castling rights
    if (whiteKingsideCastling != other.whiteKingsideCastling ||
        whiteQueensideCastling != other.whiteQueensideCastling ||
        blackKingsideCastling != other.blackKingsideCastling ||
        blackQueensideCastling != other.blackQueensideCastling)
        return false;

    // Compare en passant squares
    if (enPassantWhite != other.enPassantWhite ||
        enPassantBlack != other.enPassantBlack)
        return false;

    return true;
}

bool Board::operator==(const Board& other) const {
    return isEqual(other); // Call your existing logic
}

int Board::gameResult(){
    bool whiteKingFound = false;
    bool blackKingFound = false;

    for (int r = 0; r < 8; ++r) {
      for (int f = 0; f < 8; ++f) {
        char p = board[r][f];

        if(p == 'K') whiteKingFound = true;
        else if(p == 'k') blackKingFound = true;
      }
    }

    if (!whiteKingFound) return -1; //black side win
    if (!blackKingFound) return 1;  //white side win
    return 0;  // ongoing

}