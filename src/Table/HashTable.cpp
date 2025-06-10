#include "Table/HashTable.h"

size_t HashTable::HashFunc(const Key key) {
    size_t hash = 0x811c9dc5; 
    
    const char* data = key.data();
    size_t size = key.size();
    
    for (size_t i = 0; i < size; ++i) {
        hash ^= static_cast<unsigned char>(data[i]);
        hash *= 0x01000193; 
    }
    
    return hash;
}