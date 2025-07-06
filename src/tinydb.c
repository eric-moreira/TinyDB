#include "../include/tinydb.h"

/* **Função `db_create()`:**
1. Alocar memória para a estrutura database_t
2. Duplicar o nome do arquivo
3. Abrir arquivo em modo "wb+" (escrita binária + leitura)
4. Inicializar o header com valores padrão
5. Escrever header no início do arquivo
6. Retornar ponteiro para database ou NULL em caso de erro
*/
database_t* db_create(const char* filename) {

}



database_t* db_open(const char* filename);
void db_close(database_t* db);

int db_insert(database_t* db, const char* name, uint32_t age, const char* email);
