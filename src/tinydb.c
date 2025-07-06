#include "../include/tinydb.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

database_t* db_create(const char* filename) {
    database_t* db = malloc(sizeof(database_t));
    if (!db) return NULL;
    db->filename = strdup(filename);
    if (!db->filename) {
        free(db);
        return NULL;
    }
    db->file = fopen(filename, "wb+");
    if (!db->file) {
        free(db->filename);
        free(db);
        return NULL;
    }

    // inicializa o header 
    db->header.magic = MAGIC_NUM;
    db->header.version = VERSION;
    db->header.deleted_count = 0;
    db->header.record_count = 0;
    db->header.next_id = 1000;

    fseek(db->file, 0, SEEK_SET);
    if(fwrite(&db->header, sizeof(db_header_t), 1, db->file) != 1){
        fprintf(stderr, "db_create() failed");
        return NULL;
    }

    return db;
}



database_t* db_open(const char* filename);
void db_close(database_t* db);

int db_insert(database_t* db, const char* name, uint32_t age, const char* email);
