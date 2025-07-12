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
        ERROR("db_create() failed");
        fclose(db->file);
        free(db->filename);
        free(db);
        return NULL;
    }

    db->is_open = 1;
    return db;
}

database_t* db_open(const char* filename){
    database_t *db;
    db = malloc(sizeof(database_t));
    if (!db){
        ERROR("malloc failed");
        return NULL;
    }
    db->filename = strdup(filename);
    if(!db->filename){
        free(db);
        return NULL;
    }
    db->file = fopen(db->filename, "rb+");
    if(!db->file){
        free(db->filename);
        free(db);
        ERROR("fopen() failed\n");
        return NULL;
    }
    if(fread(&db->header, sizeof(db_header_t), 1, db->file) != 1){
        ERROR("Failed to read DB header");
        fclose(db->file);
        free(db->filename);
        free(db);
        return NULL;
    }

    if(db->header.magic != MAGIC_NUM) {
        ERROR("File corrupted or not a valid DB");
        fclose(db->file);
        free(db->filename);
        free(db);
        return NULL;
    }
    db->is_open = 1;

    return db;

}

void db_close(database_t* db){
    if(db && db->is_open){
        fseek(db->file, 0, SEEK_SET);
        fwrite(&db->header, sizeof(db_header_t), 1, db->file);

        fclose(db->file);

        free(db->filename);
        free(db);
    }
}

int db_insert(database_t* db, const char* name, uint32_t age, const char* email);
