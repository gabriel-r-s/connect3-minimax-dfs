#include <cstdint>
#include <limits>
#include "board.hpp"
struct GameManager
{
    GameState board; // tabuleiro atual do jogo

    // implementação do mine max
    float evaluate_mini_max(const GameState &board, uint8_t depth, float alpha = -std::numeric_limits<float>::infinity(), float beta = std::numeric_limits<float>::infinity())
    {
        if (depth == 0 || board.result() != GameResult_NotDone)
            return board.evaluate();
        auto boards = board.calculate_sub_boards();
        for (unsigned char i = 0; i < boards.num_board; i++)
        {
            float score = -evaluate_mini_max(boards.board[i], depth - 1, -beta, -alpha);
            if (score >= beta)
            {
                alpha = score; // we found a good enough score
                break;
            }
            if (score > alpha)
            {
                alpha = score;
            }
        }
        return alpha;
    }

public:
    // atualiza o tabuleiro usando busca em profundidade como base.
    void dfs_next(uint8_t depth = 6);

    // atualiza o tabuleiro do jogo utilizando minimax.
    void minimax_next(uint8_t depth = 6, float alpha = -std::numeric_limits<float>::infinity(), float beta = std::numeric_limits<float>::infinity())
    {
        auto boards=board.calculate_sub_boards();
        unsigned char turn = board.turn;
        for(int i=0; i<boards.num_board; i++)
        {
            float score = evaluate_mini_max(boards.board[i], depth-1, alpha, beta);
            if(turn==0 && score>alpha)
            {
                alpha=score;
                board = boards.board[i];
            }
            else if(turn==1 && score<beta)
            {
                beta=score;
                board= boards.board[i];
            }
        }
    }
};
