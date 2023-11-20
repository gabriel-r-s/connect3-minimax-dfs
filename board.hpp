enum GameResult {
    GameResult_Player1_Wins,
    GameResult_Player2_Wins,
    GameResult_Draw,
    GameResult_NotDone,
    GameResult_Invalid
};

struct GameState {
    // cada coluna, usando bit field pra economizar o maximo possivel de memoria
    unsigned col1: 4;
    unsigned col2: 4;
    unsigned col3: 4;
    unsigned col4: 4;
    // numero de elementos em cada coluna
    unsigned len1: 3;
    unsigned len2: 3;
    unsigned len3: 3;
    unsigned len4: 3;
    unsigned pading : 4 = 0; //alinhamento de memória, o compilador não gosta de coisas com 28 bits.

    // 'joga' neste tabuleiro na coluna e turno determinados, gerando o filho
    GameState play(unsigned col, bool turn);

    // verifica se é vitória do player1, player2
    GameResult result();
};



