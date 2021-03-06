// Copyright 1996-2014 by Jon Dart.  All Rights Reserved
#ifndef __BOOKDEFS_H__
#define __BOOKDEFS_H__

// definitions and constants related to the internal format
// of the opening book (BOOK.BIN)

#include "types.h"
extern "C" {
#include <memory.h>
};

namespace book {

const int BOOK_VERSION = 14;

const int INDEX_PAGE_SIZE = 1024;
const int DATA_PAGE_SIZE = 1024;
const uint16 NO_NEXT = 65535;
const uint16 INVALID_INDEX = 65535;
const unsigned NO_RECOMMEND = 1025;
const int MAX_WEIGHT = 1024;

// Header of the opening book.
struct BookHeader
BEGIN_PACKED_STRUCT
   byte version;
   uint16 num_index_pages;
   BookHeader() : version(0), num_index_pages(0) {
   }
END_PACKED_STRUCT

// Information needed to specify the location
// of the head of a move chain
struct BookLocation
BEGIN_PACKED_STRUCT
   BookLocation(uint16 p, uint16 i)
    :page(p),index(i) {
    }
   BookLocation()
    :page(0),index(0) {
    }
    int isValid() const {
       return index != INVALID_INDEX;
    }
    uint16 page;
    uint16 index;
END_PACKED_STRUCT

// Entry in the index table
struct IndexEntry : public BookLocation
BEGIN_PACKED_STRUCT
    IndexEntry(hash_t hash, uint16 p, uint16 i) :
    BookLocation(p,i),hashCode(hash) {
    }
    IndexEntry() :
    BookLocation(), hashCode(0x0ULL) {
    }
    hash_t hashCode;
END_PACKED_STRUCT

struct IndexPage
BEGIN_PACKED_STRUCT
    uint32 next_free;
    IndexEntry index[INDEX_PAGE_SIZE];
    IndexPage() : next_free(0) {
    }
END_PACKED_STRUCT

// Entry in a data page
struct DataEntry
BEGIN_PACKED_STRUCT 
    byte index;
    uint16 next;
    uint16 weight;
    uint32 count;
END_PACKED_STRUCT

struct DataPage
BEGIN_PACKED_STRUCT
    uint32 free_list;
    uint32 num_free;
    DataEntry data[DATA_PAGE_SIZE];
    DataPage() : free_list(0), num_free(DATA_PAGE_SIZE) {
        // create the free list and link all entries into it
        memset(data,'\0',sizeof(DataEntry)*DATA_PAGE_SIZE);
        for (int i = 0; i < DATA_PAGE_SIZE-1; i++) {
            data[i].next = i+1;
        }
        data[DATA_PAGE_SIZE-1].next = NO_NEXT;
    }
END_PACKED_STRUCT

};

#endif
