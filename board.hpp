enum GameResult
{
    GameResult_Player1_Wins,
    GameResult_Player2_Wins,
    GameResult_Draw,
    GameResult_NotDone
};

struct GameState
{
    // cada coluna, usando bit field pra economizar o maximo possivel de memoria
    unsigned col1 : 4;
    unsigned col2 : 4;
    unsigned col3 : 4;
    unsigned col4 : 4;
    // numero de elementos em cada coluna
    unsigned len1 : 3;
    unsigned len2 : 3;
    unsigned len3 : 3;
    unsigned len4 : 3;
    unsigned turn : 1;       // indica quem deve ser o próximo a jogar.
    unsigned pading : 3 = 0; // alinhamento de memória, o compilador não gosta de coisas com 29 bits.

    // joga na coluna especificada, gerando o próximo tabuleiro, retorna falso se não for uma jogada pocível.
    bool play(unsigned col);

    // verifica se é vitória do player1, player2
    GameResult result();

    // calcula a utilidade desse estado do tabuleiro do ponto de vista do player turn==0 (valores mais altos são melhores para ele.)
    float evaluate()
    {
        auto temp_result = result();
        if(temp_result==GameResult_Player1_Wins) return 100;// melhor caso
        else if(temp_result==GameResult_Player2_Wins) return -100; // pior caso
        else if(temp_result==GameResult_Draw) return 0;// tudo igual.
        else// bora calcular coisas
        {
            return 1;// código temporário.
        }
    }
};
