
#ifndef SERIALIZEBUFFERFILEMOCK_H_
#define SERIALIZEBUFFERFILEMOCK_H_

// Google's sparsehash serialization isn't the most flexible, and modifying it heavily is
// also a problem. However, by changing the explicit FILE* to a template type, we can mock
// the FILE* interface, and sort-of meet a middle ground.
// It's not the best, but it's not clear how else to do this without getting too messy.

inline int fputc(int chr, SerializeBuffer* buffer) {
	buffer->write_byte(chr);
	return chr;
}
inline int getc(SerializeBuffer* buffer) {
	return buffer->read_byte();
}

inline int fwrite(const void* data, int size, int count, SerializeBuffer* buffer) {
	buffer->write_raw((const char*)data, size*count);
	return count;
}

inline int fread(void* data, int size, int count, SerializeBuffer* buffer) {
	buffer->read_raw((char*)data, size*count);
	return count;
}

#include <google/sparse_hash_set>

#endif
