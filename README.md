# TinyDB

A simple database engine implementation in C that stores data in binary files.

## Features

- **File-based storage**: Data persisted in binary format
- **Record management**: Fixed-size records with automatic ID assignment
- **CRUD operations**: Create, read, update, delete functionality
- **Magic number validation**: File integrity checking
- **Memory management**: Resource cleanup and error handling
- **Cross-platform**: Works on Linux, macOS, and Windows

## Current Implementation

- Database creation and opening
- Record insertion with automatic timestamping
- Binary file I/O operations
- Header management with metadata
- Memory leak prevention
- Search by ID

## Data Structure

Each record contains:
- Unique ID (auto-incremented)
- Active/deleted flag
- Name (64 characters)
- Age (0-255)
- Email (128 characters)
- Creation timestamp

## Building

```bash
make
```

## Usage

```c
#include "tinydb.h"

// Create database
database_t* db = db_create("mydata.db");

// Insert records
db_insert(db, "John Smith", 25, "john@example.com");
db_insert(db, "Jane Doe", 30, "jane@example.com");

// Close database
db_close(db);

// Reopen database
db = db_open("mydata.db");
printf("Records: %u\n", db->header.record_count);
db_close(db);
```

## File Structure

```
TinyDB/
├── include/
│   └── tinydb.h          # Header definitions
├── src/
│   ├── main.c            # Demo application
│   └── tinydb.c          # Core implementation
└── Makefile              # Build configuration
```

## Database File Format

| Offset | Size | Description |
|--------|------|-------------|
| 0      | 4    | Magic number (0x65726963) |
| 4      | 4    | Version |
| 8      | 4    | Record count |
| 12     | 4    | Next ID |
| 16     | 4    | Deleted count |
| 20     | 44   | Reserved space |
| 64+    | 256  | Records (256 bytes each) |

## Planned Features

- [ ] List all records
- [ ] Delete records (logical deletion)
- [ ] Database compaction
- [ ] Index creation
- [ ] Validation utilities
- [ ] Statistics reporting

## Requirements

- C99 compiler (gcc, clang)
- Standard C library
- POSIX-compatible system (for file operations)

## Testing

```bash
make test       # Build and run demo
make valgrind   # Memory leak detection
```
