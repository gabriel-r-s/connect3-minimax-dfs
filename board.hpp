// esse arquivo contém uma classe para representar tabuleiros do jogo connect 4.
class board
{
public:
    // largura do tabuleiro
    static const int width = 7;
    // altura do tabuleiro
    static const int height = 6;
    // verifica se existe espaço livre em uma coluna
    bool can_play(int col) const
    {
        return len[col] < height;
    }
    // faz a jogada em uma determinada coluna (não verifica se a jogada é pocível ou não)
    void play(int col)
    {
        board_array[col][len[col]] = 1 + moves % 2;
        moves++;
        len[col]++;
    }
    /*
    faz uma sequência de jogadas.
    Retorna o número de jogadas feitas.
    Pode parar pelos 3 motivos:
    1. todas as jogadas foram feitas
    2. a próxima jogada resultaria em uma vitória do jogador atual.
    3. a próxima jogada seria feita em uma coluna cheia
    */
    unsigned int play(const char *sec)
    {
        unsigned int sec_pos = 0;
        while (sec[sec_pos])
        {
            if (!can_play(sec[sec_pos] - '1') || wins(sec[sec_pos] - '1')) // o jogador atual ganha, ou a coluna está cheia
                break;
            play(sec[sec_pos] - '1');
            sec_pos++;
        }
        return sec_pos;
    }
    // retorna a quantidade de movimentos realisados no tabuleiro
    unsigned int get_moves() const
    {
        return moves;
    }
    // verifica se jogar em uma coluna resulta na vitória do jogador atual
    bool wins(int col) const
    {
        int player = 1 + moves % 2;
        // verifica por alinhamento vertical
        if (len[col] >= 3 && board_array[col][len[col] - 1] == player && board_array[col][len[col] - 2] == player && board_array[col][len[col] - 3] == player)
            return true;
        // verifica diagonais e horisontais
        for (int dy = -1; dy <= 1; dy++) // deslocamento vertical
        {
            int num = 0;                        // número de pedras encontradas que pertencem ao jogador atual
            for (int dx = -1; dx <= 1; dx += 2) // verifica pedras para direita e para esquerda da pedra atual
                for (int x = col + dx, y = len[col] + dx * dy; x >= 0 && x < width && y >= 0 && y < height && board_array[x][y] == player; num++)
                {
                    x += dx;
                    y += dx * dy;
                }
            if (num == 3)
                return true;
        }
        return false;
    }
board(): board_array{0}, len{0} {}
private:
    // contador de movimentos, indica quantas pedras já foram jogadas
    unsigned int moves = 0;
    // matriz do tabuleiro, 0 indica vasio, 1 indica jogador 1 e 2 indica jogador 2
    int board_array[width][height];
    // indica quantas pedras foram jogadas em determinada coluna
    int len[width];
};