// implementação de uma tabela hash sem gerenciamento de colisões
#include <cstdint>
#include <vector>
#include <cstring>
#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
// estrutura representando um elemento da tabela
typedef struct
{
    uint64_t key : 56; // a chave do elemento armazenado
    uint8_t val = 0;   // note que o campo acima tem 56bits, e por questão de alinhamento o compilador alocou 64bits para ele, o que significa que os bits mais significativos são usados pelo valor, como nosso tabuleiro só usa os 49 bits menos significativos não à problema
} hash_table_element;
class hash_table
{
    // quantidade de elementos na tabela (multiplique por 16 para obter tamanho em bytes)
    static const unsigned int table_size = 16777259; // número primo
    std::vector<hash_table_element> table;

public:
    hash_table() : table(table_size) {}
    // função para limpar a tabela
    void reset()
    {
        memset(table.data(), 0, table_size * sizeof(hash_table_element));
        hit = 0;
        miss = 0;
    }
    // função para inserir elementos na tabela
    void insert(uint64_t key, uint8_t val)
    {
        unsigned int i = key % table_size;
        ;
        table[i].key = key;
        table[i].val = val;
    }
    // função para obter o valor de um elemento da tabela (0 indica valor não existente)
    uint8_t get(uint64_t key)
    {
        unsigned int i = key % table_size;
        ;
        if (key == table[i].key)
        {
            hit++;
            return table[i].val;
        }
        miss++;
        return 0;
    }
    // contador de hit e miss
    uint64_t hit = 0, miss = 0;
};
#endif