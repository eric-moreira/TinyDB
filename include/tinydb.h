#ifndef TINYDB_H
#define TINYDB_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define MAGIC_NUM 0x65726963 // "eric" in hex
#define VERSION 1
#define MAX_NAME_LEN 64
#define MAX_EMAIL_LEN 128
#define ERROR(msg) fprintf(stderr, "ERROR: %s\n", msg)


typedef struct {
    uint32_t magic; 
    uint32_t version;
    uint32_t record_count;
    uint32_t next_id;
    uint32_t deleted_count;
    uint8_t reserved[44]; // 44 bytes reservados para futuras adições
} __attribute__((packed)) db_header_t;

// __atribute__((packed)) vai manter a struct sem gaps no meio 
// e garantir os 64 bytes que eu reservei para o header
// IMPORTANTE para structs que vão pro DISCO


typedef struct {
    uint32_t id;
    uint8_t active;
    char name[MAX_NAME_LEN];
    uint8_t age;
    char email[MAX_EMAIL_LEN];
    uint32_t created_at;
    uint8_t reserved[54]; 
    // espaço reservado para expansões futuras
} __attribute__ ((packed)) db_user_t;


typedef struct {
    FILE* file;
    char* filename;
    db_header_t header;
    uint8_t is_open;
} database_t;

database_t* db_create(const char* filename);
database_t* db_open(const char* filename);
void db_close(database_t* db);

int db_insert(database_t* db, const char* name, uint32_t age, const char* email);

#endif
