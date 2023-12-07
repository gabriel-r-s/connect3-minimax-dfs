Esta pasta contém os seguintes arquivos:
board.hpp:
implementação de um tabuleiro do jogo connect 4, usa máscaras binárias para ser eficiente.
compile.cmd:
execute esse arquivo para compilar todos os arquivos mencionados aqui.
game_manager.hpp:
Código do Sistema que permite jogar o connect 4 de verdade, no geral, apenas código para atualização de estados, e tratamento de entrada e saída.
game.tree:
Arquivo binário contendo a parte inicial da árvore do jogo para o minimax, é importante para que o minimax faça as jogadas rapidamente, caso não existir o minimax pode levar vários minutos para fazer uma jogada.
generator.cpp:
Código para gerar parte da árvore para aumentar velocidade do minimax durante o jogo.
Pré-calcula uma quantidade específica de níveis da árvore e salva em um arquivo, pode levar horas para executar dependendo da profundidade especificada na variável num_level.
hash_table.hpp:
A implementação da tabela hash para guardar estados durante a busca minimax.
LICENSE:
A licença na qual o repositório do github foi criado
move_sorter.hpp:
A implementação do ordenador de movimentos para a segunda otimização de ordem de exploração, que avalia estados pela quantidade de quadrados que conectam 4 que estão disponíveis.
Play.cpp:
A implementação do jogo connect 4, durante o jogo, use os números de 1 à 7 para fazer uma jogada se um dos jogadores estiverem como humano.
Note que o tabuleiro muda de acordo com o turno, x sempre representa as peças do jogador atual, e o sempre representa as peças do oponente.
Readme.txt:
Este arquivo.
Solver.hpp:
A implementação dos resolvedores, minimax alpha beta e lds.
Test_Lx_Rx:
Os datasets, cada um tem 1000 linhas, cada uma tendo um estado de tabuleiro e uma pontuação esperada, o número depois da letra L indica um nível aproximado na árvore, 1 início, e 3 final, o número após o R indica a dificuldade de do tabuleiro em questão, 1 indica que é fácil para se obter uma vitória partindo desse tabuleiro, e 3 indica uma complexidade maior, como o L3 já está no final da árvore só existe R1 para ele.
Tester.cpp:
O programa para testar os datasets, executará por uma hora caso aberto, e criará arquivos com o nome dos datasets seguido da extensão .out.
Timer.hpp:
Implementação de um temporizador para calcular tempo de execução.
