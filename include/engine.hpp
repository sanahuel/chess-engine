#include "board.hpp" 

class Engine {
public:
    Board& board;
    Engine(Board& b); 

    std::string findBestMove(int depth);

private:
    int minimax(int depth, int alpha, int beta, bool isMaximizingPlayer);
    static const int INF = 1000000;
};
