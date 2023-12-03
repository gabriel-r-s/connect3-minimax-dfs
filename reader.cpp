#include <stdio.h>
#include <iostream>
#include "hash_table.hpp"
using std::cout;
typedef struct
{
    uint64_t key : 56; // a chave do elemento armazenado
    int8_t val = 0;   // note que o campo acima tem 56bits, e por questão de alinhamento o compilador alocou 64bits para ele, o que significa que os bits mais significativos são usados pelo valor, como nosso tabuleiro só usa os 49 bits menos significativos não à problema
} node;
int main()
{
    node e;
    auto f = fopen64("game.tree", "rb");
    while(fread(&e, sizeof(hash_table_element), 1, f))
    {
        cout << e.key << " " << int(e.val) << '\n';
    }
    fclose(f);
}