// esse arquivo contém uma classe para representar tabuleiros do jogo connect 4.
#include <cstdint>
// retorna um bit field contendo 1 no ponto mais baixo de cada coluna(está declarado fora porque precisamos dele para calcular valores de constantes dentro da classe)
constexpr static uint64_t bottom(int width, int height)
{
    return width == 0 ? 0 : bottom(width - 1, height) | 1LL << (width - 1) * (height + 1);
}
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
    // verifica se o jogador atual pode ganhar  com uma jogada
    bool canWinNext() const
    {
        return winning_position() & possible_moves();
    }

    // faz a jogada em uma determinada coluna (não verifica se a jogada é pocível ou não)
    void play(int col)
    {
        player_stones ^= stones; // o bit field contendo a lista de pedras do jogador atual contém agora as pedras do próximo jogador
        stones |= stones + bottom_mask(col);
        moves++;
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
    // retorna um bitfield contendo 1 para cada posição que o jogador atual pode jogar sem fazer o oponente ganhar no próxino turno
    uint64_t possibleNonLosingMoves() const
    {
        uint64_t possible_mask = possible_moves();
        uint64_t opponent_win = opponent_winning_position();
        uint64_t forced_moves = possible_mask & opponent_win;
        if (forced_moves)
        {
            if (forced_moves & (forced_moves - 1)) // check if there is more than one forced move
                return 0;                          // qualquer lugar vai fazer eu perder, então 0
            else
                possible_mask = forced_moves; // enforce to play the single forced move
        }
        return possible_mask & ~(opponent_win >> 1); // não queremos jogar abaixo de um ponto de vitória do jogador
    }
    // retorna a quantidade de movimentos realisados no tabuleiro
    unsigned int get_moves() const
    {
        return moves;
    }
    // verifica se jogar em uma coluna resulta na vitória do jogador atual
    bool wins(int col) const
    {
        return winning_position() & possible_moves() & column_mask(col);
    }
    // gera uma chave única para o tabuleiro atual
    uint64_t key() const
    {
        return stones + player_stones;
    }

private:
    // contador de movimentos, indica quantas pedras já foram jogadas
    unsigned int moves = 0;
    // bitfield representando as pedras do jogador atual
    uint64_t player_stones = 0;
    // bitfield representando qualquer pedra no tabuleiro
    uint64_t stones = 0;
    // bitfield contendo 1s na parte de baixo de cada coluna
    static const uint64_t bottom_mask_v = bottom(width, height);
    // bitfield contendo a representação de um tabuleiro cheio
    static const uint64_t full_board = bottom_mask_v * ((1ll << height) - 1);
    // retorna um bitfield contendo 1 para cada posição que o jogador atualpode ganhar
    uint64_t winning_position() const
    {
        return compute_winning_position(player_stones, stones);
    }

    // retorna um bitfield contendo 1 para cada posição que o oponente pode ganhar
    uint64_t opponent_winning_position() const
    {
        return compute_winning_position(player_stones ^ stones, stones);
    }
    // retorna um bit field contendo 1 em cada ponto que gera um alinhamento conciderando que as pedras do jogador atual estão no bitfield position, e que o bitfield mask contém as pedras do tabuleiro
    static uint64_t compute_winning_position(uint64_t position, uint64_t mask)
    {
        // vertical;
        uint64_t r = (position << 1) & (position << 2) & (position << 3);

        // horizontal
        uint64_t p = (position << (height + 1)) & (position << 2 * (height + 1));
        r |= p & (position << 3 * (height + 1));
        r |= p & (position >> (height + 1));
        p >>= 3 * (height + 1);
        r |= p & (position << (height + 1));
        r |= p & (position >> 3 * (height + 1));

        // diagonal 1
        p = (position << height) & (position << 2 * height);
        r |= p & (position << 3 * height);
        r |= p & (position >> height);
        p >>= 3 * height;
        r |= p & (position << height);
        r |= p & (position >> 3 * height);

        // diagonal 2
        p = (position << (height + 2)) & (position << 2 * (height + 2));
        r |= p & (position << 3 * (height + 2));
        r |= p & (position >> (height + 2));
        p >>= 3 * (height + 2);
        r |= p & (position << (height + 2));
        r |= p & (position >> 3 * (height + 2));

        return r & (full_board ^ mask);
    }
    // retorna um bitfield contendo 1s na coluna passada como argumento
    static constexpr uint64_t column_mask(int col)
    {
        return ((uint64_t(1) << height) - 1) << col * (height + 1);
    }
    // retorna um bitfield contendo um 1 no ponto mais baixo da coluna passada como argumento
    static constexpr uint64_t bottom_mask(int col)
    {
        return uint64_t(1) << col * (height + 1);
    }
    // retorna o bitfield representando o topo da coluna no argumento
    static constexpr uint64_t top_mask(int col)
    {
        return (uint64_t(1) << (height - 1)) << col * (height + 1);
    }
    // retorna um bitfield contendo 1 para cada ponto jogável do tabuleiro
    uint64_t possible_moves() const
    {
        return (stones + bottom_mask_v) & full_board;
    }
};