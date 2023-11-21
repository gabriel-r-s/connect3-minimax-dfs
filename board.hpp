enum GameResult
{
    GameResult_Player1_Wins,
    GameResult_Player2_Wins,
    GameResult_Draw,
    GameResult_NotDone
};

enum LineFlags
{
    Line1 = 0x1,
    Line2 = 0x2,
    Line3 = 0x4,
    Line4 = 0x8,
    Line5 = 0x10,
    Line6 = 0x20
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

struct GameState
{
    // cada coluna, usando bit field pra economizar o maximo possivel de memoria
    unsigned col1 : 6;
    unsigned col2 : 6;
    unsigned col3 : 6;
    unsigned col4 : 6;
    unsigned col5 : 6;
    unsigned col6 : 6;
    unsigned col7 : 6;
    // numero de elementos em cada coluna
    unsigned len1 : 3;
    unsigned len2 : 3;
    unsigned len3 : 3;
    unsigned len4 : 3;
    unsigned len5 : 3;
    unsigned len6 : 3;
    unsigned len7 : 3;
    unsigned turn : 1; // indica quem deve ser o próximo a jogar.

    // faz  a jogada na coluna e retorna o novo tabuleiro
    GameState play(unsigned col);

    // verifica se é vitória do player1, player2
    GameResult result();

    // verifica a validade do tabuleiro
    bool is_valid()
    {
        return !(len1==7 || len2==7 || len3==7 || len4==7 || len5==7 || len6==7 || len7==7);
    }
    // calcula a utilidade desse estado do tabuleiro do ponto de vista do player turn==0 (valores mais altos são melhores para ele.)
    float evaluate();

    // calcula tabuleiros pocíveis a partir deste tabuleiro
    BoardArray calculate_sub_boards()
    {
        BoardArray final;
        for (unsigned i = 1; i <= 7; i++)
        {
            auto temp_board = play(i);
            if (temp_board.is_valid())
                final.insert(temp_board);
        }
    }
};
