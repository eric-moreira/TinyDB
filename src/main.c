#include "../include/tinydb.h"

int main() {
   database_t* db = db_create("test.db");
   if (!db) {
       printf("Failed to create database\n");
       return 1;
   }
   
   printf("=== TinyDB Demonstration ===\n");
   
   printf("Inserting records...\n");
   int id1 = db_insert(db, "John Smith", 25, "john@example.com");
   int id2 = db_insert(db, "Sarah Johnson", 30, "sarah@example.com");
   int id3 = db_insert(db, "Mike Wilson", 28, "mike@example.com");
   int id4 = db_insert(db, "Emily Davis", 22, "emily@example.com");
   
   printf("Inserted %u records (IDs: %d, %d, %d, %d)\n", 
          db->header.record_count, id1, id2, id3, id4);
   
   printf("Magic number: 0x%08X\n", db->header.magic);
   printf("Version: %u\n", db->header.version);
   printf("Next ID: %u\n", db->header.next_id);
   
   db_close(db);
   printf("Database closed and saved\n");
   
   printf("\nReopening database...\n");
   db = db_open("test.db");
   if (db) {
       printf("Successfully reopened database\n");
       printf("Records found: %u\n", db->header.record_count);
       printf("Magic: 0x%08X, Version: %u\n", db->header.magic, db->header.version);
       db_close(db);
   } else {
       printf("Failed to reopen database\n");
       return 1;
   }
   
   printf("\nTinyDB demonstration completed successfully!\n");
   return 0;
}