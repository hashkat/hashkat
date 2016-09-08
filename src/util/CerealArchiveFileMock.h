
#ifndef CEREALARCHIVEFILEMOCK_H_
#define CEREALARCHIVEFILEMOCK_H_

// Google's sparsehash serialization isn't the most flexible, and modifying it heavily is
// also a problem. However, by changing the explicit FILE* to a template type, we can mock
// the FILE* interface, and sort-of meet a middle ground.
// It's not the best, but it's not clear how else to do this without getting too messy.

template <typename Archive>
inline int sparse_hash_fputc(unsigned char chr, Archive& ar) {
    ar(chr);
    return chr;
}
template <typename Archive>
inline int sparse_hash_getc(Archive& ar) {
    unsigned char chr;
    ar(chr);
    return chr;
}

template <typename Archive>
inline int sparse_hash_fwrite(const void* data, int size, int count, Archive& ar) {
    ar.saveBinary((const char*) data, size * count);
    return count;
}

template <typename Archive>
inline int sparse_hash_fread(void* data, int size, int count, Archive& ar) {
    ar.loadBinary((const char*) data, size * count);
    return count;
}

#include <google/sparse_hash_set>

#endif
