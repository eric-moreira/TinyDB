#include "../include/tinydb.h"


int main() {
    // Criar database
    database_t* db = db_create("test.db");
    if (!db) {
        printf("Erro ao criar database\n");
        return 1;
    }
    
    // Inserir alguns registros
    db_insert(db, "João Silva", 25, "joao@email.com");
    db_insert(db, "Maria Santos", 30, "maria@email.com");
    db_insert(db, "Pedro Lima", 28, "pedro@email.com");
    
    printf("Database criado com %u registros\n", db->header.record_count);
    
    db_close(db);
    
    // Testar abertura
    db = db_open("test.db");
    if (db) {
        printf("Database reaberto: %u registros encontrados\n", db->header.record_count);
        db_close(db);
    }
    
    return 0;
}