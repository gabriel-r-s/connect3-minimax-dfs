enum GameResult
{
    GameResult_Player1_Wins,
    GameResult_Player2_Wins,
    GameResult_Draw,
    GameResult_NotDone
};

struct BoardArray; // forward declaration
struct GameState
{
    // array de colunas
    unsigned char cols[7] = {0, 0, 0, 0, 0, 0, 0};
    // numero de elementos em cada coluna
    unsigned char len[7] = {0, 0, 0, 0, 0, 0, 0};
    unsigned char turn = 0; // indica quem deve ser o próximo a jogar.

    // faz  a jogada na coluna e retorna o novo tabuleiro (não verifica validade)
    GameState play(unsigned col)
    {
        GameState final = *this;
        if (col < 7)
        {
            final.cols[col] += (turn << len[col]);
            final.len[col]++;
            final.turn = final.turn ? 0 : 1;
        }
        return final;
    }

    // faz a jogada na coluna especificada e retorna o novo tabuleiro se for válido
    GameState try_play(unsigned char col)
    {
        GameState board = play(col);
        return board.is_valid() ? board : *this;
    }
    // verifica se é vitória do player1, player2
    GameResult result();

    // verifica a validade do tabuleiro
    bool is_valid()
    {
        for (int i = 0; i < 7; i++)
            if (len[i] > 6)
                return false;
        return true;
    }
    // retorna o valor dada uma linha e coluna
    unsigned char get_position(unsigned char line, unsigned char col)
    {
        return (cols[col] >> line) & 1;
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
        if (col + line_size >= 7)
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
        for (unsigned char i = 0; i < 6; i++)
        {
            for (unsigned char j = 0; j < 7 - line_size + 1; j ++)
            {
                if (is_h_line(i, j, line_size, player))
                {
                    final++;
                    j+=line_size-1;
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
        for (unsigned char j = 0; j < 7; j++)
        {
            for (unsigned char i = 0; i < 6-line_size+1; i ++)
            {
                if (is_v_line(i, j, line_size, player))
                {
                    final++;
                    i+=line_size-1;
                }
            }
        }
        return final;
    }

    // calcula a utilidade desse estado do tabuleiro do ponto de vista do player turn==0 (valores mais altos são melhores para ele.)
    float evaluate();

    // calcula tabuleiros pocíveis a partir deste tabuleiro
    BoardArray calculate_sub_boards();
};

struct BoardArray
{
    GameState board[7];
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
    for (unsigned i = 0; i < 7; i++)
    {
        auto temp_board = play(i);
        if (temp_board.is_valid())
            final.insert(temp_board);
    }
    return final;
}