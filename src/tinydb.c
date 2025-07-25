#include "../include/tinydb.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


database_t* db_create(const char* filename) {
    database_t* db = malloc(sizeof(database_t));
    if (!db) return NULL;
    db->filename = malloc(strlen(filename) + 1);
    if (!db->filename) {
            free(db);
            return NULL;
    }
    strcpy(db->filename, filename);
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
    db->filename = malloc(sizeof(filename) + 1);
    strcpy(db->filename, filename);
    
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

int db_insert(database_t* db, const char* name, uint32_t age, const char* email){
    if(!db || !db->is_open) return -1;

    user_record_t record;
    memset(&record, 0, sizeof(user_record_t));

    record.id = db->header.next_id++;
    record.active = 1;
    strncpy(record.name, name, MAX_NAME_LEN -1);
    record.age = age;
    strncpy(record.email, email, MAX_EMAIL_LEN -1);
    record.created_at = (uint32_t)time(NULL);

    fseek(db->file, 0, SEEK_END);

    if(fwrite(&record, sizeof(user_record_t), 1, db->file) != 1){
        fprintf(stderr, "fwrite() failed to insert");
        return -1;
    }
    db->header.record_count++;
    return record.id;

}


/*
I am using linear search as the db file is using linear writing at this point
This will change in the future when I create indexes and add_table function 
*/

user_record_t db_search_by_id(database_t* db, uint32_t id){
    user_record_t record, not_found = {0};
    
    if(!db || !db->is_open){
        fprintf(stderr, "db is invalid or not open");
        return not_found;
    }
    if(fseek(db->file, sizeof(db_header_t), SEEK_SET) != 0){
        fprintf(stderr, "fseek failed to fetch the records");
        return not_found;
    }

    while(fread(&record, sizeof(user_record_t), 1, db->file) == 1){
        if(record.id == id) {
            return record;
        }
    }

    return not_found;

}