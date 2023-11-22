#include <cstdint>
#include <limits>
#include "board.hpp"

class GameManager
{
    GameState board; // tabuleiro atual do jogo

    // função de maximisação
    unsigned char max(BoardArray &boards, uint8_t depth)
    {
        unsigned char index = 0;
        float max = -(std::numeric_limits<float>::infinity());
        for (unsigned char i = 0; i < boards.num_board; i++)
        {
            auto temp = evaluate_mine_max(boards.board[i], depth - 1);
            if (temp > max)
            {
                max = temp;
                index = i;
            }
        }
        return index;
    }
    // função de minimisação
    unsigned char min(BoardArray &boards, uint8_t depth)
    {
        unsigned char index = 0;
        float min = (std::numeric_limits<float>::infinity());
        for (unsigned char i = 0; i < boards.num_board; i++)
        {
            auto temp = evaluate_mine_max(boards.board[i], depth - 1);
            if (temp < min)
            {
                min = temp;
                index = i;
            }
        }
        return index;
    }
    // implementação do mine max
    float evaluate_mine_max(GameState &board, uint8_t depth)
    {
        if (!depth || board.result() != GameResult_NotDone)
            return board.evaluate();
        auto boards = board.calculate_sub_boards();
        unsigned char index = 0;
        if (board.turn == 0)
            index = max(boards, depth);
        else
            index = min(boards, depth);
        board = boards.board[index];
        return board.evaluate();
    }

public:
    // initializa o tabuleiro.
    GameManager();

    // atualiza o tabuleiro usando busca em profundidade como base.
    void dfs_next(uint8_t depth = 6);

    // atualiza o tabuleiro do jogo utilizando minimax.
    void minimax_next(uint8_t depth = 6)
    {
        evaluate_mine_max(board, depth);
    }
};
