#ifndef DATAVISITOR_H_
#define DATAVISITOR_H_

#include "lcommon/SerializeBuffer.h"

/* Passed to objects visit() method during writing: */
struct DataWriter {
    DataWriter(const char* file_name)
        : buffer(file_name, SerializeBuffer::OUTPUT, /* Close file: */true){
    }

    template <typename T>
    void visit(const T& obj) {
        buffer.write(obj);
    }

    template <typename T>
    void operator<<(const T& obj) {
        visit(obj);
    }

    template <typename T>
    void visitAll(T& obj) {
        buffer.write_container(obj);
    }

    bool is_reading() {
        return false;
    }

    bool is_writing() {
        return true;
    }

private:
    SerializeBuffer buffer;
};

/* Passed to objects visit() method during reading: */
struct DataReader {
    DataReader(const char* file_name)
        : buffer(file_name, SerializeBuffer::INPUT, /* Close file: */true){
    }

    template <typename T>
    void visit(T& type) {
        buffer.read(type);
    }

    template <typename T>
    void visitAll(T& type) {
        buffer.read_container(type);
    }

    bool is_reading() {
        return true;
    }

    bool is_writing() {
        return false;
    }

private:
    SerializeBuffer buffer;
};


#endif
