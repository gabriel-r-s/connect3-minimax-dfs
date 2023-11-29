// esse arquivo contém uma classe para representar tabuleiros do jogo connect 4.
#include <cstdint>
class board
{
public:
    // largura do tabuleiro
    static const int width = 7;
    // altura do tabuleiro
    static const int height = 6;
    // limite das pontuações
    static const int min_score = -(width * height) / 2 + 3;
    static const int max_score = (width * height) / 2 - 3;
    // verifica se existe espaço livre em uma coluna
    bool can_play(int col) const
    {
        return (stones & top_mask(col)) == 0;
    }
    // retorna o bitfield representando o topo da coluna no argumento
    static uint64_t top_mask(int col)
    {
        return (uint64_t(1) << (height - 1)) << col * (height + 1);
    }
    // faz a jogada em uma determinada coluna (não verifica se a jogada é pocível ou não)
    void play(int col)
    {
        player_stones ^= stones; // o bit field contendo a lista de pedras do jogador atual contém agora as pedras do próximo jogador
        stones |= stones + bottom_mask(col);
        moves++;
    }
    // retorna o bitfield do ponto mais baixo da coluna passada como argumento
    static uint64_t bottom_mask(int col)
    {
        return uint64_t(1) << col * (height + 1);
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
        uint64_t temp = player_stones | ((stones + bottom_mask(col)) & column_mask(col));
        return alignment(temp);
    }
    // retorna um bitfield contendo 1s na coluna passada como argumento
    static uint64_t column_mask(int col)
    {
        return ((uint64_t(1) << height) - 1) << col * (height + 1);
    }
    // verifica se o bitfield das pedras do player passado como argumento tem um alinhamento
    static bool alignment(uint64_t pos)
    {
        // horizontal
        uint64_t m = pos & (pos >> (height + 1));
        if (m & (m >> (2 * (height + 1))))
            return true;

        // diagonal 1
        m = pos & (pos >> height);
        if (m & (m >> (2 * height)))
            return true;

        // diagonal 2
        m = pos & (pos >> (height + 2));
        if (m & (m >> (2 * (height + 2))))
            return true;

        // vertical;
        m = pos & (pos >> 1);
        if (m & (m >> 2))
            return true;

        return false;
    }

private:
    // contador de movimentos, indica quantas pedras já foram jogadas
    unsigned int moves = 0;
    // bit field representando as pedras do jogador atual
    uint64_t player_stones = 0;
    // bit field representando qualquer pedra no tabuleiro
    uint64_t stones = 0;
};