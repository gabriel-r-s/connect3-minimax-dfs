#include <iostream>
using std::cout;
#include <math.h>

// macro de avaliação de quadrados
#define square(i, j) 1000 / (fabs(2.5 - i) * 10 + fabs(3 - j) * 10)

enum GameResult
{
    GameResult_Player1_Wins,
    GameResult_Player2_Wins,
    GameResult_Draw,
    GameResult_NotDone
};
// dimenções do tabuleiro
#define width 7
#define height 6
// quantos items devem ser conectados
#define connect_win 4
struct BoardArray; // forward declaration
struct GameState
{
    // array de colunas
    unsigned char cols[width];
    // numero de elementos em cada coluna
    unsigned char len[width];
    unsigned char turn = 0; // indica quem deve ser o próximo a jogar.

    // imprime o tabuleiro
    void print()
    {
        for (unsigned char i = height - 1; i < height; i--) // geramos um underflow.
        {
            for (unsigned j = 0; j < width; j++)
                cout << (i >= len[j] ? "*" : cols[j] & (1 << i) ? "2"
                                                                : "1");
            cout << '\n';
        }
    }

    // faz  a jogada na coluna e retorna o novo tabuleiro (não verifica validade)
    GameState play(unsigned col)
    {
        GameState final = *this;
        if (col < width)
        {
            final.cols[col] += (turn << len[col]);
            final.len[col]++;
            final.turn = final.turn ? 0 : 1;
        }
        return final;
    }

    // faz a jogada na coluna especificada e atualiza o tabuleiro se for válido
    void try_play(unsigned char col)
    {
        GameState board = play(col);
        if (board.is_valid())
            *this = board;
    }
    // verifica se é vitória do player1, player2
    GameResult result()
    {
        return (get_h_lines(connect_win, 0) || get_v_lines(connect_win, 0) || get_d_lines(connect_win, 0)) ? GameResult_Player1_Wins : (get_h_lines(connect_win, 1) || get_v_lines(connect_win, 1) || get_d_lines(connect_win, 1)) ? GameResult_Player2_Wins
                                                                                                                                   : is_full()                                                                                     ? GameResult_Draw
                                                                                                                                                                                                                                   : GameResult_NotDone;
    }

    // verifica a validade do tabuleiro
    bool is_valid()
    {
        for (int i = 0; i < width; i++)
            if (len[i] > height)
                return false;
        return true;
    }
    // verifica se o tabuleiro está cheio
    bool is_full()
    {
        for (int i = 0; i < width; i++)
            if (len[i] < height)
                return false;
        return true;
    }
    // retorna o valor dada uma linha e coluna
    unsigned char get_position(unsigned char line, unsigned char col)
    {
        return (cols[col] >> line) & 1;
    }
    // verifica se existe uma linha diagonal partindo do ponto especificado (cima direita)
    bool is_dru_line(unsigned char line, unsigned char col, unsigned line_size, unsigned char player)
    {
        bool final = true;
        if (width < col + line_size)
            return false;
        for (unsigned char i = line, j = col; i < line + line_size; i++, j++)
        {
            if (i >= len[j] || get_position(i, j) != player)
                return false;
        }
        return final;
    }
    // verifica se existe uma linha diagonal partindo do ponto especificado (cima esquerda)
    bool is_dlu_line(unsigned char line, unsigned char col, unsigned line_size, unsigned char player)
    {
        bool final = true;
        if (col < line_size - 1)
            return false;
        for (unsigned char i = line, j = col; i < line + line_size; i++, j--)
        {
            if (i >= len[j] || get_position(i, j) != player)
                return false;
        }
        return final;
    }
    // verifica se existe uma linha vertical partindo do ponto especificado (sempre para cima)
    bool is_v_line(unsigned char line, unsigned char col, unsigned line_size, unsigned char player)
    {
        bool final = true;
        if (len[col] < line + line_size)
            return false;
        for (unsigned char i = line; i < line + line_size; i++)
        {
            if (get_position(i, col) != player)
                return false;
        }
        return final;
    }
    // verifica se existe uma linha horisontal partindo do ponto especificado(sempre para a direita)
    bool is_h_line(unsigned char line, unsigned char col, unsigned line_size, unsigned char player)
    {
        bool final = true;
        if (col + line_size > width)
            return false;
        for (unsigned char j = col; j < col + line_size; j++)
        {
            if (line >= len[j] || get_position(line, j) != player)
                return false;
        }
        return final;
    }
    // calcula a quantidade de linhas horisontais de tamanho n do jogador atual (caso um valor seja passado para player, o número será calculado para o jogador expecificado.)
    unsigned get_h_lines(unsigned line_size, unsigned char player = 2)
    {
        unsigned final = 0;
        player = player == 2 ? turn : player;
        for (unsigned char i = 0; i < height; i++)
        {
            for (unsigned char j = 0; j < width - line_size + 1; j++)
            {
                if (is_h_line(i, j, line_size, player))
                {
                    final++;
                    j += line_size - 1;
                }
            }
        }
        return final;
    }
    // calcula a quantidade de linhas verticais de tamanho n do jogador atual (caso um valor seja passado para player, o número será calculado para o jogador expecificado.)
    unsigned get_v_lines(unsigned line_size, unsigned char player = 2)
    {
        unsigned final = 0;
        player = player == 2 ? turn : player;
        for (unsigned char j = 0; j < width; j++)
        {
            for (unsigned char i = 0; i < height - line_size + 1; i++)
            {
                if (is_v_line(i, j, line_size, player))
                {
                    final++;
                    i += line_size - 1;
                }
            }
        }
        return final;
    }
    // calcula a quantidade de linhas diagonais de tamanho n do jogador atual (caso um valor seja passado para player, o número será calculado para o jogador expecificado.)
    unsigned get_d_lines(unsigned line_size, unsigned char player = 2)
    {
        unsigned final = 0;
        player = player == 2 ? turn : player;
        for (unsigned char i = 0; i < height; i++)
        {
            for (unsigned char j = 0; j < width; j++)
            {
                if (is_dru_line(i, j, line_size, player))
                    final++;
                if (is_dlu_line(i, j, line_size, player))
                    final++;
            }
        }
        return final;
    }
    // calcula a utilidade desse estado do tabuleiro do ponto de vista do player turn==0 (valores mais altos são melhores para ele.)
    float evaluate()
    {
        if (result() == GameResult_Player1_Wins)
            return 1000000;
        if (result() == GameResult_Player2_Wins)
            return -1000000;
        if (result() == GameResult_Draw)
            return 0;
        float score = 0;
        for (int j = 0; j < width; j++)
        {
            for (int i = 0; i < height; i++)
            {
                if (len[j] <= i)
                    continue;
                score += get_position(i, j) == 0 ? square(i, j) : -square(i, j);
            }
        }
        return score;
    }

    // calcula tabuleiros pocíveis a partir deste tabuleiro
    BoardArray calculate_sub_boards();
};

// matriz de tabuleiros, evitar o uso de std vector.
struct BoardArray
{
    GameState board[width];
    unsigned char num_board = 0;
    void insert(GameState board)
    {
        this->board[num_board] = board;
        num_board++;
    }
};

// tive que definir a função fora, ela precisa saber o tamanho da struct BoardArray
BoardArray GameState::calculate_sub_boards()
{
    BoardArray final;
    for (unsigned i = 0; i < width; i++)
    {
        auto temp_board = play(i);
        if (temp_board.is_valid())
            final.insert(temp_board);
    }
    return final;
}